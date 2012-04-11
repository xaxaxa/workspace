/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>
#include <cplib/asyncfile.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "JoinStream.h"
#include <boost/shared_ptr.hpp>
#include "DNSServer.H"

#define DEFPORT 6969
using namespace boost;
using namespace std;
using namespace xaxaxa;
using namespace Sockets;
#define SO_ORIGINAL_DST 80
struct tmp123
{
	Socket s1, s2;
	IPEndPoint ep;
	JoinStream* j;
};
extern SocketManager *m;
static void socks_cb1(void* obj, Stream* s, void* v)
{
	tmp123* tmp = (tmp123*)obj;
	dbgprint("################socks sent_callback##################");
	tmp->j->dowrite2 = true;
	tmp->j->begin2w();
}
static void socks_cb(void* obj, Stream* s, void* v)
{
	tmp123* tmp = (tmp123*)obj;
	try
	{
		SOCKS5::socks_endconnect(v);
	}
	catch(Exception& ex)
	{
		tmp->j->Close();
		delete tmp;
		return;
	}
	dbgprint("################socks connected##################");
	tmp->j->Begin();
	delete tmp;
}

FUNCTION_DECLWRAPPER(cb_connect, void, SocketManager* m, Socket sock)
{
	tmp123* tmp = (tmp123*)obj;
	tmp->j = NULL;
	try
	{
		m->EndConnect(sock);
		dbgprint("connected");
	}
	catch(Exception& ex)
	{
		tmp->s1.Close();
		tmp->s2.Close();
		delete tmp;
		return;
	}
	try
	{
		SocketStream* str1 = new SocketStream(tmp->s1);
		SocketStream* str2 = new SocketStream(tmp->s2);
		JoinStream *j = new JoinStream(str1, str2);
		str1->Release();
		str2->Release();
		//j->Begin();
		//delete tmp;
		tmp->j = j;
		j->begin1r();
		SOCKS5::socks_connect(j->s2, &(tmp->ep), SOCKS5::Callback(socks_cb, tmp), SOCKS5::Callback(socks_cb1, tmp));
	}
	catch(Exception& ex)
	{
		if(tmp->j != NULL) tmp->j->Close();
		delete tmp;
	}
}
const char* socks_host = "127.0.0.1";
int socks_port = 9999;
FUNCTION_DECLWRAPPER(cb1, void, SocketManager* m, Socket sock)
{
	Socket s = m->EndAccept(sock);
	dbgprint("accepted: " << s._s);
	//new client(s);
	Socket s2(AF_INET, SOCK_STREAM, 0);
	//dbgprint("asdf2");
	tmp123* tmp = NULL;
	try {
		sockaddr_in	dstaddr;
		socklen_t dstlen = sizeof(dstaddr);
		if(getsockopt(s._f, SOL_IP, SO_ORIGINAL_DST, (struct sockaddr *)&dstaddr, &dstlen) != 0)throw Exception(errno);
		IPEndPoint ep(dstaddr);

		tmp = new tmp123();
		tmp->ep = ep;
		tmp->s1 = s;
		tmp->s2 = s2;
		IPEndPoint ep2(IPAddress(socks_host), socks_port);
		m->BeginConnect(s2, &ep2, SocketManager::Callback(cb_connect, tmp));
	} catch(Exception& ex)
	{
		s.Close();
		s2.Close();
		if(tmp != NULL)delete tmp;
	}
	m->BeginAccept(sock, SocketManager::Callback(cb1, NULL));
}


int iptsocks_main(int argc, char **argv)
{
	signal(SIGPIPE, SIG_IGN);
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart functions if
	                                 interrupted by handler */
	sigaction(SIGHUP, &sa, NULL);
	sa.sa_handler = SIG_DFL;
	//cout<<sigaction(SIGSTOP, &sa, NULL)<<endl;
	//cout<<errno<<endl;
	sigaction(SIGCONT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	sigaction(SIGTTIN, &sa, NULL);
	sigaction(SIGTTOU, &sa, NULL);

	Socket s(AF_INET, SOCK_STREAM, 0);
	IPEndPoint ep(IPAddress("0.0.0.0"), DEFPORT);
	s.Bind(&ep);
	s.Listen(32);
	m->BeginAccept(s, SocketManager::Callback(cb1, NULL));

	DNSServer* srv;
	srv=new DNSServer(IPEndPoint(IPAddress("0.0.0.0"), 5353),[&srv](const EndPoint& ep, const DNSServer::dnsreq& req)
	{
		IPAddress ip("127.0.0.1");
		Buffer tmpb((Byte*)&ip.a, sizeof(ip.a));
		DNSServer::dnsreq resp(req.create_answer());
		for(int i=0;i<(int)resp.queries.size();i++)
		{
			DNSServer::answer a{i,resp.queries[i].type,resp.queries[i].cls,1000000,tmpb};
			resp.answers.push_back(a);
		}
		srv->sendreply(ep, resp);
	});

	m->EventLoop();
	s.Close();
	return 0;
}

