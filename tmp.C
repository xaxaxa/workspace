#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>

#define NICK "hussain12"
#define USER "hussain12"
#define RNAME "hussain"
#define SERV "irc.freenode.net"
#define PORT 8001

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
	s.Connect({"89.16.176.16",PORT});
	s.BeginRead()
	
	stringstream ss;
	string serv=SERV;
	string port=PORT;
	ss << "CAP LS\r\nNICK " << NICK << "\r\nUSER "<< USER << " " << USER << " " << "irc.freenode.org" << " :" << RNAME << "\r\n" << "CAP END\r\n";
	string s=ss.str();
	
	
	SocketManager* m=SocketManager::GetDefault();
	m->EventLoop();
}
