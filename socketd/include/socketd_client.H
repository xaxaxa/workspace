/*
 * socketd_client.H
 *
 *  Created on: Mar 8, 2013
 *      Author: xaxaxa
 */

#ifndef SOCKETD_CLIENT_H_
#define SOCKETD_CLIENT_H_
#include <socketd.H>
#include <cpoll/cpoll.H>
#include <unistd.h>
#include <cpoll/sendfd.H>
#include <functional>
namespace socketd
{
	using namespace std;
	using namespace CP;
	class SocketProxy: public CP::Socket
	{
	public:
		uint8_t* buf;
		int bufPos, bufLen;
		SocketProxy(int fd, int d, int t, int p, int buflen) :
				CP::Socket(fd, d, t, p), bufPos(0), bufLen(buflen) {
			buf = new uint8_t[buflen];
		}
		int32_t tryFixRead(void* buf, int32_t len) {
			if (this->buf == NULL || bufPos >= bufLen) return -1;
			int32_t l = len > (bufLen - bufPos) ? (bufLen - bufPos) : len;
			if (l <= 0) return 0;
			memcpy(buf, this->buf + bufPos, l);
			bufPos += l;
			if (bufPos >= bufLen) {
				delete[] this->buf;
				this->buf = NULL;
			}
			return l;
		}
		int32_t read(void* buf, int32_t len) {
			int32_t r;
			if ((r = tryFixRead(buf, len)) == -1) CP::Socket::read(buf, len);
			else return r;
		}
		int32_t recv(void* buf, int32_t len, int32_t flags = 0) {
			int32_t r;
			if ((r = tryFixRead(buf, len)) == -1) CP::Socket::recv(buf, len, flags);
			else {
				if (flags & MSG_WAITALL) {
					if (r < len) {
						int32_t tmp = CP::Socket::recv(((uint8_t*) buf) + r, len - r, flags);
						if (tmp > 0) r += tmp;
					}
				}
				return r;
			}
		}
		void read(void* buf, int32_t len, const Callback& cb, bool repeat = false) {
			int32_t r;
			if ((r = tryFixRead(buf, len)) == -1) CP::Socket::read(buf, len, cb, repeat);
			else {
				cb(r);
				if (repeat) CP::Socket::read(buf, len, cb, true);
			}
		}
		void recv(void* buf, int32_t len, int32_t flags, const Callback& cb, bool repeat = false) {
			int32_t r;
			if ((r = tryFixRead(buf, len)) == -1) CP::Socket::recv(buf, len, flags, cb, repeat);
			else {
				if (flags & MSG_WAITALL) {
					if (r < len) {
						CP::Socket::recv(((uint8_t*) buf) + r, len - r, flags,
								[cb,r,repeat,buf,len,flags,this](int i)
								{
									int r1=r;
									if(i>0)r1+=i;
									cb(r1);
									if(repeat)CP::Socket::recv(buf, len, flags, cb, true);
								}, false);
						return;
					}
				}
				if (repeat) CP::Socket::recv(buf, len, flags, cb, true);
			}
		}
		~SocketProxy() {
			if (buf != NULL) delete[] buf;
		}
	};
	class socketd_client
	{
	public:
		CP::Poll& p;
		RGC::Ref<CP::Socket> sock;
		function<void(Socket*, const function<void()>&)> cb;
		protocolHeader ph;
		prot_handleConnection ph1;
		/*vector<int> acks;
		 uint8_t* tmp;
		 int tmplen;
		 bool writing;
		 void startWrite() {
		 if(writing || acks.size()<=0)return;
		 int sz=sizeof(protocolHeader)+sizeof(prot_ackConnection);
		 int sz1=sz*acks.size();
		 if(tmplen<sz1) {
		 if(tmp!=NULL)free(tmp);
		 tmplen=sz1;
		 tmp=(uint8_t*)malloc(sz1);
		 }
		 for(int i=0;i<acks.size();i++) {
		 protocolHeader* ph=(protocolHeader*)(tmp+(sz*i));
		 prot_ackConnection* ack=(prot_ackConnection*)(ph+1);
		 ph->type=protocolHeader::ackConnection;
		 ack->id=acks[i];
		 ack->success=true;
		 }
		 acks.resize(0);
		 writing=true;
		 sock->write(tmp,sz1,[this](int r) {
		 writing=false;
		 if(r<=0)return;
		 startWrite();
		 });
		 }*/
		void startRead();
		void readCB(int r) {
			if (r <= 0) {
				cb(NULL, NULL);
				return;
			}
			switch (ph.type) {
				case protocolHeader::handleConnection:
				{
					sock->read(&ph1, sizeof(ph1), [this](int r) {
						if(r<=0) {
							cb(NULL,NULL);
							return;
						}
						int fd=recvfd(sock->handle);
						if(fd<0) {
							cb(NULL,NULL);
							return;
						}

						auto cb1=[this](CP::Socket* newsock) {
							p.add(*newsock);
							int64_t id=ph1.id;
							//printf("aaaaa %lli %i %i %i\n",ph1.id, ph1.d, ph1.t, ph1.p);
							cb(newsock, [this,id]() {
										protocolHeader ph;
										ph.type=protocolHeader::ackConnection;
										prot_ackConnection ack;
										ack.id=id;
										ack.success=true;
										if(sock->send(&ph,sizeof(ph),MSG_DONTWAIT)!=sizeof(ph)) throw runtime_error("unix socket buffer overflow");
										if(sock->send(&ack,sizeof(ack),MSG_DONTWAIT)!=sizeof(ack)) throw runtime_error("unix socket buffer overflow");
										//acks.push_back(id);
										//startWrite();
									});
							newsock->release();
							startRead();
						};
						//printf("asdfg %i\n",ph1.bufferLen);
						if(ph1.bufferLen<=0) {
							CP::Socket* newsock;
							newsock=new CP::Socket(fd, ph1.d, ph1.t, ph1.p);
							cb1(newsock);
						} else {
							SocketProxy* newsock;
							newsock=new SocketProxy(fd, ph1.d, ph1.t, ph1.p, ph1.bufferLen);
							int r=sock->recv(newsock->buf, ph1.bufferLen,MSG_WAITALL);
							if(r<=0) {
								cb(NULL,NULL);
								return;
							}
							cb1(newsock);
						}

					});
					return;
				}
			}
			startRead();
		}
		socketd_client(CP::Poll& p, const function<void(Socket*, const function<void()>&)>& cb,
				CP::Socket* sock = NULL) :
				p(p), cb(cb)/*, tmp(NULL),tmplen(0),writing(false)*/{
			if (sock == NULL) {
				char* tmp = getenv("SOCKETD_FD");
				if (tmp == NULL) throw logic_error("environment \"SOCKETD_FD\" not set");
				sock = RGC::newObj<CP::Socket>(atoi(tmp), AF_UNIX, SOCK_STREAM, 0);
				p.add(*sock);
			}
			this->sock = sock;
			startRead();
		}
	};
	void socketd_client::startRead() {
		sock->read(&ph, sizeof(ph), [this](int r) {readCB(r);});
	}

}
#endif /* SOCKETD_CLIENT_H_ */
