#include <iostream>
//#include <cplib/cplib.hpp>
//#include <cplib/asyncsock.hpp>
#include <cpoll.H>
#include <signal.h>

//using namespace xaxaxa;
//using namespace Sockets;
using namespace CP;

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
	
	
	Socket* s;
	if(listen) {
		Socket lsock(AF_INET,SOCK_STREAM);
		lsock.bind(IPEndPoint(IPAddress(ip), port));
		lsock.listen(1);
		s=lsock.accept();
	} else {
		s=new Socket(AF_INET,SOCK_STREAM);
		s->connect(IPEndPoint(IPAddress(ip), port));
	}
	CP::Poll p;
	p.add(*s);
	CP::File* s_in=new CP::File(0);
	CP::File* s_out=new CP::File(1);
	p.add(*s_in);
	p.add(*s_out);
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
	s_in->read(buf2,sizeof(buf2),recvcb2);
	p.loop();
}
