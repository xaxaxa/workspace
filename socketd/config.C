/*
 * config.cpp
 *
 *  Created on: 2011-05-20
 *      Author: xaxaxa
 *
 *
 * TODO: zombies are not cleaned up
 */
#include "include/config.H"
#include <cpoll/sendfd.H>
#include <stdexcept>
#include <unistd.h>
#include <stdint.h>
#include <rgc.H>
#include <signal.h>

//8: debug; 5: info; 3: warn; 2: err; 1: fatal
#define SOCKETD_DEBUG(LEVEL, ...) printf(__VA_ARGS__)
namespace socketd
{
	static int asdf = 0;
	void spawnApp(vhost* vh, CP::Poll& p, string exepath);
	struct connectionInfo
	{
		socketd* This;
		listen* l;
		CP::Socket* s;
		CP::Poll* p;
		int tries;
		connectionInfo() :
				tries(0) {
		}
		//transfer socket to application
		void do_transfer(vhost* vh) {
			//cout << "do_transfer" << endl;
			retry: if ((++tries) > 3) {
				SOCKETD_DEBUG(4, "exceeded 3 tries for connection %p\n", s);
				delete this;
				return;
			}
			if (vh->conn() == NULL && vh->exepath.length() > 0) {
				spawnApp(vh, *p, vh->exepath);
			}
			if (vh->attachmentConn() != NULL) {
				int r =
						vh->attachmentConn->passConnection(s, NULL, 0,
								[this,vh](bool b) {
									if(b) {
										SOCKETD_DEBUG(8,"received acknownedgement for connection %p (with attachment)\n",s);
										delete this;
									}
									else do_transfer(vh);
								});
				if (r == 1) { //fail
					goto aaaaa;
				} else if (r == 0) { //success
					SOCKETD_DEBUG(8, "connection %p pre-succeeded (with attachment)\n", s);
					delete this;
					return;
				} else return;
			}
			aaaaa: if (vh->conn() != NULL) {
				//cout << "vh->conn() != NULL" << endl;
				auto tmpptr = vh->conn();
				int r = tmpptr->passConnection(s, NULL, 0, [this,vh,tmpptr](bool b) {
					if(b) {
						SOCKETD_DEBUG(8,"received acknownedgement for connection %p\n",s);
						delete this;
					}
					else {
						if(tmpptr==vh->conn()) {
							vh->conn->shutDown();
							vh->conn=NULL;
						}
						do_transfer(vh);
					}
				});
				if (r == 1) {
					//application possibly dead; respawn
					vh->conn->shutDown();
					vh->conn = NULL;
					goto retry;
				} else if (r == 0) {
					SOCKETD_DEBUG(8, "connection %p pre-succeeded\n", s);
					delete this;
					return;
				} else return;
			} else {
				//no handler found; reset connection
				SOCKETD_DEBUG(5, "no handler for connection %p\n", s);
				delete this;
			}

		}
		void process() {
			//figure out what needs to be read to decide which binding to use

			//0: none; 1: reqLine; 2: headers
			int readTo = 0;
			for (uint32_t i = 0; i < This->bindings.size(); i++) {
				if ((!This->bindings[i]->match_listenID) || This->bindings[i]->listenID == l->id) {
					if (This->bindings[i]->matchLevel & binding::match_httpHost) {
						readTo = 2;
						break;
					} else if (This->bindings[i]->matchLevel & binding::match_httpPath) {
						readTo = 1;
						break;
					} else {
						//matched
						do_transfer(This->bindings[i]->vh);
						return;
					}
				}
			}
			printf("readTo=%i\n", readTo);
			switch (readTo) {
				default:
					delete this;
					break;
			}
		}
		~connectionInfo() {
			s->release();
		}
	};
	appConnection::appConnection(vhost* vh) :
			vh(vh) {
		//if(vh->conn==NULL) vh->conn=this;
	}

	appConnection::~appConnection() {
		//if(vh!=NULL && vh->conn==this) vh->conn=NULL;
	}
	struct appConnection_unix: public appConnection
	{
		RGC::Ref<CP::Socket> unixsock;

		//not yet acknowledged
		map<int64_t, passConnCB> pendingConnections;
		int64_t maxID;
		protocolHeader buf;
		prot_ackConnection buf1;
		CP::Poll& p;
		//char sbuf[sizeof(protocolHeader)+sizeof(prot_handleConnection)];
		bool dead;
		bool down;
		virtual void shutDown() {
			if (!down) {
				down = true;
				unixsock->close([](int r) {});
			}
		}
		void die(int64_t ignoreID) {
			if (dead) return;
			dead = true;
			for (auto it = pendingConnections.begin(); it != pendingConnections.end(); it++) {
				if ((*it).first != ignoreID) (*it).second(false);
			}
			pendingConnections.clear();
			shutDown();
		}
		void startRead();
		void readCB(int r) {

			if (r <= 0) {
				SOCKETD_DEBUG(5, "application died; r=%i; errno: %s\n", r, strerror(errno));
				die(0);
			}
			if (dead) {

				release();
				return;
			}

			switch (buf.type) {
				case protocolHeader::ackConnection:
				{
					unixsock->read(&buf1, sizeof(buf1), [this](int r) {
						if (r <= 0) {
							die(0);
						}
						if (dead) {
							release();
							return;
						}
						auto it=pendingConnections.find(buf1.id);
						if(it!=pendingConnections.end()) {
							(*it).second(buf1.success);
							pendingConnections.erase(it);
						}
						startRead();
					});
					break;
				}
				default:
					startRead();
					break;
			}
		}
		appConnection_unix(vhost* vh, CP::Poll& p, string exepath) :
				appConnection(vh), maxID(0), p(p), dead(false), down(false) {
			int socks[2];
			if (socketpair(AF_UNIX, SOCK_STREAM, 0, socks) < 0) {
				throw runtime_error(strerror(errno));
			}
			pid_t pid = fork();
			if (pid < 0) throw runtime_error(strerror(errno));
			else if (pid == 0) {
				//child
				close(socks[0]);
				dup2(socks[1], 3); //fd 3
				close(socks[1]);
				execlp(exepath.c_str(), exepath.c_str(), (const char*) NULL);
				_exit(1);
			} else {
				//parent
				close(socks[1]);
				unixsock = RGC::newObj<CP::Socket>(socks[0], AF_UNIX, SOCK_STREAM, 0);
				p.add(*unixsock);
				printf("asdfg %i\n", ++asdf);
				retain();
				startRead();
			}
		}
		virtual int passConnection(CP::Socket* s, void* buffer, int buflen, const passConnCB& cb) {
			if (dead) return 1;
			//printf("passConnection\n");
			//s->retain();
			int64_t id = (++maxID);
			int r;
			{
				int len = sizeof(protocolHeader) + sizeof(prot_handleConnection);
				uint8_t* hdr[len];
				memset(hdr, 0, len);
				protocolHeader* ph = new (hdr) protocolHeader();
				prot_handleConnection* ph1 =
						new (hdr + sizeof(protocolHeader)) prot_handleConnection();

				ph->type = protocolHeader::handleConnection;
				ph1->id = id;
				ph1->d = s->addressFamily;
				ph1->t = s->type;
				ph1->p = s->protocol;
				ph1->bufferLen = buflen;
				r = unixsock->send(hdr, len, 0);
				ph->~protocolHeader();
				ph1->~prot_handleConnection();
			}
			if (r <= 0) goto fail;
			if (buflen > 0) if (unixsock->send(buffer, buflen, 0) <= 0) goto fail;
			if (sendfd(unixsock->handle, s->handle) < 0) goto fail;
			//s->release();
			pendingConnections.insert( { id, cb });
			return 2;
			fail: //s->release();
			die(id);
			return 1;
		}
		virtual ~appConnection_unix() {
			p.del(*unixsock);
			printf("asdfg %i\n", --asdf);
			//throw runtime_error("fuck");
		}
	};
	void appConnection_unix::startRead() {
		unixsock->read(&buf, sizeof(buf), [this](int r) {this->readCB(r);});
	}
	void spawnApp(vhost* vh, CP::Poll& p, string exepath) {
		vh->conn = RGC::newObj<appConnection_unix>(vh, p, exepath);
	}
	void socketd::run() {
		//ignore SIGCHLD
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART; /* Restart system calls if
		 interrupted by handler */
		sigaction(SIGCHLD, &sa, NULL);

		this->bindings.clear();
		for (uint32_t i = 0; i < vhosts.size(); i++) {
			for (uint32_t ii = 0; ii < vhosts[i].bindings.size(); ii++) {
				this->bindings.push_back(&(vhosts[i].bindings[ii]));
				vhosts[i].bindings[ii].vh = &vhosts[i];
			}
			vhosts[i].hasAttachments = false;
		}
		CP::Poll p;

		for (uint32_t i = 0; i < listens.size(); i++) {
			auto l = listens[i];
			if (l.sock.get() == NULL) {
				l.sock = RGC::newObj<CP::Socket>(l.ep->addressFamily, SOCK_STREAM);
				l.sock->bind(*l.ep);
				l.sock->listen(l.backlog);
			}
			p.add(*l.sock);
			l.sock->repeatAccept([&l,&p,this](CP::Socket* s) {
				//cout << "accepted connection" << endl;
					connectionInfo* ci=new connectionInfo();
					ci->This=this;
					ci->l=&l;
					ci->s=s;
					ci->p=&p;
					//cout << "p=" << &p << endl;
					ci->process();
				});
		}
		p.loop();
	}
}
