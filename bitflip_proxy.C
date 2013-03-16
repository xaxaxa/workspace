#include <iostream>
//#include <cplib/cplib.hpp>
//#include <cplib/asyncsock.hpp>
#include <cpoll.H>
#include <signal.h>

//name is wrong; it does bitshift not bitflip

//using namespace xaxaxa;
//using namespace Sockets;
using namespace CP;

static const int bufSize=4096;

int main(int argc, char** argv)
{
	if(argc<5)
	{
		cerr << "usage: " << argv[0] << " bind_host bind_port forward_host forward_port [r]" << endl;
		return 1;
	}
	//no need to disable SIGHUP and SIGPIPE as cpoll does that automatically
	Poll p;
	Socket srvsock;
	p.add(srvsock);
	srvsock.bind(IPEndPoint(IPAddress(argv[1]),atoi(argv[2])));
	struct conf {
		IPEndPoint fwd;
		bool r;
	} cfg {{IPAddress(argv[3]),(in_port_t)atoi(argv[4])},argc>5&&argv[5][0]=='r'};
	struct {
		conf& cfg;
		Socket& srvsock;
		Poll& p;
		bool rev;
		void operator()(HANDLE h) {
			struct handler {
				conf& cfg;
				Poll& p;
				Socket s;
				Socket s2;
				char buf1[bufSize];	//local -> remote
				char buf2[bufSize]; //remote -> local
				bool rev;
				handler(conf& cfg,Poll& p, HANDLE h, int d, int t, int pr)
					:cfg(cfg),p(p),s(h,d,t,pr) {}
				void stop() { delete this; }
				void closed(int i) { stop(); }
				void transform(uint8_t* buf, int len) {
					if(cfg.r)
						for(int i=0;i<len;i++)
							buf[i]-=69;
					else
						for(int i=0;i<len;i++)
							buf[i]+=69;
				}
				void start() {
					p.add(s);
					p.add(s2);
					s2.connect(cfg.fwd, Callback(&handler::connectCB,this));
				}
				void read1() { s.recv(buf1,bufSize,0,Callback(&handler::read1cb,this)); }
				void read2() { s2.recv(buf2,bufSize,0,Callback(&handler::read2cb,this)); }
				void read1cb(int r) {
					if(r<=0) { s2.close(Callback(&handler::closed,this)); return; }
					transform((uint8_t*)buf1,r);
					write1(r);
				}
				void read2cb(int r) {
					if(r<=0) { s.close(Callback(&handler::closed,this)); return; }
					transform((uint8_t*)buf2,r);
					write2(r);
				}
				
				void write1(int i) { s2.send(buf1,i,0,Callback(&handler::write1cb,this)); }
				void write2(int i) { s.send(buf2,i,0,Callback(&handler::write2cb,this)); }
				void write1cb(int r) {
					if(r<=0) { stop(); return; }
					read1();
				}
				void write2cb(int r) {
					if(r<=0) { stop(); return; }
					read2();
				}
				void connectCB(int r) {
					//printf("%i\n",r);
					if(r<0) { stop(); return; }
					read1();
					read2();
				}
			}* hdlr=new handler(cfg,p,h,srvsock.addressFamily,srvsock.type,srvsock.protocol);
			hdlr->start();
		}
	} cb {cfg,srvsock,p};
	srvsock.repeatAcceptHandle(&cb);
	srvsock.listen();
	p.loop();
}
