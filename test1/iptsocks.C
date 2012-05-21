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
#include "defines.H"
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
#include <map>
#include <list>
#include <tuple>

//#define WARNLEVEL 10
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

struct p
{
	const char *socks_host, *socks_port, *start_addr, *end_addr;
	int tcp_port, dns_port;

} iptsocks_params
{ "127.0.0.1", "9999", "127.1.0.1", NULL, 6969, 6953 };
Property<EndPoint> socks_host_ep;
struct ip_pool
{
	IPAddress begin, end;
	pair<IPAddress, bool> get()
	{
		if (begin <= end)
		{
			pair<IPAddress, bool> tmp
			{ begin, true };
			begin = begin + 1;
			return tmp;
		}
		return pair<IPAddress, bool>(IPAddress(), false);
	}
	bool empty()
	{
		return !(begin < end);
	}
};

struct host_item
{
	string host;
	int refcount;
	ULong unused_id;
};
map<IPAddress, host_item> hosts;
map<string, IPAddress> hosts2;
ULong max_id = 0;
map<ULong, IPAddress> unused_hosts;
ip_pool pool;
//{ IPAddress("127.1.0.1"), IPAddress("127.1.255.255") };

ULong get_id()
{
	return ++max_id;
}
IPAddress map_host(const string& host)
{
	auto it = hosts2.find(host);
	if (it == hosts2.end())
	{
		//not found. add.
		auto ip1 = pool.get();
		if (!ip1.second)
		{
			//pool empty. get an unused address.
			auto it1 = unused_hosts.begin();
			if (it1 == unused_hosts.end())
				return IPAddress("0.0.0.0");
			auto ip = (*it1).second;
			hosts2.erase(hosts[ip].host);
			hosts[ip]=
			{	host,0,0};
			hosts2.insert(
			{ host, ip });
			auto id = get_id();
			unused_hosts.insert(
			{ id, ip });
			unused_hosts.erase(it1);
			hosts[ip].unused_id = id;
			return ip;
		}
		else
		{
			auto& ip = ip1.first;
			auto id = get_id();
			host_item tmp
			{ host, 0, id };
			hosts.insert(
			{ ip, tmp });
			hosts2.insert(
			{ host, ip });
			unused_hosts.insert(
			{ id, ip });
			return ip;
		}
	}
	else
	{
		return (*it).second;
	}
}
string map_ip(const IPAddress& ip)
{
	auto it = hosts.find(ip);
	if (it == hosts.end())
		return string();
	return (*it).second.host;
}
void add_unused(const IPAddress& ip)
{
	auto& tmp = hosts[ip];
	if (tmp.unused_id != 0)
		return;
	auto id = get_id();
	unused_hosts.insert(
	{ id, ip });
	tmp.unused_id = id;
	//WARN(2, inet_ntoa(ip.a) << " freed");
}
void rm_unused(const IPAddress& ip)
{
	auto& tmp = hosts[ip];
	if (tmp.unused_id == 0)
		return;
	unused_hosts.erase(tmp.unused_id);
	tmp.unused_id = 0;
	//WARN(2, inet_ntoa(ip.a) << " acquired");
}
void increment_host(const IPAddress& ip)
{
	auto it = hosts.find(ip);
	if (it == hosts.end())
		return;
	(*it).second.refcount++;
	//WARN(2, inet_ntoa(ip.a) << " incremented to " << (*it).second.refcount);
	rm_unused(ip);
}
void decrement_host(const IPAddress& ip)
{
	auto it = hosts.find(ip);
	if (it == hosts.end())
		return;
	(*it).second.refcount--;
	//WARN(2, inet_ntoa(ip.a) << " decremented to " << (*it).second.refcount);
	if ((*it).second.refcount <= 0)
	{
		(*it).second.refcount = 0;
		add_unused(ip);
	}
}

static void socks_cb1(void* obj, Stream* s, void* v)
{
	tmp123* tmp = (tmp123*) obj;
	dbgprint("################socks sent_callback##################");
	tmp->j->dowrite2 = true;
	tmp->j->begin2w();
	//delete tmp;
}
static void socks_cb(void* obj, Stream* s, void* v)
{
	tmp123* tmp = (tmp123*) obj;
	try
	{
		SOCKS5::socks_endconnect(v);
	} catch (Exception& ex)
	{
		tmp->j->Close();
		delete tmp;
		return;
	}
	WARN(5, "################socks connected##################");
	tmp->j->Begin();
	increment_host(tmp->ep.Address);
	delete tmp;
}

FUNCTION_DECLWRAPPER(cb_connect, void, SocketManager* m, Socket sock)
{
	tmp123* tmp = (tmp123*) obj;
	tmp->j = NULL;
	try
	{
		m->EndConnect(sock);
	} catch (Exception& ex)
	{
		tmp->s1.Close();
		tmp->s2.Close();
		delete tmp;
		return;
	}
	try
	{
		objref<SocketStream> str1(tmp->s1);
		objref<SocketStream> str2(tmp->s2);
		JoinStream *j = new JoinStream(str1(), str2());
//j->Begin();
//delete tmp;
		tmp->j = j;
		j->begin1r();
		//SOCKS5::socks_connect(j->s2, &(tmp->ep), SOCKS5::Callback(socks_cb, tmp),
		//		SOCKS5::Callback(socks_cb1, tmp));
		string h = map_ip(tmp->ep.Address);
		if (h == string())
			SOCKS5::socks_connect(j->s2(), &(tmp->ep), SOCKS5::Callback(socks_cb, tmp),
					SOCKS5::Callback(socks_cb1, tmp));
		else
			SOCKS5::socks_connect(j->s2(), h.c_str(), tmp->ep.Port, SOCKS5::Callback(socks_cb, tmp),
					SOCKS5::Callback(socks_cb1, tmp));
		auto ip = tmp->ep.Address;
		j->onclose = [ip,j](JoinStream* th)
		{
			//WARN(2,inet_ntoa(ip.a)<<" closed");
				decrement_host(ip);
				/*j->s1->Close();
				 j->s2->Close();
				 delete j->s1;
				 delete j->s2;*/
				//WARN(1,j << " deleted");
				j->RefCount_dec();
			};
	} catch (Exception& ex)
	{
		if (tmp->j != NULL)
			tmp->j->Close();
		delete tmp;
	}
}

FUNCTION_DECLWRAPPER(cb1, void, SocketManager* m, Socket sock)
{
	Socket s = m->EndAccept(sock);
	//dbgprint("accepted: " << s._s);
	//new client(s);
	Socket s2(AF_INET, SOCK_STREAM, 0);
//dbgprint("asdf2");
	tmp123* tmp = NULL;
	try
	{
		sockaddr_in dstaddr;
		socklen_t dstlen = sizeof(dstaddr);
		if (getsockopt(s._f, SOL_IP, SO_ORIGINAL_DST, (struct sockaddr *) &dstaddr, &dstlen) != 0)
			throw Exception(errno);
		IPEndPoint ep(dstaddr);

		tmp = new tmp123();
		tmp->ep = ep;
		tmp->s1 = s;
		tmp->s2 = s2;
		//IPEndPoint ep2(IPAddress(iptsocks_params.socks_host), iptsocks_params.socks_port);
		m->BeginConnect(s2, socks_host_ep(), SocketManager::Callback(cb_connect, tmp));
	} catch (Exception& ex)
	{
		s.Close();
		s2.Close();
		if (tmp != NULL)
			delete tmp;
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

	/*if (argc > 1)
	 iptsock_params.socks_host = argv[1];
	 if (argc > 2)
	 iptsock_params.socks_port = atoi(argv[2]);*/
	int i = 0;
	Util.ParseArgs(argc, argv,
			[&](char* name, const std::function<char*()>& getvalue)
			{
				char* tmpv;
				if(name==NULL)
				{
					switch(i)
					{
						case 0:
						iptsocks_params.socks_host=getvalue();
						break;
						case 1:
						iptsocks_params.socks_port=getvalue();
						break;
					}
					i++;
				}
				else if(strcmp(name,"d")==0)
				{
					if((tmpv=getvalue())==NULL)throw Exception("-d(dns_port) requires a value");
					iptsocks_params.dns_port=atoi(tmpv);
				}
				else if(strcmp(name,"p")==0)
				{
					if((tmpv=getvalue())==NULL)throw Exception("-p(tcp_port) requires a value");
					iptsocks_params.tcp_port=atoi(tmpv);
				}
				else if(strcmp(name,"s")==0)
				{
					if((tmpv=getvalue())==NULL)throw Exception("-s(mapping_start_address) requires a value");
					iptsocks_params.start_addr=tmpv;
				}
				else if(strcmp(name,"e")==0)
				{
					if((tmpv=getvalue())==NULL)throw Exception("-e(mapping_end_address) requires a value");
					iptsocks_params.end_addr=tmpv;
				}
				else if(strcmp(name,"h")==0)
				{
					cout << "usage: " << argv[0] << " [-p tcp_port] [-d dns_port] [-s mapping_start_address] [-e mapping_end_address] [socks_host] [socks_port]" << endl;
					exit(0);
				}
			});
	if (iptsocks_params.end_addr == NULL)
		iptsocks_params.end_addr = (IPAddress(iptsocks_params.start_addr) + 65534).ToStr().c_str();
	WARN(1,
			"params: socks_host=" << iptsocks_params.socks_host << "; socks_port=" << iptsocks_params.socks_port << "; tcp_port=" << iptsocks_params.tcp_port << "; dns_port=" << iptsocks_params.dns_port << "; mapping_start_address=" << iptsocks_params.start_addr << "; mapping_end_address=" << iptsocks_params.end_addr << ";");
	pool={IPAddress(iptsocks_params.start_addr),IPAddress(iptsocks_params.end_addr)};

	auto vect = EndPoint::LookupHost(iptsocks_params.socks_host, iptsocks_params.socks_port, 0,
			SOCK_STREAM);
	if (vect.size() < 0)
		throw Exception("could not resolve host name of SOCKS server");
	socks_host_ep = vect[0];
	WARN(1, "SOCKS server endpoint: " << socks_host_ep->ToStr());
	Socket s(AF_INET, SOCK_STREAM, 0);
	IPEndPoint ep(IPAddress("0.0.0.0"), iptsocks_params.tcp_port);
	s.Bind(&ep);
	s.Listen(32);
	m->BeginAccept(s, SocketManager::Callback(cb1, NULL));

	auto answer_request = [&pool](DNSServer& srv, const EndPoint& ep, const DNSServer::dnsreq& req)
	{
		WARN(6,"RECEIVED DNS PACKET");
		IPAddress ip;
		/*auto tmp=pool.get();
		 if(tmp.second)ip=tmp.first;
		 else ip=IPAddress("127.0.0.1");*/

		DNSServer::dnsreq resp(req.create_answer());
		for(int i=0;i<(int)resp.queries.size();i++)
		{
			auto ip=map_host(resp.queries[i].q);
			Buffer tmpb((Byte*)&ip.a, sizeof(ip.a));
			DNSServer::answer a
			{	i,resp.queries[i].type,resp.queries[i].cls,100000000,tmpb};
			resp.answers.push_back(a);
		}
		srv.sendreply(ep, resp);
	};
	DNSServer* srv;
	srv = new DNSServer(IPEndPoint(IPAddress("127.0.0.1"), iptsocks_params.dns_port),
			answer_request);
	srv->start();
	srv = new DNSServer(IPEndPoint(IPAddress("0.0.0.0"), iptsocks_params.dns_port), answer_request);
	srv->start();
	WARN(6, "started.");
	m->EventLoop();
	s.Close();
	return 0;
}

