#include <iostream>
#include <cpoll/cpoll.H>
#include <signal.h>
#include <math.h>

using namespace CP;

static const int bufSize=4096*16;
int main(int argc, char** argv)
{
	if(argc<3)
	{
		cerr << "usage: " << argv[0] << " IP PORT" << endl;
		return 1;
	}
	int port;
	const char* ip="0.0.0.0";
	ip=argv[1];
	port=atoi(argv[2]);
	signal(SIGPIPE, SIG_IGN);
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart functions if	 interrupted by handler */
	sigaction(SIGHUP, &sa, NULL);
	
	StandardStream ss;
	ss.setBlocking(true);
	MemoryStream ms;
	ss.readToEnd(ms);
	char* buf;
	int bs=1024*16;
	if(ms.length()>bs) {
		bs=ms.length();
		buf=(char*)ms.data();
	}
	else {
		bs=ceil(double(bs)/ms.length())*ms.length();
		buf=(char*)malloc(bs);
		if(buf==NULL) {
			perror("malloc");
			return 1;
		}
		for(int i=0;i<bs;i+=ms.length()) {
			memcpy(buf+i,ms.data(),ms.length());
		}
	}
	
	Socket* s=new Socket(AF_INET,SOCK_STREAM);
	struct handler {
		Poll& p;
		Stream* s1;
		Stream* s2;
		char buf1[bufSize];
		String buf2;
		handler(Poll& p, Stream* s1, Stream* s2, String data)
			:p(p),s1(s1),s2(s2),buf2(data) {}
		void stop() { exit(0); }
		void closed(int i) { stop(); }
		void start() {
			read1();
			write2();
		}
		void read1() { s1->read(buf1,bufSize,Callback(&handler::read1cb,this)); }
		void read1cb(int r) {
			if(r<=0) { s2->close(Callback(&handler::closed,this)); return; }
			write1(r);
		}
		
		void write1(int i) { s2->write(buf1,i,Callback(&handler::write1cb,this)); }
		void write2() { s1->writeAll(buf2.data(),buf2.length(),Callback(&handler::write2cb,this)); }
		void write1cb(int r) {
			if(r<=0) { stop(); return; }
			read1();
		}
		void write2cb(int r) {
			if(r<=0) { stop(); return; }
			write2();
		}
	}* hdlr;
	
	CP::Poll p;
	struct {
		Poll& p;
		Socket& s;
		Stream& ss;
		String data;
		void operator()(int i) {
			if(i<0) throw runtime_error("Connection error");
			(new handler(p,&s,&ss,data))->start();
		}
	} cb {p,*s,ss,{buf,bs}};
	s->connect(IPEndPoint(IPAddress(ip), port), &cb);
	
	p.add(*s);
	ss.addToPoll(p);
	p.loop();
}
