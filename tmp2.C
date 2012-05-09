#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>

using namespace xaxaxa;
using namespace Sockets;
int counter=0;
StringBuilder sb;
struct tmp_s
{
	//Buffer b;
	SocketManager::Callback cb;
};
FUNCTION_DECLWRAPPER(cb1, void, SocketManager* m, Socket sock)
{
	try
	{
		Socket s = m->EndAccept(sock);
		WARN(1,"accepted " << ++counter << "th client");
		StringBuilder sb1;
		sb1 << counter << "th client\n";
		m->BeginSend(s, sb1.ToBuffer(), SocketManager::Callback([](void* v, SocketManager* m, Socket sock)
		{
			try
			{
				m->EndSend(sock);
				tmp_s* zxcv=new tmp_s();
				zxcv->cb=SocketManager::Callback([](void* v, SocketManager* m, Socket sock)
				{
					tmp_s* zxcv=(tmp_s*)v;
					try
					{
						if(m->EndSend(sock)<=0)goto close;
						m->BeginSend(sock, sb.ToBuffer(), zxcv->cb, true);
					}catch(Exception& ex){goto close;}
					return;
				close:
					m->Cancel(sock);
					sock.Close();
					delete zxcv;
				}, zxcv);
				m->BeginSend(sock, sb.ToBuffer(), zxcv->cb, true);
			}catch(Exception& ex){sock.Close();}
		}, NULL), true);
	}catch(Exception& ex){}
	m->BeginAccept(sock, SocketManager::Callback(cb1, NULL));
}
int main()
{
	signal(SIGPIPE, SIG_IGN);
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart functions if	 interrupted by handler */
	sigaction(SIGHUP, &sa, NULL);
	
	StringBuilder sb1;
	sb1 << "troll";
	for(int i=0;i<30;i++)
		sb1 << " lol";
	sb1 << "\n";
	//sb << ' ';
	for(int i=0;i<5000;i++)
	{
		sb << sb1.ToBuffer();
	}
	Socket s(AF_INET,SOCK_STREAM,0);
	s.Bind(IPEndPoint(IPAddress("0.0.0.0"),16969));
	s.Listen(10);
	SocketManager* m = SocketManager::GetDefault();
	m->BeginAccept(s, SocketManager::Callback(cb1, NULL));
	m->EventLoop();
}
