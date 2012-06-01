#include <iostream>
#include <vector>
//#include <SFML/Audio.hpp>
//#include <SFML/Graphics.hpp>
#include <cplib/asyncsock.hpp>
using namespace std;
using namespace xaxaxa;
using namespace Sockets;
int main()
{
	//IPEndPoint ep(IPAddress("0.0.0.0"),26969);
	Socket s(AF_INET,SOCK_DGRAM,0);
	//Socket s(AF_INET,SOCK_STREAM,0);
	IPEndPoint dst(IPAddress("192.168.100.8"),6666);
	//s.Connect(dst);
	Buffer b(4096);
	StandardStream ss;
	while(true)
	{
		int br=ss.Read(b);
		if(br<=0)return 0;
		s.SendTo(b.SubBuffer(0,br),dst);
		//s.Send(b.SubBuffer(0,br));
	}
}
