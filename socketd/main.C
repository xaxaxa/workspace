/*
 * main.cpp
 *
 *  Created on: 2011-05-20
 *      Author: xaxaxa
 */
#include "include/config.H"
#include <cpoll.H>
using namespace RGC;
using namespace socketd;
void listenthread();
int main(int argc,char** argv)
{
	listenthread();
}
void listenthread()
{
	//CP::Poll p;
	socketd::socketd sd;
	sd.listens.push_back({newObj<CP::IPEndPoint>(CP::IPAddress("0.0.0.0"),16969),1,32});
	sd.vhosts.push_back({{{0,"/asdf","",binding::match_httpPath}},"vhost1","/home/xaxaxa/workspace/test/socketd_test",""});
	sd.vhosts.push_back({{{0,"/zxcv","",binding::match_httpPath}},"vhost2","/home/xaxaxa/workspace/test/socketd_test",""});

	//sd.vhosts.push_back({{{1,"","",binding::match_listenID}},"vhost1","/home/xaxaxa/workspace/test/socketd_test",""});
	sd.run();
	/*config::rtconfigmanager *c=config::rtconfigmanager::getmainconfig();
	SocketManager m;
	int i;
	for(i=0;i<c->listens_c;i++)
	{
		Socket s(AF_INET,SOCK_STREAM|SOCK_CLOEXEC,0);
		s.Bind(c->listens[i].ep);
		s.Listen(c->listens[i].backlog);
		m.BeginAccept(s,SocketManager::Callback(cb1,NULL));
	}
	m.EventLoop();*/

}
