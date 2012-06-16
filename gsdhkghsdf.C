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
	//s.Bind(IPAddress("192.168."),60000);
	//Socket s(AF_INET,SOCK_STREAM,0);
	//IPEndPoint dst(IPAddress("192.168.101.1"),2223);
	//s.Connect(dst);
	Buffer b(129);
	//b.Data[128]='\x00';
	StandardStream ss;
	vector<IPAddress> base{"192.168.100.1","192.168.101.1"};
	vector<int> ports{2222,2223,3333,3334,7000,4444,4445,5555,5556,2322,2323};
	while(true)
	{
		int br=ss.Read(b.SubBuffer(0,128));
		if(br<=0)return 0;
		b.Data[br]='\x00';
		//s.SendTo(b.SubBuffer(0,br),dst);
		//s.Send(b.SubBuffer(0,br));
		BufferRef bufr=b.SubBuffer(0,br+1);
		for(auto it=base.begin();it!=base.end();it++)
		{
			for(int i=0;i<21;i++)
			{
				IPAddress tmp=(*it)+i;
				try
				{
					for(auto it1=ports.begin();it1!=ports.end();it1++)
						s.SendTo(bufr,IPEndPoint(tmp,*it1));
				}catch(Exception& ex){}
			}
		}
		cerr << "sent" << endl;
	}
}
