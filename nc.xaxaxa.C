#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>

using namespace xaxaxa;
using namespace Sockets;

int main(int argc, char** argv)
{
	if(argc<3)
	{
		cout << "usage: " << argv[0] << " ip port" << endl;
		return 1;
	}
	signal(SIGPIPE, SIG_IGN);
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart functions if	 interrupted by handler */
	sigaction(SIGHUP, &sa, NULL);
	
	StandardStream ss;
	Socket s(AF_INET,SOCK_STREAM,0);
	s.Connect(IPEndPoint(IPAddress(argv[1]),atoi(argv[2])));
	Buffer b(1024*64);
	int br;
	while((br=s.Recv(b))>0)
	//while((br=recv(s._f,b.Data,b.Length,0))>0)
	{
		ss.Write(b);
		//write(1,b.Data,b.Length);
	}
	ss.Close();
}
