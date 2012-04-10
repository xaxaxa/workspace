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
#ifndef DNSSERVER_H
#define DNSSERVER_H
#include <functional>
#include <string>
#include <cplib/asyncsock.hpp>
#include <cplib/cplib.hpp>
#include <map>
#include "PacketQueue.H"
using namespace xaxaxa;
using namespace Sockets;
using namespace std;
using namespace boost;
class DNSServer
{
public:
	struct dnshdr
	{
		UShort transaction_id, flags, queries, answers, authority_rr, additional_rr;
	};
	struct dnshdr_q
	{
		UShort type, cls;
	};
	struct dnshdr_a
	{
		UShort offset, type, cls;
		UInt ttl, datalen;
	};
	struct query
	{
		string q;
		UShort type, cls;
	};
	struct answer
	{
		Int query_index;
		UShort type, cls;
		UInt ttl;
		Buffer addr;
	};
	struct dnsreq
	{
		vector<query> queries;
		vector<answer> answers;
		UShort transaction_id, flags;
		dnsreq create_answer() const
		{
			return
			{	queries,vector<answer>(),transaction_id,0x8180};
		}
	};
	static int write_dns_name(const string& name, StringBuilder& sb);
	static void create_dns_packet(const dnsreq& req, StringBuilder& sb);
	static Int read_dns_name(const Buffer& b, StringBuilder& sb, int i = 0);
	static void parse_dns_packet(const Buffer& b, dnsreq& req);
	void cb1(SocketManager* m, Socket s);
	void operator=(const DNSServer& other)=delete;
	DNSServer(const DNSServer& other)=delete;
	typedef std::function<void(const EndPoint& ep, const dnsreq&)> Callback;
	Socket s;
	//boost::shared_ptr<SocketStream> ss;
	Callback cb;
	Buffer buf;
	EndPoint* tmp_ep;
	UDPQueue q;
	DNSServer(const EndPoint& listen_addr, const Callback& cb);
	virtual ~DNSServer();
	void start();
	void sendreply(const dnsreq& response);
};

#endif // DNSSERVER_H
