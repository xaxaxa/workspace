#ifndef __ASYNCSOCK_H
#define __ASYNCSOCK_H

#include <map>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include "cplib.hpp"
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/un.h>
#include <boost/function.hpp>
#ifdef __debug_print123
#define dbgprint(msg) cout << msg << endl
#else
#define dbgprint(msg)
#endif
using namespace std;
namespace xaxaxa
{
	namespace Sockets
	{
		typedef FILEDES SOCKET;
#define CreateSocket(domain,type,protocol) socket(domain,type,protocol);

		struct IPAddress
		{
			in_addr a;
			inline IPAddress()
			{
			}
			inline IPAddress(const char* addr/*NOT hostname*/)
			{
				inet_pton(AF_INET, addr, &a.s_addr);
			}
			inline IPAddress(const in_addr& a)
			{
				this->a = a;
			}
		};
		struct IPv6Address
		{
			in6_addr a;
			inline IPv6Address()
			{
			}
			inline IPv6Address(const char* addr)
			{
				inet_pton(AF_INET6, addr, &a.__in6_u);
			}
			inline IPv6Address(const in6_addr& a)
			{
				this->a = a;
			}
		};
		class EndPoint
		{
		public:
			int AddressFamily;
			virtual void GetSockAddr(sockaddr* addr) const=0;
			virtual void SetSockAddr(const sockaddr* addr)=0;
			virtual int GetSockAddrSize() const=0;
			static EndPoint* FromSockAddr(const sockaddr* addr);
			static EndPoint* CreateNull(int AddressFamily);
			virtual void Clone(EndPoint& to) const=0;
			//static EndPoint Resolve(
		};
		class IPEndPoint: public EndPoint
		{
		public:
			IPAddress Address;
			int Port;
			IPEndPoint()
			{
				this->AddressFamily = AF_INET;
			}
			IPEndPoint(IPAddress address, int port)
			{
				this->AddressFamily = AF_INET;
				this->Address = address;
				this->Port = port;
			}
			void set_addr(const sockaddr_in& addr)
			{
				this->AddressFamily = AF_INET;
				this->Address = IPAddress(addr.sin_addr);
				this->Port = ntohs(addr.sin_port);
			}
			virtual void SetSockAddr(const sockaddr* addr)
			{
				if (addr->sa_family != AF_INET)
					throw Exception(
							"attemting to set the address of an IPEndPoint to a sockaddr that is not AF_INET");
				set_addr(*(sockaddr_in*) addr);
			}
			IPEndPoint(const sockaddr_in& addr)
			{
				set_addr(addr);
			}
			virtual void GetSockAddr(sockaddr* addr) const
			{
				sockaddr_in* addr_in = (sockaddr_in*) addr;
				addr_in->sin_family = AF_INET;
				addr_in->sin_port = htons(Port);
				addr_in->sin_addr = Address.a;
			}
			virtual int GetSockAddrSize() const
			{
				return sizeof(sockaddr_in);
			}
			virtual void Clone(EndPoint& to) const
			{
				if (to.AddressFamily != AddressFamily)
					throw Exception(
							"attempting to clone an EndPoint to another EndPoint with a different AddressFamily");
				IPEndPoint& tmp((IPEndPoint&)to);
				tmp.Address = Address;
				tmp.Port = Port;
			}
		};
		class IPv6EndPoint: public EndPoint
		{
		public:
			IPv6Address Address;
			int Port;
			IPv6EndPoint()
			{
				this->AddressFamily = AF_INET6;
			}
			IPv6EndPoint(IPv6Address address, int port)
			{
				this->AddressFamily = AF_INET6;
				this->Address = address;
				this->Port = port;
			}
			void set_addr(const sockaddr_in6& addr)
			{
				this->AddressFamily = AF_INET6;
				this->Address = IPv6Address(addr.sin6_addr);
				this->Port = ntohs(addr.sin6_port);
			}
			IPv6EndPoint(const sockaddr_in6& addr)
			{
				set_addr(addr);
			}
			virtual void SetSockAddr(const sockaddr* addr)
			{
				if (addr->sa_family != AF_INET6)
					throw Exception(
							"attemting to set the address of an IPv6EndPoint to a sockaddr that is not AF_INET6");
				set_addr(*(sockaddr_in6*) addr);
			}
			virtual void GetSockAddr(sockaddr* addr) const
			{
				sockaddr_in6* addr_in = (sockaddr_in6*) addr;
				addr_in->sin6_family = AF_INET6;
				addr_in->sin6_port = htons(Port);
				addr_in->sin6_addr = Address.a;
			}
			virtual int GetSockAddrSize() const
			{
				return sizeof(sockaddr_in);
			}
			virtual void Clone(EndPoint& to) const
			{
				if (to.AddressFamily != AddressFamily)
					throw Exception(
							"attempting to clone an EndPoint to another EndPoint with a different AddressFamily");
				IPv6EndPoint& tmp((IPv6EndPoint&)to);
				tmp.Address = Address;
				tmp.Port = Port;
			}
		};
		class UNIXEndPoint: public EndPoint
		{
		public:
			string Name;
			UNIXEndPoint()
			{
				this->AddressFamily = AF_UNIX;
			}
			UNIXEndPoint(string name)
			{
				this->AddressFamily = AF_UNIX;
				Name = name;
			}
			void set_addr(const sockaddr_un& addr)
			{
				this->AddressFamily = AF_UNIX;
				this->Name = addr.sun_path;
			}
			UNIXEndPoint(const sockaddr_un& addr)
			{
				set_addr(addr);
			}
			virtual void SetSockAddr(const sockaddr* addr)
			{
				if (addr->sa_family != AF_UNIX)
					throw Exception(
							"attemting to set the address of an UNIXEndPoint to a sockaddr that is not AF_UNIX");
				set_addr(*(sockaddr_un*) addr);
			}
			virtual void GetSockAddr(sockaddr* addr) const
			{
				sockaddr_un* a = (sockaddr_un*) addr;
				a->sun_family = AF_UNIX;
				strncpy(a->sun_path, Name.c_str(), Name.length());
				a->sun_path[Name.length()] = '\0';
			}
			virtual int GetSockAddrSize() const
			{
				return sizeof(sa_family_t) + Name.length() + 1;
			}
			virtual void Clone(EndPoint& to) const
			{
				if (to.AddressFamily != AddressFamily)
					throw Exception(
							"attempting to clone an EndPoint to another EndPoint with a different AddressFamily");
				UNIXEndPoint& tmp((UNIXEndPoint&)to);
				tmp.Name = Name;
			}
		};
		struct Socket: public File
		{
			//SOCKET _s;
			/*bool autoClose;
			 inline Socket NoDestruct()
			 {
			 Socket s=*this;
			 s.autoClose=false;
			 return s;
			 }*/
			inline Socket()
			{
			}
			inline Socket(int domain, int type, int protocol)
			{
				_f = CreateSocket(domain,type,protocol);
				if (_f < 0)
					throw Exception(errno);
				//int set = 1;
				//setsockopt(_s, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
				dbgprint("socket " << _f << " created");
				//this->autoClose=autoClose;
			}
			inline Socket(SOCKET s)
			{
				_f = s;
				//int set = 1;
				//setsockopt(_s, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
				//this->autoClose=autoClose;
			}
			/*inline void Close()
			 {
			 //throw Exception();
			 dbgprint("socket " << _f << " closed");
			 if(_s!=-1)close(_f);
			 _s=-1;
			 }*/
			inline void Shutdown(int how)
			{
				shutdown(_f, how);
			}
			inline int Send(Buffer buf, Int flags = 0)
			{
				retry: Int tmp = send(_f, buf.Data, buf.Length, flags);
				if (tmp < 0)
				{
					if (errno == 4)
						goto retry;
					throw Exception(errno);
				}
				else
					return tmp;
			}
			inline int Recv(Buffer buf, Int flags = 0)
			{
				retry: Int tmp = recv(_f, buf.Data, buf.Length, flags);
				if (tmp < 0)
				{
					if (errno == 4)
						goto retry;
					throw Exception(errno);
				}
				else
					return tmp;
			}
			inline int RecvFrom(Buffer buf, EndPoint& ep, Int flags = 0)
			{
				socklen_t size = ep.GetSockAddrSize();
				uint8_t addr[size];
				//ep->GetSockAddr((sockaddr*)tmp);
				retry: Int tmp = recvfrom(_f, buf.Data, buf.Length, flags, (sockaddr*) addr, &size);
				if (tmp < 0)
				{
					if (errno == 4)
						goto retry;
					throw Exception(errno);
				}
				else
				{
					ep.SetSockAddr((sockaddr*) addr);
					return tmp;
				}
			}
			inline int SendTo(Buffer buf, const EndPoint& ep, Int flags = 0)
			{
				socklen_t size = ep.GetSockAddrSize();
				uint8_t addr[size];
				ep.GetSockAddr((sockaddr*) addr);
				retry: Int tmp = sendto(_f, buf.Data, buf.Length, flags, (sockaddr*) addr, size);
				if (tmp < 0)
				{
					if (errno == 4)
						goto retry;
					throw Exception(errno);
				}
				else
				{
					//ep.SetSockAddr((sockaddr*)addr);
					return tmp;
				}
			}
			inline Socket Accept(Int flags = 0)
			{
				retry: SOCKET s = accept4(_f, NULL, NULL, flags);
				if (s == -1)
				{
					if (errno == 4)
						goto retry;
					throw Exception(errno);
				}
				return Socket(s);
			}
			inline void Connect(sockaddr *addr, int addr_size = 0)
			{
				retry: Int tmp = connect(_f, addr, (addr_size == 0 ? sizeof(sockaddr) : addr_size));
				if (tmp != 0 && errno != 115)
				{
					if (errno == 4)
						goto retry;
					throw Exception(errno);
				}
			}
			inline void Bind(sockaddr *addr, Int addr_size = 0)
			{
				int tmp12345 = 1;
				setsockopt(_f, SOL_SOCKET, SO_REUSEADDR, &tmp12345, sizeof(tmp12345));
				if (::bind(_f, addr, (Int) (addr_size == 0 ? sizeof(sockaddr) : addr_size)) != 0)
					throw Exception(errno);
			}
			inline void Bind(EndPoint *ep)
			{
				int tmp12345 = 1;
				setsockopt(_f, SOL_SOCKET, SO_REUSEADDR, &tmp12345, sizeof(tmp12345));
				Int size = ep->GetSockAddrSize();
				uint8_t tmp[size];
				ep->GetSockAddr((sockaddr*) tmp);
				if (::bind(_f, (sockaddr*) tmp, size) != 0)
					throw Exception(errno);
			}
			inline void Bind(const EndPoint& ep)
			{
				Bind((EndPoint*) &ep);
			}
			inline void Connect(EndPoint *ep)
			{
				int size = ep->GetSockAddrSize();
				uint8_t asdf[size];
				ep->GetSockAddr((sockaddr*) asdf);
				retry: int tmp = connect(_f, (sockaddr*) asdf, size);
				if (tmp != 0 && errno != 115)
				{
					if (errno == 4)
						goto retry;
					throw Exception(errno);
				}
			}
			inline void Connect(const EndPoint& ep)
			{
				Connect((EndPoint*) &ep);
			}
			inline void Listen(int backlog)
			{
				if (listen(_f, backlog) != 0)
					throw Exception(errno);
			}
			inline int GetFlags()
			{
				return fcntl(_f, F_GETFL, 0);
			}
			inline void SetFlags(int f)
			{
				if (fcntl(_f, F_SETFL, f) < 0)
					throw Exception(errno, "could not set socket flags");
			}
			/*inline ~Socket()
			 {
			 if(autoClose)Close();
			 }*/
		};
		class SocketManager;
		static SocketManager* __def_sockmanager;
		class SocketManager: public Object
		{
		public:

			inline static SocketManager* GetDefault()
			{
				if (__def_sockmanager == NULL)
					__def_sockmanager = new SocketManager();
				return __def_sockmanager;
			}
			//typedef boost::function<void (SocketManager*, Socket)> Callback;
			FUNCTION_DECLARE(Callback,void,SocketManager*,Socket);
			int epollfd;
			//arraylist<Socket> r,w,x;
			//arraylist<Socket> r2,w2,x2;
			//map<Socket,void*> r_info;
			//map<Socket,void*> w_info;
			//map<Socket,void*> x_info;
			/*enum taskType
			 {
			 read,write,accept
			 };*/

			struct taskInfo
			{
				//bool initialized;
				int bits; //1: init; 2: read_fill; 4: write_fill;
				SOCKET s;
				Callback cb_recv, cb_send;
				__uint32_t events;
				__uint32_t new_events;
				Buffer r_buf, w_buf;
				int r_length, w_length;
				int r_errno, w_errno;
				inline taskInfo()
				{
					bits = 0;
				}
				//bool inLoop;//reserved; do not modify
			};
			map<SOCKET, taskInfo> info;
			//ArrayList<SOCKET> tmp_erased;
			map<SOCKET, bool> tmp_erased;
			SocketManager();
			~SocketManager();
			virtual void EventLoop();
			void BeginRecv(Socket s, Buffer buf, Callback cb, bool fill = false);
			int EndRecv(Socket s);
			int EndRecvFrom(Socket s, EndPoint& ep);
			void BeginAccept(Socket s, Callback cb);
			Socket EndAccept(Socket s, int flags = 0);
			void BeginSend(Socket s, Buffer buf, Callback cb, bool fill = false);
			int EndSend(Socket s);
			int EndSendTo(Socket s, EndPoint& ep);
			void BeginConnect(Socket s, EndPoint* ep, Callback cb);
			void EndConnect(Socket s);
			taskInfo* __current_task_;
			void Cancel(Socket s);
			void CancelRead(Socket s);
			void CancelWrite(Socket s);
		};
		class SocketStream: public Stream
		{
		public:
			Socket socket;
			SocketManager* m;
			SocketStream()
			{
			}
			SocketStream(Socket socket, SocketManager *m = NULL)
			{
				this->socket = socket;
				if (m == NULL)
					m = SocketManager::GetDefault();
				this->m = m;
			}
			virtual ~SocketStream()
			{
				Close();
			}
			virtual int Read(const Buffer& buf)
			{
				return socket.Recv(buf, 0);
			}
			virtual void Write(const Buffer& buf)
			{
				int bw = 0;
				int off = 0;
				while (off < buf.Length && (bw = socket.Send(buf.SubBuffer(off))) > 0)
					off += bw;
			}
			virtual void Flush()
			{

			}
			virtual void Close()
			{
				m->Cancel(socket);
				socket.Close();
			}
			Callback r_cb;
			Callback w_cb;FUNCTION_DECLWRAPPER(_r_cb,void,SocketManager* m,Socket sock)
			{
				FUNCTION_CALL(((SocketStream*)obj)->r_cb, (SocketStream*)obj);
			}
			FUNCTION_DECLWRAPPER(_w_cb,void,SocketManager* m,Socket sock)
			{
				FUNCTION_CALL(((SocketStream*)obj)->w_cb, (SocketStream*)obj);
			}
			virtual void BeginRead(const Buffer& buf, Callback cb)
			{
				dbgprint("SocketStream::BeginRead()");
				r_cb = cb;
				m->BeginRecv(socket, buf, SocketManager::Callback(_r_cb, this), false);
			}
			;
			virtual void BeginWrite(const Buffer& buf, Callback cb)
			{
				dbgprint("SocketStream::BeginWrite()");
				w_cb = cb;
				m->BeginSend(socket, buf, SocketManager::Callback(_w_cb, this), true);
			}
			virtual int EndRead()
			{
				return m->EndRecv(socket);
			}
			virtual void EndWrite()
			{
				m->EndSend(socket);
			}
			;
		};
		class SOCKS5
		{
		public:
			//typedef boost::function<void (Stream*, void*)> Callback;
			FUNCTION_DECLARE(Callback,void,Stream*,void*);
			struct tmp
			{
				Buffer b;
				StringBuilder* sb;
				int addrlen;
				Callback cb;
				Callback sent_cb;
				int br;
				Exception ex;
				//void* state;
			};
			static void socks_connect(Stream* s, const char* host, uint16_t port, Callback cb,
					Callback sent_cb);
			static void socks_connect(Stream* s, EndPoint* ep, Callback cb, Callback sent_cb =
					Callback::null);
			static void socks_endconnect(void* v);
			static void cb1(void* obj, Stream* s);
			static void cb2(void* obj, Stream* s);
		};

	}
}

#endif
