#include <cpoll/statemachines.H>
#include <cpoll/cpoll.H>
#include <unistd.h>
#include <cpoll/sendfd.H>
#include <iostream>
#include <socketd.H>
#include <signal.h>
#include <socketd_client.H>

using namespace std;
using namespace socketd;
using namespace CP;
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
	struct {
		int& reqs;
		CP::Poll& p;
		void operator()(socketd_client& cl, Socket* s, int64_t id) {
			if(s==NULL)kill(getpid(),9);
			/*if((++reqs)>500) {
				//recycle
				exit(0);
			}*/
			cl.ack(id);
			
			struct handler:public RGC::Object {
				Socket* s;
				uint8_t buf[4096];
				void writeCB(int r) {
					s->shutdown(SHUT_WR);
					release();
				}
				void readCB(int r) {
					if(r<=0) {
						release();
					}
				}
				handler(Socket* s):s(s) {
					s->retain();
					this->retain(2);
					s->write(resp,strlen(resp), CP::Callback(&handler::writeCB,this));
					s->repeatRead(buf, 4096, CP::Callback(&handler::readCB,this));
				}
				~handler() {
					s->release();
				}
			}* hdlr=new handler(s);
			hdlr->release();
		}
	} cb {reqs, p};
	socketd_client cl(p, &cb);
	p.loop();
}
