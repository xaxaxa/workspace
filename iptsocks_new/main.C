/*
 * main.C
 *
 *  Created on: Mar 17, 2013
 *      Author: xaxaxa
 */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "joinstream.C"
#include "DNSServer.H"
#include <map>
#include <list>
#include <tuple>
#include <cpoll/cpoll.H>
#include <rgc.H>
#include "iptsocks.H"
#include "socks5.H"
#include <stdexcept>
using namespace std;
using namespace CP;
#define SO_ORIGINAL_DST 80

struct p
{
	string socks_host, socks_port, start_addr, end_addr;
	int tcp_port, dns_port;

} iptsocks_params { "127.0.0.1", "9999", "127.1.0.1", "", 6969, 6953 };
RGC::Ref<EndPoint> socks_host_ep;
struct ip_pool
{
	IPAddress begin, end;
	pair<IPAddress, bool> get() {
		if (begin <= end) {
			pair<IPAddress, bool> tmp { begin, true };
			begin = begin + 1;
			return tmp;
		}
		return pair<IPAddress, bool>(IPAddress(), false);
	}
	bool empty() {
		return !(begin < end);
	}
};

struct host_item
{
	string host;
	int refcount;
	uint64_t unused_id;
};
map<IPAddress, host_item> hosts;
map<string, IPAddress> hosts2;
uint64_t max_id = 0;
map<uint64_t, IPAddress> unused_hosts;
ip_pool pool;
//{ IPAddress("127.1.0.1"), IPAddress("127.1.255.255") };

uint64_t get_id() {
	return ++max_id;
}
IPAddress map_host(const string& host) {
	auto it = hosts2.find(host);
	if (it == hosts2.end()) {
		//not found. add.
		auto ip1 = pool.get();
		if (!ip1.second) {
			//pool empty. get an unused address.
			auto it1 = unused_hosts.begin();
			if (it1 == unused_hosts.end()) return IPAddress("0.0.0.0");
			auto& ip = (*it1).second;
			hosts2.erase(hosts[ip].host);
			hosts[ip]=
			{	host,0,0};
			hosts2.insert( { host, ip });
			auto id = get_id();
			unused_hosts.insert( { id, ip });
			unused_hosts.erase(it1);
			hosts[ip].unused_id = id;
			return ip;
		} else {
			auto& ip = ip1.first;
			auto id = get_id();
			host_item tmp { host, 0, id };
			hosts.insert( { ip, tmp });
			hosts2.insert( { host, ip });
			unused_hosts.insert( { id, ip });
			return ip;
		}
	} else {
		return (*it).second;
	}
}
string map_ip(const IPAddress& ip) {
	auto it = hosts.find(ip);
	if (it == hosts.end()) return string();
	return (*it).second.host;
}
void add_unused(const IPAddress& ip) {
	auto& tmp = hosts[ip];
	if (tmp.unused_id != 0) return;
	auto id = get_id();
	unused_hosts.insert( { id, ip });
	tmp.unused_id = id;
	//WARN(2, inet_ntoa(ip.a) << " freed");
}
void rm_unused(const IPAddress& ip) {
	auto& tmp = hosts[ip];
	if (tmp.unused_id == 0) return;
	unused_hosts.erase(tmp.unused_id);
	tmp.unused_id = 0;
	//WARN(2, inet_ntoa(ip.a) << " acquired");
}
void increment_host(const IPAddress& ip) {
	auto it = hosts.find(ip);
	if (it == hosts.end()) return;
	(*it).second.refcount++;
	//WARN(2, inet_ntoa(ip.a) << " incremented to " << (*it).second.refcount);
	rm_unused(ip);
}
void decrement_host(const IPAddress& ip) {
	auto it = hosts.find(ip);
	if (it == hosts.end()) return;
	(*it).second.refcount--;
	//WARN(2, inet_ntoa(ip.a) << " decremented to " << (*it).second.refcount);
	if ((*it).second.refcount <= 0) {
		(*it).second.refcount = 0;
		add_unused(ip);
	}
}

struct iptsocks_connection: public virtual RGC::Object
{
	Poll& p;
	Socket s1, s2;
	IPEndPoint ep;
	JoinStream j;
	iptsocks_connection(Poll& p, HANDLE h1) :
			p(p), s1(h1, AF_INET, SOCK_STREAM, 0), s2(AF_INET, SOCK_STREAM, 0) {
		try {
			j.from1to2= {&iptsocks_connection::from1to2,this};
			j.from2to1= {&iptsocks_connection::from2to1,this};
			j.s1 = &s1;
			j.s2 = &s2;
			p.add(s1);
			p.add(s2);
			sockaddr_in dstaddr;
			socklen_t dstlen = sizeof(dstaddr);
			if (getsockopt(h1, SOL_IP, SO_ORIGINAL_DST, (struct sockaddr *) &dstaddr, &dstlen) != 0) throw runtime_error(
					strerror(errno));
			ep.setSockAddr((sockaddr*) &dstaddr);
			//IPEndPoint ep2(IPAddress(iptsocks_params.socks_host), iptsocks_params.socks_port);
			s2.connect(*socks_host_ep, Callback(&iptsocks_connection::cb_connect, this));
			retain();
		} catch (exception& ex) {

		}
	}
	void from1to2(JoinStream& j, uint8_t* data, int& len) {
		if (len <= 0) {
			//WARN(1,"from1to2(): release(): before-ref-count: " << refCount);
			delete this; return;
			struct {
				void operator()() {}
			}tmpcb;
			//s2.shutdown(SHUT_WR,&tmpcb);
			s2.close(&tmpcb);
			release();
		}
	}
	void from2to1(JoinStream& j, uint8_t* data, int& len) {
		if (len <= 0) {
			//WARN(1,"from2to1(): release(): before-ref-count: " << refCount);
			delete this; return;
			struct {
				void operator()() {}
			}tmpcb;
			s1.close(&tmpcb);
			//s1.shutdown(SHUT_WR,&tmpcb);
			release();
		}
	}
	void socks_cb(Stream& s, exception* ex) {
		if (ex != NULL) {
			WARN(2, "exception: " << ex->what());
			delete this;
			return;
		}
		retain();
		j.read2();
		increment_host(ep.address);
	}
	void socks_cb1(Stream& s, exception* ex) {
		if (ex != NULL) {
			WARN(2, "exception: " << ex->what());
			delete this;
			return;
		}
		//retain(); release();
		j.read1();
	}
	void cb_connect(int r) {
		if (r < 0) {
			delete this;
			return;
		}
		string h = map_ip(ep.address);
		if (h.length() == 0) SOCKS5::socks_connect(s2, ep,
				SOCKS5::Callback(&iptsocks_connection::socks_cb, this),
				SOCKS5::Callback(&iptsocks_connection::socks_cb1, this));
		else SOCKS5::socks_connect(s2, h.c_str(), ep.port,
				SOCKS5::Callback(&iptsocks_connection::socks_cb, this),
				SOCKS5::Callback(&iptsocks_connection::socks_cb1, this));

	}
};
void parseArgs(int argc, char** argv, const function<void(char*, const function<char*()>&)>& cb) {
	int i = 1;
	function<char*()> func = [&]()->char*
	{
		if(i+1>=argc)return NULL;
		return argv[(++i)];
	};
	for (; i < argc; i++) {
		if (argv[i][0] == '\x00') continue;
		if (argv[i][0] == '-') {
			cb(argv[i] + 1, func);
		} else {
			cb(NULL, [argv,i]()
			{	return argv[i];});
		}
	}
}
int main(int argc, char **argv) {
	int i = 0;
	parseArgs(argc, argv,
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
					if((tmpv=getvalue())==NULL)throw logic_error("-d(dns_port) requires a value");
					iptsocks_params.dns_port=atoi(tmpv);
				}
				else if(strcmp(name,"p")==0)
				{
					if((tmpv=getvalue())==NULL)throw logic_error("-p(tcp_port) requires a value");
					iptsocks_params.tcp_port=atoi(tmpv);
				}
				else if(strcmp(name,"s")==0)
				{
					if((tmpv=getvalue())==NULL)throw logic_error("-s(mapping_start_address) requires a value");
					iptsocks_params.start_addr=tmpv;
				}
				else if(strcmp(name,"e")==0)
				{
					if((tmpv=getvalue())==NULL)throw logic_error("-e(mapping_end_address) requires a value");
					iptsocks_params.end_addr=tmpv;
				}
				else if(strcmp(name,"h")==0)
				{
					cout << "usage: " << argv[0] << " [-p tcp_port] [-d dns_port] [-s mapping_start_address] [-e mapping_end_address] [socks_host] [socks_port]" << endl;
					exit(0);
				}
			});
	if (iptsocks_params.end_addr.length() == 0) {
		iptsocks_params.end_addr = (IPAddress(iptsocks_params.start_addr.c_str()) + 65534).toStr();
	}
	WARN(1,
			"params: socks_host=" << iptsocks_params.socks_host << "; socks_port=" << iptsocks_params.socks_port << "; tcp_port=" << iptsocks_params.tcp_port << "; dns_port=" << iptsocks_params.dns_port << "; mapping_start_address=" << iptsocks_params.start_addr << "; mapping_end_address=" << iptsocks_params.end_addr << ";");
	pool= {IPAddress(iptsocks_params.start_addr.c_str()),IPAddress(iptsocks_params.end_addr.c_str())};

	Poll p;

	auto vect = EndPoint::lookupHost(iptsocks_params.socks_host.c_str(),
			iptsocks_params.socks_port.c_str(), 0, SOCK_STREAM);
	if (vect.size() < 0) throw runtime_error("could not resolve host name of SOCKS server");
	socks_host_ep = vect[0];
	WARN(1, "SOCKS server endpoint: " << socks_host_ep->toStr());
	Socket s(AF_INET, SOCK_STREAM, 0);
	IPEndPoint ep(IPAddress("0.0.0.0"), iptsocks_params.tcp_port);
	s.bind(ep);
	s.listen();
	p.add(s);
	struct
	{
		Poll& p;
		void operator()(HANDLE h) {
			iptsocks_connection* conn = new iptsocks_connection(p, h);
			conn->release();
		}
	} cb1 { p };
	s.repeatAcceptHandle(&cb1);

	struct
	{
		void operator()(DNSServer& srv, const EndPoint& ep, const DNSServer::dnsreq& req) {
			WARN(6, "RECEIVED DNS PACKET");

			/*auto tmp=pool.get();
			 if(tmp.second)ip=tmp.first;
			 else ip=IPAddress("127.0.0.1");*/

			DNSServer::dnsreq resp(req.create_answer());
			IPAddress ip1[resp.queries.size()];
			for (int i = 0; i < (int) resp.queries.size(); i++) {
				ip1[i] = map_host(resp.queries[i].q);
				DNSServer::answer a { i, resp.queries[i].type, resp.queries[i].cls, 100000000, string {
						(const char*) &ip1[i].a, sizeof(ip1[i].a) } };
				resp.answers.push_back(a);
			}
			srv.sendreply(ep, resp);
		}
		;
	} answer_request;
	DNSServer* srv;
	srv = new DNSServer(p, IPEndPoint(IPAddress("127.0.0.1"), iptsocks_params.dns_port),
			&answer_request);
	srv->start();
	DNSServer* srv1;
	srv1 = new DNSServer(p, IPEndPoint(IPAddress("0.0.0.0"), iptsocks_params.dns_port),
			&answer_request);
	srv1->start();
	WARN(6, "started.");
	p.loop();
	return 0;
}
