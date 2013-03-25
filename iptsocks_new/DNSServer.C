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
#include <stdlib.h>
#include <stdexcept>

DNSServer::DNSServer(Poll& p, const EndPoint& listen_addr, const Callback& cb) :
		p(p), s(listen_addr.addressFamily, SOCK_DGRAM, IPPROTO_UDP), cb(cb), q(s) {
	//ctor
	tmp_ep = EndPoint::create(listen_addr.addressFamily);
	s.bind(listen_addr);
	p.add(s);
	struct
	{
		//"this" is invalid at the time that this function is called because it is on the stack
		//of a function that had already returned, but it doesn't matter because "this" is not used.
		void operator()(uint8_t* buf) {
			free(buf);
		}
	} tmpcb;
	q.sent = &tmpcb;
}

DNSServer::~DNSServer() {
	//dtor
	//delete tmp_ep;
}
void DNSServer::sendreply(const EndPoint& ep, const dnsreq& response) {
	CP::MemoryStream ms;
	CP::StreamWriter sb(ms);
	create_dns_packet(response, sb);
	sb.flush();
	q.append(ms.buffer, ms.len, ep);
	q.start();
	ms.keepBuffer();
}
void DNSServer::cb1(int br) {
	if (br <= 0) return;
	//WARN(6, "got dns packet");
	try {
		dnsreq req;
		parse_dns_packet(buf, br, req);
		cb(*this, *tmp_ep, req);
	} catch (runtime_error& ex) {
	}
	start();
}
void DNSServer::start() {
	s.recvFrom(buf, sizeof(buf), 0, *tmp_ep, CP::Callback(&DNSServer::cb1, this));
	//WARN(8, "receiving...");
}

int DNSServer::write_dns_name(const string& name, StreamWriter& sb) {
	const char* ch = name.data();
	int len = name.length();
	int i = 0;
	int last_i = 0;
	int ret = 0;
	while (i < len) {
		if (ch[i] == '.') {
			sb.write((char) (i - last_i));
			sb.write(ch + last_i, i - last_i);
			ret += 1 + (i - last_i);
			last_i = i + 1;
		}
		i++;
	}
	if (last_i < len) {
		sb.write((char) (len - last_i));
		sb.write(ch + last_i, len - last_i);
		ret += 1 + (len - last_i);
	}
	sb.write((char) 0);
	return ret;
}
void DNSServer::create_dns_packet(const dnsreq& req, StreamWriter& sb) {
	dnshdr hdr;
	hdr.transaction_id = req.transaction_id;
	hdr.flags = htons(req.flags);
	hdr.additional_rr = hdr.authority_rr = htons(0);
	hdr.queries = htons(req.queries.size());
	hdr.answers = htons(req.answers.size());
	sb.write((char*) &hdr, sizeof(hdr));
	int i, offset = sizeof(hdr);
	uint16_t queries[req.queries.size()];
	for (i = 0; i < (int) req.queries.size(); i++) {
		queries[i] = offset;
		offset += write_dns_name(req.queries[i].q, sb);
		dnshdr_q hdr1 { htons(req.queries[i].type), htons(req.queries[i].cls) };
		sb.write((char*) &hdr1, sizeof(hdr1));
		offset += sizeof(hdr1);
	}
	for (i = 0; i < (int) req.answers.size(); i++) {
		if (req.answers[i].query_index >= (int) req.queries.size()) continue;
		dnshdr_a hdr1 { (uint16_t) (htons(queries[req.answers[i].query_index] | dns_offset_mask)),
				htons(req.answers[i].type), htons(req.answers[i].cls), htonl(req.answers[i].ttl), htons(
						req.answers[i].addr.length()) };
		sb.write((char*) &hdr1, sizeof(hdr1));
		sb.write(req.answers[i].addr.data(), req.answers[i].addr.length());
	}
}
int DNSServer::read_dns_name(uint8_t* buf, int buflen, StreamWriter& sb, int i1) {
	int i = i1;
	int br(0);
	uint8_t len;
	if (i >= buflen) goto err;
	aaaaa: len = buf[i];
	if (i + 1 + len > buflen) goto err;
	sb.write(buf + i + 1, len);
	i += 1 + len;
	br += 1 + len;
	if (i < buflen) {
		if (buf[i] == 0) return br + 1;
		else {
			sb.write('.');
			goto aaaaa;
		}
	} else return br;
	err: throw runtime_error("error parsing DNS name");
}
void DNSServer::parse_dns_packet(uint8_t* buf, int buflen, dnsreq& req) {
	MemoryStream ms;
	StreamWriter sb(ms);
	uint16_t qs, as;
	const dnshdr* hdr;
	int i, index = sizeof(dnshdr);
	map<uint16_t, int> queries;
	if (buflen < (int) sizeof(dnshdr)) goto err;
	hdr = (const dnshdr*) buf;
	req.transaction_id = hdr->transaction_id;
	req.flags = ntohs(hdr->flags);
	req.queries = vector<query>();
	req.answers = vector<answer>();
	qs = ntohs(hdr->queries);
	as = ntohs(hdr->answers);
	for (i = 0; i < qs; i++) {
		ms.clear();
		queries.insert(std::pair<uint16_t, int>(index, i));
		int tmp = read_dns_name(buf, buflen, sb, index);
		sb.flush();
		index += tmp;
		const dnshdr_q* hdr1 = (const dnshdr_q*) (buf + index);
		if (index + (int) sizeof(dnshdr_q) > buflen) goto err;
		query q { string{ (const char*)ms.buffer, (size_t)ms.len }, ntohs(hdr1->type), ntohs(hdr1->cls) };
		req.queries.push_back(q);
		index += sizeof(dnshdr_q);
	}
	for (i = 0; i < as; i++) {
		const dnshdr_a* hdr1 = (const dnshdr_a*) (buf + index);
		if (index + (int) sizeof(dnshdr_a) > buflen) goto err;
		auto it = queries.find(ntohs(hdr1->offset) & ~dns_offset_mask);
		if (it == queries.end()) goto err;
		index += sizeof(dnshdr_a);
		uint16_t tmp = ntohs(hdr1->datalen);
		if (index + (int) tmp > buflen) goto err;
		//Buffer buf(tmp);
		//memcpy(buf.Data, b.Data + index, tmp);
		answer a { (*it).second, ntohs(hdr1->type), ntohs(hdr1->cls), ntohl(hdr1->ttl), {
				((const char*) buf) + index, tmp } };
		req.answers.push_back(a);
	}
	return;
	err: throw runtime_error("invalid or corrupt DNS packet");
}
