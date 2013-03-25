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
#include <cpoll/cpoll.H>
#include <map>
#include "PacketQueue.H"
using namespace std;
using namespace CP;
using namespace xaxaxa;
class DNSServer
{
public:
	static const uint16_t dns_offset_mask=3<<14;
	struct dnshdr
	{
		uint16_t transaction_id, flags, queries, answers, authority_rr, additional_rr;
	} __attribute__((packed));
	struct dnshdr_q
	{
		uint16_t type, cls;
	} __attribute__((packed));
	struct dnshdr_a
	{
		uint16_t offset, type, cls;
		uint32_t ttl;
		uint16_t datalen;
	} __attribute__((packed));
	struct query
	{
		string q;
		uint16_t type, cls;
	};
	struct answer
	{
		int32_t query_index;
		uint16_t type, cls;
		uint32_t ttl;
		string addr;
	};
	struct dnsreq
	{
		vector<query> queries;
		vector<answer> answers;
		uint16_t transaction_id, flags;
		dnsreq create_answer() const
		{
			return
			{	queries,vector<answer>(),transaction_id,0x8180};
		}
	};
	static int write_dns_name(const string& name, StreamWriter& sb);
	static void create_dns_packet(const dnsreq& req, StreamWriter& sb);
	static int32_t read_dns_name(uint8_t* buf, int len, StreamWriter& sb, int i = 0);
	static void parse_dns_packet(uint8_t* buf, int len, dnsreq& req);
	void cb1(int r);
	void operator=(const DNSServer& other)=delete;
	DNSServer(const DNSServer& other)=delete;
	typedef Delegate<void(DNSServer& srv, const EndPoint& ep, const dnsreq&)> Callback;

	Poll& p;
	uint8_t buf[4096];
	Socket s;
	//boost::shared_ptr<SocketStream> ss;
	Callback cb;
	//Buffer buf;
	EndPoint* tmp_ep;
	UDPQueue q;
	DNSServer(Poll& p, const EndPoint& listen_addr, const Callback& cb);
	virtual ~DNSServer();
	void start();
	void sendreply(const EndPoint& ep, const dnsreq& response);
};

#endif // DNSSERVER_H
