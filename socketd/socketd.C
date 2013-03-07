/*
 * config.cpp
 *
 *  Created on: 2011-05-20
 *      Author: xaxaxa
 *
 *
 */
/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
#include "include/socketd_internal.H"
#include <cpoll/sendfd.H>
#include <cpoll/statemachines.H>
#include <stdexcept>
#include <unistd.h>
#include <stdint.h>
#include <rgc.H>
#include <signal.h>
#include <tuple>
#include <ctype.h>

#define SOCKETD_THREADING

#define SOCKETD_READBUFFER 256
using namespace std;
//8: debug; 5: info; 3: warn; 2: err; 1: fatal
#define SOCKETD_DEBUG(LEVEL, ...) if(LEVEL<6)printf(__VA_ARGS__)
namespace socketd
{
	static const int nthreads = 2;
	static int asdf = 0;
	static const int rBufSize = 4096;
	static const int rLineBufSize = 512;
	void spawnApp(vhost* vh, CP::Poll& p, string exepath, int threadID);
	bool comparePath(const char* conf, int confLen, const char* path, int pathLen) {
		//cout << string(path, pathLen) << endl;
		if (confLen == pathLen && memcmp(conf, path, confLen) == 0) {
			/*cout << "matched (exact): " << string(path, pathLen) << " against " << string(conf, confLen)
			 << endl;*/
			return true;
		}
		if (pathLen > confLen && memmem(path, pathLen, conf, confLen) == path
				&& (confLen == 1 || path[confLen] == '/')) {
			/*cout << "matched (substring): " << string(path, pathLen) << " against " << string(conf, confLen)
			 << endl;*/
			return true;
		}
		//
		return false;
	}
	bool compareHost(const char* conf, int confLen, const char* host, int hostLen) {
		if (confLen == hostLen && memcmp(conf, host, confLen) == 0) {
			return true;
		}
		if (confLen >= 1 && conf[0] == '*') {
			if (((const char*) memmem(host, hostLen, conf + 1, confLen - 1)) - host
					== (hostLen - (confLen - 1))) {
				return true;
			}
		}
		return false;
	}
	SocketDException::SocketDException() :
			message(strerror(errno)), number(errno) {
	}
	SocketDException::SocketDException(int32_t number) :
			message(strerror(number)), number(number) {
	}
	SocketDException::SocketDException(string message, int32_t number) :
			message(message), number(number) {
	}
	SocketDException::~SocketDException() throw () {
	}
	const char* SocketDException::what() const throw () {
		return message.c_str();
	}
	char* strupper(char* s, int len) {
		for (char* p = s; *p; ++p)
			*p = toupper(*p);
		return s;
	}

	char* strlower(char* s, int len) {
		for (char* p = s; *p; ++p)
			*p = tolower(*p);
		return s;
	}
	struct connectionInfo
	{
		socketd* This;
		listen* l;
		CP::Socket* s;
		CP::Poll* p;
		//CP::streamReader* sr;
		char _sr[sizeof(CP::persistentStreamReader)];

		int tries;
		//0: none; 1: reqLine; 2: headers
		int readTo;
		int pos;
		int threadID;
		int processIndex;

		//int lineBufLen;
		string httpPath;
		string httpHost;
		bool firstLine;
		bool reading;
		bool cancelread;
		bool deletionFlag;
		bool streamReaderInit;

		connectionInfo() :
				tries(0), readTo(0), pos(0), processIndex(-1), deletionFlag(false),
						streamReaderInit(false) {
		}
		void startRead();
		void checkMatch();
		void startSocketRead();
		void processLine(uint8_t* buf, int len) {
			//uint8_t* lineBuf = ((uint8_t*) sr) + CP::streamReader_getSize() + rBufSize;
			uint8_t* lineBuf = buf;
			int lineBufLen = len;
			//printf("got line: ");
			//write(1, lineBuf, lineBufLen);
			//printf("\n");
			if (firstLine) {
				firstLine = false;
				uint8_t* tmp = (uint8_t*) memchr(lineBuf, ' ', lineBufLen);
				if (tmp == NULL) goto fail;
				tmp++;
				if (lineBuf + lineBufLen - tmp <= 0) goto fail;
				uint8_t* tmp1 = (uint8_t*) memchr(tmp, ' ', lineBuf + lineBufLen - tmp);
				if (tmp1 == NULL) goto fail;
				const char* path = (const char*) tmp;
				int pathLen = tmp1 - tmp;
				if (pathLen <= 0) goto fail;
				pos = 1;
				httpPath = string(path, pathLen);
				checkMatch();
				return;
			}

			const uint8_t* tmp;
			tmp = (const uint8_t*) memchr(buf, ':', len);
			if (tmp == NULL) return;
			int i;
			i = tmp - buf;
			if (i > 1024) goto fail;
			char fieldname[i + 1];
			memcpy(fieldname, buf, i);

			return;
			fail: delete this;
		}
		void readCB(uint8_t* buf, int len) {
			/*uint8_t* lineBuf;
			 if (len <= 0) goto aaa;
			 //overflowed the line buffer
			 if (lineBufLen + len > rLineBufSize) goto fail;
			 lineBuf = ((uint8_t*) sr) + CP::streamReader_getSize() + rBufSize;
			 memcpy(lineBuf + lineBufLen, buf, len);
			 lineBufLen += len;
			 aaa: if (last) {
			 cancelread = true;
			 processLine();
			 }
			 return;
			 fail: delete this;*/
			cancelread = true;
			processLine(buf, len);
		}
		inline int connIndex(vhost* vh) {
			return threadID * vh->processes + processIndex;
		}
		//transfer socket to application
		void do_transfer(vhost* vh) {
			//cout << "do_transfer" << endl;
			retry: if ((++tries) > 3) {
				SOCKETD_DEBUG(3, "exceeded 3 tries for connection %p\n", this);
				if (reading) deletionFlag = true;
				else delete this;
				return;
			}
			if (processIndex < 0) {
				processIndex = (vh->curProcess[threadID]++) % vh->processes;
			}
			if (vh->conns[connIndex(vh)]() == NULL && vh->exepath.length() > 0) {
				spawnApp(vh, *p, vh->exepath, connIndex(vh));
			}
			uint8_t* buf;
			int bufLen;
			if (streamReaderInit) {
				CP::persistentStreamReader* sr = (CP::persistentStreamReader*) _sr;
				std::tie(buf, bufLen) = sr->getHistory();
			} else {
				buf = NULL;
				bufLen = 0;
			}
			if (vh->attachmentConn() != NULL) {
				int r =
						vh->attachmentConn->passConnection(s, NULL, 0,
								[this,vh](bool b) {
									if(b) {
										SOCKETD_DEBUG(8,"received acknownedgement for connection %p (with attachment)\n",this);
										delete this;
									}
									else {
										do_transfer(vh);
									}
								});
				if (r == 1) { //fail
					goto aaaaa;
				} else if (r == 0) { //success
					SOCKETD_DEBUG(8, "connection %p pre-succeeded (with attachment)\n", this);
					delete this;
					return;
				} else return;
			}
			aaaaa: if (vh->conns[connIndex(vh)]() != NULL) {
				//cout << "vh->conn() != NULL" << endl;
				auto tmpptr = vh->conns[connIndex(vh)]();
				SOCKETD_DEBUG(8, "bufLen=%i\n", bufLen);
				int r = tmpptr->passConnection(s, buf, bufLen, [this,vh,tmpptr](bool b) {
					if(b) {
						SOCKETD_DEBUG(8,"received acknownedgement for connection %p\n",this);
						delete this;
					}
					else {
						if(tmpptr==vh->conns[connIndex(vh)]()) {
							vh->conns[connIndex(vh)]->shutDown();
							vh->conns[connIndex(vh)]=NULL;
						}
						do_transfer(vh);
					}
				});
				if (r == 1) {
					//application possibly dead; respawn
					tmpptr->shutDown();
					if (tmpptr == vh->conns[connIndex(vh)]()) vh->conns[connIndex(vh)] = NULL;
					goto retry;
				} else if (r == 0) {
					SOCKETD_DEBUG(8, "connection %p pre-succeeded\n", this);
					delete this;
					return;
				} else return;
			} else {
				//no handler found; reset connection
				SOCKETD_DEBUG(5, "no handler for connection %p\n", this);
				delete this;
			}

		}
		void process() {
			SOCKETD_DEBUG(9, "connectionInfo::process()\n");
			checkMatch();
		}

		~connectionInfo() {
			SOCKETD_DEBUG(9, "~connectionInfo\n");
			s->release();
			if (streamReaderInit) {
				CP::persistentStreamReader* sr = (CP::persistentStreamReader*) _sr;
				sr->~persistentStreamReader();
			}
		}
	};
	void connectionInfo::startSocketRead() {
		if (reading) return;
		CP::persistentStreamReader* sr = (CP::persistentStreamReader*) _sr;
		auto tmp = sr->beginPutData(SOCKETD_READBUFFER);
		SOCKETD_DEBUG(9,
				"attempting to read %i bytes of data from client socket\n", SOCKETD_READBUFFER);
		reading = true;
		s->read(tmp, SOCKETD_READBUFFER, [this](int r) {
			SOCKETD_DEBUG(9, "got %i bytes of data from client socket\n", r);
			CP::persistentStreamReader* sr=(CP::persistentStreamReader*)_sr;
			sr->endPutData(r);
			reading=false;
			if(deletionFlag) {
				delete this; return;
			}
			if(cancelread)return;
			if(r<=0) {
				uint8_t* buf; int len;
				std::tie(buf,len) = sr->getBufferData();
				sr->clearBuffer();
				readCB(buf, len);
				return;
			}
			startSocketRead();
		});
	}
	void connectionInfo::checkMatch() {
		//figure out what needs to be read to decide which binding to use

		//0: none; 1: reqLine; 2: headers
		//int readTo = 0;
		if (pos < readTo) return;
		for (uint32_t i = 0; i < This->bindings.size(); i++) {
			if (!(This->bindings[i]->matchLevel & This->bindings[i]->match_listenID)
					|| This->bindings[i]->listenID == l->id) {
				if (This->bindings[i]->matchLevel & binding::match_httpPath) {
					if (pos < 1) {
						readTo = 1;
						break;
					} else {
						if (comparePath(This->bindings[i]->httpPath.data(),
								This->bindings[i]->httpPath.length(), httpPath.data(), httpPath.length())) {
							goto matched_httpPath;
						} else continue;
					}
				} else {
					matched_httpPath: if (This->bindings[i]->matchLevel & binding::match_httpHost) {
						if (pos < 2) {
							readTo = 2;
							break;
						} else {
							if (comparePath(This->bindings[i]->httpHost.data(),
									This->bindings[i]->httpHost.length(), httpHost.data(),
									httpHost.length())) {
								goto matched_httpHost;
							} else continue;
						}
					} else {
						matched_httpHost: do_transfer(This->bindings[i]->vh);
						return;
					}
				}
			}
		}
		SOCKETD_DEBUG(9, "readTo=%i pos=%i\n", readTo, pos);
		if (readTo > pos) {
			if (pos == 0) {
				//initialize streamReader
				CP::persistentStreamReader* sr;
				sr = new (_sr) CP::persistentStreamReader();
				streamReaderInit = true;
				//if (sr == NULL) goto fail;
				//CP::streamReader_init(sr, rBufSize);
				firstLine = true;
				sr->output = [this](uint8_t* buf, int len) {readCB(buf,len);};
				reading = false;
				p->add(*s);
			}
			startRead();
		} else goto fail;
		return;
		fail: if (reading) deletionFlag = true;
		else delete this;
	}
	void connectionInfo::startRead() {
		CP::persistentStreamReader* sr = (CP::persistentStreamReader*) _sr;
		sr->readUntilString("\r\n");
		cancelread = false;
		startSocketRead();
	}
	appConnection::appConnection() {
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
		socketd* sd;
		set<vhost*> bound_vhosts;
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
			//throw 5;
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
						//printf("%i\n",buf1.id);
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
				maxID(0), p(p), dead(false), down(false) {
			int socks[2];
			if (socketpair(AF_UNIX, SOCK_STREAM, 0, socks) < 0) {
				throw runtime_error(strerror(errno));
			}
			pid_t pid = fork();
			if (pid < 0) throw runtime_error(strerror(errno));
			else if (pid == 0) {
				//child
				close(socks[0]);
				if (socks[1] != 3) {
					dup2(socks[1], 3); //fd 3
					close(socks[1]);
				}
				setenv("SOCKETD_FD", "3", 1);
				if (vh->preload) {
					setenv("LD_PRELOAD", socketd_proxy, 1);
				}
				if (vh->useShell) execlp("/bin/sh", "/bin/sh", "-c", exepath.c_str(),
						(const char*) NULL);
				else execlp(exepath.c_str(), exepath.c_str(), (const char*) NULL);
				_exit(1);
			} else {
				//parent
				close(socks[1]);
				int n;
				unsigned int n_size = sizeof(n);
				n = 1024 * 1024 * 128;
				setsockopt(socks[0], SOL_SOCKET, SO_RCVBUF, (void *) &n, n_size);
				setsockopt(socks[0], SOL_SOCKET, SO_SNDBUF, (void *) &n, n_size);
				//getsockopt(socks[0], SOL_SOCKET, SO_RCVBUF, (void *) &n, &n_size);
				//SOCKETD_DEBUG(8, "unix socket receive buffer size: %i\n", n);

				unixsock = RGC::newObj<CP::Socket>(socks[0], AF_UNIX, SOCK_STREAM, 0);
				p.add(*unixsock);
				//printf("asdfg %i\n", ++asdf);
				retain();
				startRead();
			}
		}
		appConnection_unix(CP::Socket* sock, CP::Poll& p, socketd* sd) :
				maxID(0), p(p), sd(sd), dead(false), down(false) {

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
				prot_handleConnection* ph1 = new (ph + 1) prot_handleConnection();

				ph->type = protocolHeader::handleConnection;
				//printf("zxcv %lli\n",id);
				ph1->id = id;
				ph1->d = s->addressFamily;
				ph1->t = s->type;
				ph1->p = s->protocol;
				ph1->bufferLen = buflen;
				//socket has SOCK_NONBLOCK set, so regular send() won't block;
				//if the socket buffer is full, then the application is already
				//considered dead
				r = unixsock->sendAll(hdr, len, MSG_DONTWAIT);
				ph->~protocolHeader();
				ph1->~prot_handleConnection();
			}
			if (r <= 0) goto fail;
			if (sendfd(unixsock->handle, s->handle) < 0) goto fail;
			if (buflen > 0) if (unixsock->sendAll(buffer, buflen, MSG_DONTWAIT) <= 0) goto fail;
			//s->release();
			pendingConnections.insert( { id, cb });
			return 2;
			fail: //s->release();
			SOCKETD_DEBUG(1, "unix socket buffer overflow; %s\n", strerror(errno));
			die(id);
			return 1;
		}
		virtual ~appConnection_unix() {
			p.del(*unixsock);
			//printf("asdfg %i\n", --asdf);
			//throw runtime_error("fuck");
		}
	};
	void appConnection_unix::startRead() {
		unixsock->read(&buf, sizeof(buf), [this](int r) {this->readCB(r);});
	}
	void spawnApp(vhost* vh, CP::Poll& p, string exepath, int connIndex) {
		vh->conns[connIndex] = RGC::newObj<appConnection_unix>(vh, p, exepath);
	}

	struct socketd_execinfo;
	struct socketd_thread
	{
		socketd* This;
		socketd_execinfo* execinfo;
		pthread_t thr;
		int pipe[2];
		int id;
	};
	struct socketd_execinfo
	{
		vector<socketd_thread> threads;
	};
	struct socketd_request
	{
		listen* l;
		CP::HANDLE fd;
	};
	void* socketd_processorThread(void* v) {
		socketd_thread* th = (socketd_thread*) v;
		CP::Poll p;
		CP::File pipe(th->pipe[0]);
		socketd_request req;
		printf("%i %i\n", th->id, th->pipe[0]);
		pipe.repeatRead(&req, sizeof(req), [&p,&pipe,&req,th](int r) {
			if(r!=sizeof(socketd_request))return;
			//printf("%i\n",th->id);
				connectionInfo* ci = new connectionInfo();
				ci->threadID=th->id;
				ci->This = th->This;
				ci->l = req.l;
				ci->s = new CP::Socket(req.fd, req.l->sock->addressFamily, req.l->sock->type,
						req.l->sock->protocol);
				ci->p = &p;
			//cout << "p=" << &p << endl;
				ci->process();
			});
		p.add(pipe);
		p.loop();
		printf("%i exited\n", th->id);
		return NULL;
	}
	void socketd::run() {
//ignore SIGCHLD
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART; /* Restart system calls if
		 interrupted by handler */
		sigaction(SIGCHLD, &sa, NULL);

		CP::Poll p;
		this->bindings.clear();
		for (uint32_t i = 0; i < vhosts.size(); i++) {
			for (uint32_t ii = 0; ii < vhosts[i].bindings.size(); ii++) {
				binding* tmp = &(vhosts[i].bindings[ii]);
				this->bindings.push_back(tmp);
				vhosts[i].bindings[ii].vh = &vhosts[i];
			}
			vhosts[i].hasAttachments = false;
#ifdef SOCKETD_THREADING
			vhosts[i].conns.resize(nthreads*vhosts[i].processes);
			vhosts[i].curProcess.resize(nthreads);
#else
			vhosts[i].conns.resize(vhosts[i].processes);
			vhosts[i].curProcess.resize(1);
#endif
			for (uint32_t ii = 0; ii < vhosts[i].curProcess.size(); ii++) {
				vhosts[i].curProcess[ii] = 0;
			}
		}
		for (uint32_t i = 0; i < extraBindings.size(); i++) {
			for (uint32_t ii = 0; ii < vhosts.size(); ii++) {
				if (vhosts[ii].name == extraBindings[i].vhostName) {
					extraBindings[i].vh = &vhosts[ii];
					break;
				}
			}
			binding* tmp = &(extraBindings[i]);
			this->bindings.push_back(tmp);
		}
		//start up unix listening socket
		if (unixAddress.length() > 0) {
			CP::Socket* unixsock = new CP::Socket(AF_UNIX, SOCK_STREAM);
			unixsock->bind(CP::UNIXEndPoint(unixAddress));
			unixsock->listen(8);
			unixsock->repeatAccept([](CP::Socket* s) {

			});
		}

#ifdef SOCKETD_THREADING
		socketd_execinfo execinfo;
		printf("this=%p\n", this);
		execinfo.threads.resize(nthreads);
		for (int i = 0; i < nthreads; i++) {
			socketd_thread& th=execinfo.threads[i];
			th.This = this;
			th.execinfo = &execinfo;
			th.id = i;
			if (pipe(th.pipe) < 0) {
				throw runtime_error(strerror(errno));
			}
			//printf("%p %p\n",&th, &th1);
			if (pthread_create(&th.thr, NULL, socketd_processorThread, &th) != 0) {
				throw runtime_error(strerror(errno));
			}
		}
		int curThread = 0;
#endif
		for (uint32_t i = 0; i < listens.size(); i++) {
			auto& l = listens[i];
			if (l.sock.get() == NULL) {
				//l.sock = RGC::newObj<CP::Socket>(l.ep->addressFamily, SOCK_STREAM);
				l.sock = RGC::newObj<CP::Socket>();
				l.sock->bind(l.host.c_str(), l.port.c_str(), AF_UNSPEC, SOCK_STREAM);
				l.sock->listen(l.backlog);
			}
			p.add(*l.sock);

#ifdef SOCKETD_THREADING
			l.sock->repeatAcceptHandle([&l,&p,this,&execinfo,&curThread](CP::HANDLE h) {
						socketd_request req {&l, h};
						//printf("%i\n",curThread);
						write(execinfo.threads[curThread].pipe[1],&req,sizeof(req));
						curThread++;
						if(curThread>=nthreads)curThread=0;
					});
#else
			l.sock->repeatAcceptHandle([&l,&p,this](CP::HANDLE h) {
				connectionInfo* ci = new connectionInfo();
				ci->threadID=0;
				ci->This = this;
				ci->l = &l;
				ci->s = new CP::Socket(h, l.sock->addressFamily, l.sock->type,
						l.sock->protocol);
				ci->p = &p;
				//cout << "p=" << &p << endl;
					ci->process();
				});
#endif

		}
		p.loop();
		printf("fdsjkhgfsdjh\n");
	}
}
