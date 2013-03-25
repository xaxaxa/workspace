/*
 * socks5.C
 *
 *  Created on: Mar 23, 2013
 *      Author: xaxaxa
 */
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

#include "socks5.H"
#include <stdexcept>
#include <concat.H>
using namespace std;
namespace SOCKS5
{
	struct socks_auth_response
	{
		uint8_t version, auth;
	}__attribute__((packed));
	struct socks_response
	{
		uint8_t version, status, reserved, addrtype, addrlen;
	}__attribute__((packed));
	struct socksConnection
	{
		//Buffer b;
		void* buf;
		int buflen;
		//StringBuilder* sb;
		//int addrlen;
		Callback cb;
		Callback sent_cb;
		//int br;
		exception* ex;
		Stream* s;
		//void* state;

		void socks_connect(EndPoint& ep) {
			int port;
			try {
				MemoryStream ms(32);
				StreamWriter sb(ms);
				char c[] = "\x05\x01\x00";
				sb.write(c, 3);
				switch (ep.addressFamily) {
					case AF_INET:
					{
						sb.write('\x01');
						port = dynamic_cast<IPEndPoint&>(ep).port;
						//tmp1->addrlen = 4;
						sockaddr_in addr;
						ep.getSockAddr((sockaddr*) &addr);
						sb.write((char*) &(addr.sin_addr.s_addr), 4);
						break;
					}
					case AF_INET6:
					{
						sb.write('\x04');
						port = dynamic_cast<IPv6EndPoint&>(ep).port;
						//tmp1->addrlen = 16;
						sockaddr_in6 addr;
						ep.getSockAddr((sockaddr*) &addr);
						sb.write((char*) &(addr.sin6_addr.__in6_u.__u6_addr8), 16);
						break;
					}
					default:
						throw logic_error("SOCKS does not support the requested address family");
				}
				port = htons(port);
				sb.write(&port, 2);
				sb.flush();
				//STRING tmp = sb->ToString();
				this->buf = ms.buffer;
				this->buflen = ms.len;
				ms.keepBuffer();
				//s->BeginWrite(sb.ToBuffer(), Stream::Callback(cb1, tmp1));
				s->writeAll("\x05\x01\x00", 3, CP::Callback(&socksConnection::auth_cb, this));
			} catch (exception& ex) {
				this->cb(*s, &ex);
				//delete tmp1->sb;
				delete this;
			}
		}
		void socks_connect(const char* host, uint16_t port) {
			this->cb = cb;
			this->sent_cb = sent_cb;
			try {
				MemoryStream ms(32);
				StreamWriter sb(ms);
				const char* c = "\x05\x01\x00";
				sb.write(c, 3);
				sb.write('\x03');
				int addrlen = strlen(host);
				//tmp1->addrlen = 0;
				sb.write((char) addrlen);
				sb.write(host);
				port = htons(port);
				sb.write((char*) &port, 2);
				sb.flush();
				this->buf = ms.buffer;
				this->buflen = ms.len;
				ms.keepBuffer();
				s->writeAll("\x05\x01\x00", 3, CP::Callback(&socksConnection::auth_cb, this));
			} catch (exception& ex) {
				this->cb(*s, &ex);
				//delete tmp1->sb;
				delete this;
			}
		}
		char tmpbuf[5];
		void auth_cb(int r) {
			try {
				s->readAll(tmpbuf, 2, CP::Callback(&socksConnection::auth_received_cb, this));
			} catch (exception& ex) {
				cb(*s, &ex);
				delete this;
			}
		}
		void auth_received_cb(int r) {
			try {
				s->writeAll(buf, buflen, CP::Callback(&socksConnection::cb1, this));
			} catch (exception& ex) {
				cb(*s, &ex);
				delete this;
			}
		}
		void cb2_1(int i) {
			try {
				if (i < 5) throw runtime_error(
						CONCAT("SOCKS error occurred: could not receive 5 bytes from socks server; got " << i) );
				socks_response* resp = (socks_response*) tmpbuf;
				int addrlen;
				switch (resp->addrtype) {
					case 1: //ipv4
						addrlen = 4;
						break;
					case 3: //hostname
						addrlen = resp->addrlen + 1;
						break;
					case 4: //ipv6
						addrlen = 16;
						break;
					default:
						throw runtime_error("SOCKS error occurred: unknown response->addrtype");
				}
				//addrlen = addrlen - 1 + 2;
				buflen = addrlen - 1 + 2;
				buf = malloc(buflen);
				s->readAll(buf, buflen, CP::Callback(&socksConnection::cb2, this));
			} catch (exception& ex) {
				cb(*s, &ex);
				delete this;
			}
		}
		void cb1(int r) {
			try {
				free(buf);
				buf = NULL;
				if (sent_cb != nullptr) sent_cb(*s, nullptr);
				/*if(tmp1->addrlen<=0)
				 {*/

				//tmp1->br = 0;
				//dbgprint("========SOCKS request sent======");
				s->readAll(tmpbuf, 5, CP::Callback(&socksConnection::cb2_1, this));
				/*
				 return;
				 }
				 tmp1->b = Buffer(8 + tmp1->addrlen);
				 tmp1->br = 0;
				 dbgprint("========SOCKS request sent======");
				 s->BeginRead(tmp1->b, Stream::Callback(cb2, tmp1));*/
			} catch (exception& ex) {
				cb(*s, &ex);
				delete this;
			}
		}
		void cb2(int i) {
			try {
				free(buf);
				buf = NULL;
				if (i <= 0) throw runtime_error("SOCKS error occurred: server closed connection");
				//dbgprint("========SOCKS response read====== " << i << " bytes received");
				//tmp1->br += i;
				try {
					cb(*s, nullptr);
				} catch (exception& ex1) {
				}
				delete this;
			} catch (exception& ex) {
				cb(*s, &ex);
				delete this;
			}
		}
		void socks_endconnect() {
			if (ex != NULL) throw(exception&) *(ex);
		}
	};
	void socks_connect(Stream& s, const char* host, uint16_t port, Callback cb, Callback sent_cb) {
		socksConnection* sc = new socksConnection();
		sc->cb = cb;
		sc->sent_cb = sent_cb;
		sc->s = &s;
		sc->socks_connect(host, port);
	}
	void socks_connect(Stream& s, EndPoint& ep, Callback cb, Callback sent_cb) {
		socksConnection* sc = new socksConnection();
		sc->cb = cb;
		sc->sent_cb = sent_cb;
		sc->s = &s;
		sc->socks_connect(ep);
	}
}
