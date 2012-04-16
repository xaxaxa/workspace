#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>

using namespace xaxaxa;
using namespace Sockets;
int counter=0;
StringBuilder sb;
struct tmp_s
{
	
};
FUNCTION_DECLWRAPPER(cb1, void, SocketManager* m, Socket sock)
{
	try
	{
		Socket s = m->EndAccept(sock);
		WARN(1,"accepted " << ++counter << "th client");
		StringBuilder sb1;
		sb1 << counter;
		m->BeginSend(s, sb1.ToBuffer(), SocketManager::Callback([](void* v, SocketManager* m, Socket sock)
		{
			try
			{
				m->EndSend(sock);
				m->BeginSend(sock, sb.ToBuffer(), SocketManager::Callback([](void* v, SocketManager* m, Socket sock)
				{
					try
					{
						m->EndSend(sock);
						sock.Close();
					}catch(Exception& ex){}
				}, NULL));
			}catch(Exception& ex){}
		}, NULL));
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
	
	sb << " troll";
	for(int i=0;i<100;i++)
		sb << " lol";
	sb << "\n";
	
	Socket s(AF_INET,SOCK_STREAM,0);
	s.Bind(IPEndPoint(IPAddress("0.0.0.0"),16969));
	s.Listen(10);
	SocketManager* m = SocketManager::GetDefault();
	m->BeginAccept(s, SocketManager::Callback(cb1, NULL));
	m->EventLoop();
}
