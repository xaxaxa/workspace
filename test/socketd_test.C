#include <cpoll/statemachines.H>
#include <cpoll/cpoll.H>
#include <unistd.h>
#include <cpoll/sendfd.H>
#include <iostream>
#include <socketd.H>
using namespace std;
using namespace socketd;
using namespace CP;

class SocketProxy: public CP::Socket
{
public:
	uint8_t* buf;
	int bufPos,bufLen;
	SocketProxy(int fd, int d, int t, int p, int buflen)
		:CP::Socket(fd,d,t,p), bufPos(0), bufLen(buflen) {
		buf=new uint8_t[buflen];
	}
	
	~SocketProxy() {
		delete[] buf;
	}
};
class socketd_client
{
public:
	CP::Poll& p;
	RGC::Ref<CP::Socket> sock;
	function<void(Socket*,const function<void()>&)> cb;
	protocolHeader ph;
	prot_handleConnection ph1;
	void startRead();
	void readCB(int r) {
		if(r<=0) {
			cb(NULL,NULL);
			return;
		}
		switch(ph.type) {
			case protocolHeader::handleConnection:
			{
				sock->read(&ph1, sizeof(ph1), [this](int r) {
					if(r<=0) {
						cb(NULL,NULL);
						return;
					}
					int fd=recvfd(sock->handle);
					if(fd<0) {
						cb(NULL,NULL);
						return;
					}
					
					auto cb1=[this](CP::Socket* newsock) {
						p.add(*newsock);
						int64_t id=ph1.id;
						//printf("aaaaa %lli %i %i %i\n",ph1.id, ph1.d, ph1.t, ph1.p);
						cb(newsock, [this,id]() {
							protocolHeader ph;
							ph.type=protocolHeader::ackConnection;
							prot_ackConnection ack;
							ack.id=id;
							ack.success=true;
							sock->write(&ph,sizeof(ph));
							sock->write(&ack,sizeof(ack));
						});
						newsock->release();
						startRead();
					};
					//printf("asdfg %i\n",ph1.bufferLen);
					if(ph1.bufferLen<=0) {
						CP::Socket* newsock;
						newsock=new CP::Socket(fd, ph1.d, ph1.t, ph1.p);
						cb1(newsock);
					} else {
						SocketProxy* newsock;
						newsock=new SocketProxy(fd, ph1.d, ph1.t, ph1.p, ph1.bufferLen);
						sock->read(newsock->buf, ph1.bufferLen, [newsock,cb1,this](int r) {
							if(r<=0) {
								cb(NULL,NULL);
								return;
							}
							//write(2,newsock->buf,r);
							cb1(newsock);
						});
					}
					
				});
				return;
			}
		}
		startRead();
	}
	socketd_client(CP::Poll& p, const function<void(Socket*,const function<void()>&)>& cb, CP::Socket* sock=NULL):p(p),cb(cb) {
		if(sock==NULL) {
			sock=RGC::newObj<CP::Socket>(3, AF_UNIX, SOCK_STREAM, 0);
			p.add(*sock);
		}
		this->sock=sock;
		startRead();
	}
};
void socketd_client::startRead() {
	sock->read(&ph, sizeof(ph), [this](int r){readCB(r);});
}

int main() {
	cout << "started child #" << getpid() << endl;
	//CP::Socket s(3, AF_UNIX, SOCK_STREAM, 0);
	//protocolHeader ph;
	int reqs=0;
	/*while(s.read(&ph, sizeof(ph))>0) {
		if(ph.type==protocolHeader::handleConnection) {
			cout << "protocolHeader::handleConnection" << endl;
			prot_handleConnection ph1;
			if(s.read(&ph1,sizeof(ph1))<=0) return 1;
			//cout << "read prot_handleConnection" << endl;
			int fd=recvfd(s.handle);
			//cout << "received fd " << fd << endl;
			const char* resp="HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n"
			"Connection: close\r\nContent-Length: 9\r\n\r\ntrollolol";
			write(fd,resp,strlen(resp));
			if((++reqs)>5) {
				//recycle
				return 0;
			}
		}
	}*/
	CP::Poll p;
	socketd_client cl(p, [&](Socket* s, const function<void()>& ack) {
		if(s==NULL)exit(0);
		/*if((++reqs)>500) {
			//recycle
			exit(0);
		}*/
		ack();
		const char* resp="HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n"
			"Connection: close\r\nDate: Mon, 11 Feb 2013 22:27:33 GMT\r\n"
			"Server: asdfghjk/1.2.34\r\n\r\ntrollolololololololololololololololololololol"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo"
			"lolololololololololololololololololololololololololololololololololololololololo";
		s->retain();
		s->write(resp,strlen(resp),[s](int r) {s->shutdown(SHUT_WR);});
		uint8_t* buf=new uint8_t[4096];
		s->repeatRead(buf, 4096, [buf,s](int r) {
			if(r<=0) {
				//printf("connection ended\n");
				delete buf;
				s->release();
			}
		});
	});
	
	p.loop();
}
