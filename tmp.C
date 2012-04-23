#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>

using namespace xaxaxa;
using namespace Sockets;
int main()
{
	int itr=0;
	signal(SIGPIPE, SIG_IGN);
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart functions if	 interrupted by handler */
	sigaction(SIGHUP, &sa, NULL);
	
	Socket s(AF_INET,SOCK_STREAM,0);
	s.Bind(IPEndPoint(IPAddress("0.0.0.0"),16969));
	s.Listen(10);
	while(true)
	{
		Socket c=s.Accept();
		//int tmp12345=1;
		//setsockopt(c._s,SOL_SOCKET,SO_LINGER,&tmp12345,sizeof(tmp12345));
		try
		{
			SocketStream ss(c);
			StreamReader sr(ss);
			StringBuilder sb1;
			while(sr.ReadLine(sb1)>0)
			{
				cout << sb1.ToCString() << endl;
				sb1.Clear();
			}
			
			itr++;
			StringBuilder sb;
			sb<<itr;
			
			sb << " troll";
			for(int i=0;i<100;i++)
				sb << "ol";
			sb << "\n";
			
			for(int i=0;i<1000;i++)
				c.Send(sb.ToBuffer());
			c.Shutdown(SHUT_WR);
			Buffer b(4096);
			while(c.Recv(b)>0);
		}catch(Exception& ex){}
		try
		{
			c.Close();
		}catch(Exception& ex){}
	}
	s.Close();
}
