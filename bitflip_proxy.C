#include <iostream>
//#include <cplib/cplib.hpp>
//#include <cplib/asyncsock.hpp>
#include <cpoll.H>
#include <signal.h>


//using namespace xaxaxa;
//using namespace Sockets;
using namespace CP;

static const int bufSize=4096;

int main(int argc, char** argv)
{
	if(argc<5)
	{
		cerr << "usage: " << argv[0] << " bind_host bind_port forward_host forward_port" << endl;
		return 1;
	}
	{
		signal(SIGPIPE, SIG_IGN);
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART; /* Restart functions if	 interrupted by handler */
		sigaction(SIGHUP, &sa, NULL);
		sigaction(SIGPIPE, &sa, NULL);
	}
	Poll p;
	Socket srvsock;
	p.add(srvsock);
	srvsock.bind(IPEndPoint(IPAddress(argv[1]),atoi(argv[2])));
	IPEndPoint ep1(IPAddress(argv[3]),atoi(argv[4]));
	struct {
		EndPoint& fwd;
		Socket& srvsock;
		Poll& p;
		void operator()(HANDLE h) {
			struct handler {
				Poll& p;
				EndPoint& fwd;
				Socket s;
				Socket s2;
				char buf1[bufSize];	//local -> remote
				char buf2[bufSize]; //remote -> local
				int closedN;
				handler(Poll& p, EndPoint& fwd, HANDLE h, int d, int t, int pr)
					:p(p),fwd(fwd),s(h,d,t,pr),closedN(-1) {}
				void stop() { delete this; }
				void closed(int i) { stop(); }
				void start() {
					p.add(s);
					p.add(s2);
					s2.connect(fwd, Callback(&handler::connectCB,this));
				}
				void read1() { s.recv(buf1,bufSize,0,Callback(&handler::read1cb,this)); }
				void read2() { s2.recv(buf2,bufSize,0,Callback(&handler::read2cb,this)); }
				void read1cb(int r) {
					if(r<=0) { s2.close(Callback(&handler::closed,this)); return; }
					write1(r);
				}
				void read2cb(int r) {
					if(r<=0) { s.close(Callback(&handler::closed,this)); return; }
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
			}* hdlr=new handler(p,fwd,h,srvsock.addressFamily,srvsock.type,srvsock.protocol);
			hdlr->start();
		}
	} cb {ep1,srvsock,p};
	srvsock.repeatAcceptHandle(&cb);
	srvsock.listen();
	p.loop();
}
