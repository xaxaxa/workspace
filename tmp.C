#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>

using namespace xaxaxa;
using namespace Sockets;
int main()
{
	StringBuilder sb;
	sb << "troll";
	for(int i=0;i<100;i++)
		sb << "ol";
	sb << "\n";
	Socket s(AF_INET,SOCK_STREAM,0);
	s.Bind(IPEndPoint(IPAddress("0.0.0.0"),16969));
	s.Listen(10);
	while(true)
	{
		Socket c=s.Accept();
		//int tmp12345=1;
		//setsockopt(c._s,SOL_SOCKET,SO_LINGER,&tmp12345,sizeof(tmp12345));
		for(int i=0;i<1000;i++)
			c.Send(sb.ToBuffer());
		c.Shutdown(SHUT_WR);
		Buffer b(4096);
		while(c.Recv(b)>0);
		c.Close();
		
	}
	s.Close();
}
