/*
 * main.cpp
 *
 *  Created on: 2011-05-20
 *      Author: xaxaxa
 */
#include "config.hpp"
#include <asyncsock.hpp>

using namespace xaxaxa;
using namespace Sockets;

void listenthread();
int main(int argc,char** argv)
{
	listenthread();
}
FUNCTION_DECLWRAPPER(cb1,void,SocketManager* m,Socket sock)
{
	Socket s=m->EndAccept(sock);
	dbgprint("accepted: " << s._s);
	//new client(s);
	m->BeginAccept(sock,SocketManager::Callback(cb1,NULL));
}
void listenthread()
{
	config::rtconfigmanager *c=config::rtconfigmanager::getmainconfig();
	SocketManager m;
	int i;
	for(i=0;i<c->listens_c;i++)
	{
		Socket s(AF_INET,SOCK_STREAM|SOCK_CLOEXEC,0);
		s.Bind(c->listens[i].ep);
		s.Listen(c->listens[i].backlog);
		m.BeginAccept(s,SocketManager::Callback(cb1,NULL));
	}
	m.EventLoop();
}
