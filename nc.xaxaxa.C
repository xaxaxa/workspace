#include <iostream>
//#include <cplib/cplib.hpp>
//#include <cplib/asyncsock.hpp>
#include <cpoll/cpoll.H>
#include <signal.h>

//using namespace xaxaxa;
//using namespace Sockets;
using namespace CP;

static const int bufSize=4096*16;
int main(int argc, char** argv)
{
	if(argc<3)
	{
		cerr << "usage: " << argv[0] << " [-l [IP] PORT] | IP PORT" << endl;
		return 1;
	}
	bool listen=false;
	int port;
	const char* ip="0.0.0.0";
	if((listen=(strcmp(argv[1],"-l")==0))) {
		if(argc>=4) {
			ip=argv[2];
			port=atoi(argv[3]);
		} else port=atoi(argv[2]);
	} else {
		ip=argv[1];
		port=atoi(argv[2]);
	}
	signal(SIGPIPE, SIG_IGN);
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart functions if	 interrupted by handler */
	sigaction(SIGHUP, &sa, NULL);
	/*
	StandardStream ss;
	Socket s(AF_INET,SOCK_STREAM,0);
	//s.Bind(IPEndPoint(IPAddress(argv[1]),atoi(argv[2])));
	s.Connect(IPEndPoint(IPAddress(argv[1]),atoi(argv[2])));
	Buffer b(1024*64);
	int br;
	while((br=s.Recv(b))>0)
	//while((br=recv(s._f,b.Data,b.Length,0))>0)
	{
		ss.Write(b.SubBuffer(0,br));
		//write(1,b.Data,b.Length);
	}
	ss.Close();*/
	
	
	
	
	
	
	Socket* s=new Socket(AF_INET,SOCK_STREAM);
	
	struct handler {
		Poll& p;
		Stream* s1;
		Stream* s2;
		char buf1[bufSize];
		char buf2[bufSize];
		handler(Poll& p, Stream* s1, Stream* s2)
			:p(p),s1(s1),s2(s2) {}
		void stop() { exit(0); }
		void closed(int i) { stop(); }
		void start() {
			read1();
			read2();
		}
		void read1() { s1->read(buf1,bufSize,Callback(&handler::read1cb,this)); }
		void read2() { s2->read(buf2,bufSize,Callback(&handler::read2cb,this)); }
		void read1cb(int r) {
			if(r<=0) { s2->close(Callback(&handler::closed,this)); return; }
			write1(r);
		}
		void read2cb(int r) {
			if(r<=0) { s1->close(Callback(&handler::closed,this)); return; }
			write2(r);
		}
		
		void write1(int i) { s2->writeAll(buf1,i,Callback(&handler::write1cb,this)); }
		void write2(int i) { s1->writeAll(buf2,i,Callback(&handler::write2cb,this)); }
		void write1cb(int r) {
			if(r<=0) { stop(); return; }
			read1();
		}
		void write2cb(int r) {
			if(r<=0) { stop(); return; }
			read2();
		}
	}* hdlr;
	
	CP::Poll p;
	StandardStream ss;
	if(listen) {
		s->bind(IPEndPoint(IPAddress(ip), port));
		s->listen(1);
		struct {
			Poll& p;
			Stream* ss;
			void operator()(Socket* s) {
				p.add(*s);
				(new handler(p,s,ss))->start();
			}
		} cb {p,&ss};
		s->accept(&cb);
	} else {
		struct {
			Poll& p;
			Socket* s;
			Stream* ss;
			void operator()(int i) {
				if(i<0) throw runtime_error("Connection error");
				(new handler(p,s,ss))->start();
			}
		} cb {p,s,&ss};
		s->connect(IPEndPoint(IPAddress(ip), port), &cb);
	}
	
	p.add(*s);
	ss.addToPoll(p);
	//hdlr->start();
	/*
	
	char buf1[4096*16], buf2[4096*16];
	Callback recvcb1;
	recvcb1=[&](int32_t r) {
		if(r<=0) {
			exit(0);
		}
		s_out->write(buf1,r,[&](int32_t r) {
			s->recv(buf1,sizeof(buf1),0,recvcb1);
		});
	};
	Callback recvcb2;
	recvcb2=[&](int32_t r) {
		if(r<=0) {
			return;
		}
		//cout << "read " << r << " bytes" << endl;
		s->send(buf2,r,0,[&](int32_t r) {
			s_in->read(buf2,sizeof(buf2),recvcb2);
		});
	};
	s->recv(buf1,sizeof(buf1),0,recvcb1);
	s_in->read(buf2,sizeof(buf2),recvcb2);*/
	p.loop();
}
