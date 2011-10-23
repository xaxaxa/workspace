
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
		typedef int SOCKET;
#define CreateSocket(domain,type,protocol) socket(domain,type,protocol);

		struct IPAddress
		{
			in_addr a;
			inline IPAddress(){}
			inline IPAddress(const char* addr/*NOT hostname*/)
			{
				inet_pton(AF_INET, addr, &a.s_addr);
			}
			inline IPAddress(in_addr& a)
			{
				this->a=a;
			}
		};
		struct IPv6Address
		{
			in6_addr a;
			inline IPv6Address(){}
			inline IPv6Address(const char* addr)
			{
				inet_pton(AF_INET6, addr, &a.__in6_u);
			}
			inline IPv6Address(in6_addr& a)
			{
				this->a=a;
			}
		};
		class EndPoint
		{
		public:
			int AddressFamily;
			virtual void GetSockAddr(sockaddr* addr)=0;
			virtual int GetSockAddrSize()=0;
			static EndPoint* FromSockAddr(sockaddr* addr);
			//static EndPoint Resolve(
		};
		class IPEndPoint:public EndPoint
		{
		public:
			IPAddress Address;
			int Port;
			IPEndPoint(){this->AddressFamily=AF_INET;}
			IPEndPoint(IPAddress address,int port)
			{
				this->AddressFamily=AF_INET;
				this->Address=address;
				this->Port=port;
			}
			IPEndPoint(sockaddr_in& addr)
			{
				this->AddressFamily=AF_INET;
				this->Address=IPAddress(addr.sin_addr);
				this->Port=ntohs(addr.sin_port);
			}
			virtual void GetSockAddr(sockaddr* addr)
			{
				sockaddr_in* addr_in=(sockaddr_in*)addr;
				addr_in->sin_family=AF_INET;
				addr_in->sin_port=htons(Port);
				addr_in->sin_addr=Address.a;
			}
			virtual int GetSockAddrSize()
			{
				return sizeof(sockaddr_in);
			}
		};
		class IPv6EndPoint:public EndPoint
		{
		public:
			IPv6Address Address;
			int Port;
			IPv6EndPoint(){this->AddressFamily=AF_INET6;}
			IPv6EndPoint(IPv6Address address,int port)
			{
				this->AddressFamily=AF_INET6;
				this->Address=address;
				this->Port=port;
			}
			IPv6EndPoint(sockaddr_in6& addr)
			{
				this->AddressFamily=AF_INET6;
				this->Address=IPv6Address(addr.sin6_addr);
				this->Port=ntohs(addr.sin6_port);
			}
			virtual void GetSockAddr(sockaddr* addr)
			{
				sockaddr_in6* addr_in=(sockaddr_in6*)addr;
				addr_in->sin6_family=AF_INET6;
				addr_in->sin6_port=htons(Port);
				addr_in->sin6_addr=Address.a;
			}
			virtual int GetSockAddrSize()
			{
				return sizeof(sockaddr_in);
			}
		};
		class UNIXEndPoint:public EndPoint
		{
		public:
			string Name;
			UNIXEndPoint(){this->AddressFamily=AF_UNIX;}
			UNIXEndPoint(string name)
			{
				this->AddressFamily=AF_UNIX;
				Name=name;
			}
			UNIXEndPoint(sockaddr_un& addr)
			{
				this->AddressFamily=AF_UNIX;
				this->Name=addr.sun_path;
			}
			virtual void GetSockAddr(sockaddr* addr)
			{
				sockaddr_un* a=(sockaddr_un*)addr;
				a->sun_family=AF_UNIX;
				strncpy(a->sun_path,Name.c_str(),Name.length());
				a->sun_path[Name.length()]='\0';
			}
			virtual int GetSockAddrSize()
			{
				return sizeof(sa_family_t)+Name.length()+1;
			}
		};
		struct Socket
		{
			SOCKET _s;
			/*bool autoClose;
			inline Socket NoDestruct()
			{
				Socket s=*this;
				s.autoClose=false;
				return s;
			}*/
			inline Socket(){}
			inline Socket(int domain,int type,int protocol)
			{
				_s=CreateSocket(domain,type,protocol);
				if(_s<0)throw Exception(errno);
				//int set = 1;
				//setsockopt(_s, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
				dbgprint("socket " << _s << " created");
				//this->autoClose=autoClose;
			}
			inline Socket(SOCKET s)
			{
				_s=s;
				//int set = 1;
				//setsockopt(_s, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
				//this->autoClose=autoClose;
			}
			inline void Close()
			{
				//throw Exception();
				dbgprint("socket " << _s << " closed");
				if(_s!=-1)close(_s);
				_s=-1;
			}
			inline int Send(Buffer buf,int flags=0)
			{
			retry:
				int tmp=send(_s,buf.buf,buf.length,flags);
				if(tmp<0)
				{
					if(errno==4)goto retry;
					throw Exception(errno);
				}
				else return tmp;
			}
			inline int Recv(Buffer buf,int flags=0)
			{
			retry:
				int tmp=recv(_s,buf.buf,buf.length,flags);
				if(tmp<0)
				{
					if(errno==4)goto retry;
					throw Exception(errno);
				}
				else return tmp;
			}
			inline Socket Accept(int flags=0)
			{
			retry:
				SOCKET s=accept4(_s,NULL,NULL,flags);
				if(s==-1)
				{
					if(errno==4)goto retry;
					throw Exception(errno);
				}
				return Socket(s);
			}
			inline void Connect(sockaddr *addr,int addr_size=0)
			{
			retry:
				int tmp=connect(_s,addr,(addr_size==0?sizeof(sockaddr):addr_size));
				if(tmp!=0 && errno!=115)
				{
					if(errno==4)goto retry;
					throw Exception(errno);
				}
			}
			inline void Bind(sockaddr *addr,int addr_size=0)
			{
				if(bind(_s,addr,(addr_size==0?sizeof(sockaddr):addr_size))!=0)throw Exception(errno);
			}
			inline void Bind(EndPoint *ep)
			{
				int size=ep->GetSockAddrSize();
				uint8_t tmp[size];
				ep->GetSockAddr((sockaddr*)tmp);
				if(bind(_s,(sockaddr*)tmp,size)!=0)throw Exception(errno);
			}
			inline void Connect(EndPoint *ep)
			{
				int size=ep->GetSockAddrSize();
				uint8_t asdf[size];
				ep->GetSockAddr((sockaddr*)asdf);
			retry:
				int tmp=connect(_s,(sockaddr*)asdf,size);
				if(tmp!=0 && errno!=115)
				{
					if(errno==4)goto retry;
					throw Exception(errno);
				}
			}
			inline void Listen(int backlog)
			{
				if(listen(_s,backlog)!=0)throw Exception(errno);
			}
			inline int GetFlags()
			{
				return fcntl(_s,F_GETFL,0);
			}
			inline void SetFlags(int f)
			{
				if(fcntl(_s,F_SETFL,f)<0)throw Exception(errno,"could not set socket flags");
			}
			/*inline ~Socket()
			{
				if(autoClose)Close();
			}*/
		};
		class SocketManager;
		static SocketManager* __def_sockmanager;
		class SocketManager:public Object
		{
		public:

			inline static SocketManager* GetDefault(){
				if(__def_sockmanager==NULL)
					__def_sockmanager=new SocketManager();
				return __def_sockmanager;
			}
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
				int bits;//1: init; 2: read_fill; 4: write_fill;
				SOCKET s;
				Callback cb_recv,cb_send;
				__uint32_t events;
				__uint32_t new_events;
				Buffer r_buf,w_buf;
				int r_length,w_length;
				int r_errno,w_errno;
				inline taskInfo()
				{
					bits=0;
				}
				//bool inLoop;//reserved; do not modify
			};
			map<SOCKET,taskInfo> info;
			//ArrayList<SOCKET> tmp_erased;
			map<SOCKET,bool> tmp_erased;
			SocketManager();
			~SocketManager();
			virtual void EventLoop();
			void BeginRecv(Socket s, Buffer buf, Callback cb,bool fill=false);
			int EndRecv(Socket s);
			void BeginAccept(Socket s, Callback cb);
			Socket EndAccept(Socket s, int flags=0);
			void BeginSend(Socket s, Buffer buf, Callback cb,bool fill=false);
			int EndSend(Socket s);
			void BeginConnect(Socket s,EndPoint* ep, Callback cb);
			void EndConnect(Socket s);
			taskInfo* __current_task_;
			void Cancel(Socket s);
			void CancelRead(Socket s);
			void CancelWrite(Socket s);
		};
		class SocketStream:public Stream
		{
		public:
			Socket socket;
			SocketManager* m;
			SocketStream()
			{}
			SocketStream(Socket socket,SocketManager *m=NULL)
			{
				this->socket=socket;
				if(m==NULL)m=SocketManager::GetDefault();
				this->m=m;
			}
			virtual ~SocketStream()
			{
				Close();
			}
			virtual int Read(Buffer buf)
			{
				return socket.Recv(buf,0);
			}
			virtual void Write(Buffer buf)
			{
				socket.Send(buf,0);
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
			Callback w_cb;
			FUNCTION_DECLWRAPPER(_r_cb,void,SocketManager* m,Socket sock)
			{
				FUNCTION_CALL(((SocketStream*)obj)->r_cb,(SocketStream*)obj);
			}
			FUNCTION_DECLWRAPPER(_w_cb,void,SocketManager* m,Socket sock)
			{
				FUNCTION_CALL(((SocketStream*)obj)->w_cb,(SocketStream*)obj);
			}
			virtual void BeginRead(Buffer buf,Callback cb)
			{
				dbgprint("SocketStream::BeginRead()");
				r_cb=cb;
				m->BeginRecv(socket,buf,SocketManager::Callback(_r_cb,this),false);
			};
			virtual void BeginWrite(Buffer buf,Callback cb)
			{
				dbgprint("SocketStream::BeginWrite()");
				w_cb=cb;
				m->BeginSend(socket,buf,SocketManager::Callback(_w_cb,this),true);
			}
			virtual int EndRead()
			{
				return m->EndRecv(socket);
			}
			virtual void EndWrite()
			{
				m->EndSend(socket);
			};
		};
		class SOCKS5
		{
		public:
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

			static void socks_connect(Stream* s, EndPoint* ep, Callback cb, Callback sent_cb=Callback::null);
			static void socks_endconnect(void* v);
			static void cb1(void* obj, Stream* s);
			static void cb2(void* obj, Stream* s);
		};


	}
}

#endif
