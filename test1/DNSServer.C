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
#include "DNSServer.H"

DNSServer::DNSServer(const EndPoint& listen_addr, const Callback& cb) :
		s(AF_INET, SOCK_DGRAM, 0), cb(cb), buf(4096), q(s)
{
	//ctor
	tmp_ep = EndPoint::CreateNull(listen_addr.AddressFamily);
	s.Bind(listen_addr);
}

DNSServer::~DNSServer()
{
	//dtor
	delete tmp_ep;
}
void DNSServer::sendreply(const EndPoint& ep, const dnsreq& response)
{
	StringBuilder sb;
	create_dns_packet(response, sb);
	boost::shared_ptr<EndPoint> ep1 = boost::shared_ptr<EndPoint>(EndPoint::CreateNull(ep.AddressFamily));
	ep.Clone(*ep1.get());
	q.Append(
	{ ep1, sb.ToBuffer() });
	q.start();
}
void DNSServer::cb1(SocketManager* m, Socket s)
{
	Int br = m->EndRecvFrom(s, *tmp_ep);
	if (br <= 0)
		return;
	dnsreq req;
	parse_dns_packet(buf.SubBuffer(0, br), req);
	cb(*tmp_ep, req);
	start();
}
void DNSServer::start()
{
	SocketManager::GetDefault()->BeginRecv(s, buf, SocketManager::Callback(&DNSServer::cb1, this),
			false);
}

int DNSServer::write_dns_name(const string& name, StringBuilder& sb)
{
	const char* ch = name.data();
	int len = name.length();
	int i = 0;
	int last_i = 0;
	int ret = 0;
	while (i < len)
	{
		if (ch[i] == '.')
		{
			sb.Append((char) (i - last_i));
			sb.Append(ch + last_i, i - last_i);
			ret += 1 + (i - last_i);
			last_i = i + 1;
		}
		i++;
	}
	if (last_i < len)
	{
		sb.Append((char) (len - last_i));
		sb.Append(ch + last_i, len - last_i);
		ret += 1 + (len - last_i);
	}
	return ret;
}
void DNSServer::create_dns_packet(const dnsreq& req, StringBuilder& sb)
{
	dnshdr hdr;
	hdr.transaction_id = req.transaction_id;
	hdr.flags = req.flags;
	hdr.additional_rr = hdr.authority_rr = 0;
	hdr.queries = req.queries.size();
	hdr.answers = req.answers.size();
	sb.Append((char*) &hdr, sizeof(hdr));
	int i, offset = sizeof(hdr);
	UShort queries[req.queries.size()];
	for (i = 0; i < (int) req.queries.size(); i++)
	{
		queries[i] = offset;
		offset += write_dns_name(req.queries[i].q, sb);
		dnshdr_q hdr1
		{ req.queries[i].type, req.queries[i].cls };
		sb.Append((char*) &hdr1, sizeof(hdr1));
		offset += sizeof(hdr1);
	}
	for (i = 0; i < (int) req.answers.size(); i++)
	{
		if (req.answers[i].query_index >= (int) req.queries.size())
			continue;
		dnshdr_a hdr1
		{ queries[req.answers[i].query_index], req.answers[i].type, req.answers[i].cls,
				req.answers[i].ttl, req.answers[i].addr.Length };
		sb.Append((char*) &hdr1, sizeof(hdr1));
		sb.Append(req.answers[i].addr);
	}
}
Int DNSServer::read_dns_name(const Buffer& b, StringBuilder& sb, int i)
{
	Int br;
	Byte len;
	if (i >= b.Length)
		goto err;
	aaaaa: len = b.Data[i];
	if (i + 1 + len > b.Length)
		goto err;
	sb << Buffer(b.Data + i + 1, len);
	i += 1 + len;
	br += 1 + len;
	if (i < b.Length)
	{
		if (b.Data[i] == 0)
			return br + 1;
		else
		{
			sb << '.';
			goto aaaaa;
		}
	}
	else
		return br;
	err: throw Exception("error parsing DNS name");
}
void DNSServer::parse_dns_packet(const Buffer& b, dnsreq& req)
{
	StringBuilder sb;
	const dnshdr* hdr;
	int i, index = sizeof(dnshdr);
	map<UShort, int> queries;
	if (b.Length < (int) sizeof(dnshdr))
		goto err;
	hdr = (const dnshdr*) b.Data;
	req.transaction_id = hdr->transaction_id;
	req.flags = hdr->flags;
	req.queries = vector<query>();
	req.answers = vector<answer>();
	for (i = 0; i < hdr->queries; i++)
	{
		sb.Clear();
		queries.insert(std::pair<UShort, int>(index, i));
		int tmp = read_dns_name(b, sb, index);
		index += tmp;
		const dnshdr_q* hdr1 = (const dnshdr_q*) b.Data + index;
		if (index + (int) sizeof(dnshdr_q) > b.Length)
			goto err;
		query q
		{ sb.ToSTDString(), hdr1->type, hdr1->cls };
		req.queries.push_back(q);
		index += sizeof(dnshdr_q);
	}
	for (i = 0; i < hdr->answers; i++)
	{
		const dnshdr_a* hdr1 = (const dnshdr_a*) b.Data + index;
		if (index + (int) sizeof(dnshdr_a) > b.Length)
			goto err;
		auto it = queries.find(hdr1->offset);
		if (it == queries.end())
			goto err;
		index += sizeof(dnshdr_a);
		int tmp = hdr1->datalen;
		if (index + tmp > b.Length)
			goto err;
		Buffer buf(tmp);
		memcpy(buf.Data, b.Data + index, tmp);
		answer a
		{ (*it).second, hdr1->type, hdr1->cls, hdr1->ttl, buf };
		req.answers.push_back(a);
	}
	return;
	err: throw Exception("invalid or corrupt DNS packet");
}
