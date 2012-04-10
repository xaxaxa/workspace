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
#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>
#include <cplib/asyncfile.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "JoinStream.h"
#include "socketmux.h"
#include <boost/shared_ptr.hpp>
#include "iptsocks.H"
using namespace boost;
//#define nosocks
//#define sshp

#define SO_ORIGINAL_DST 80
#ifdef __debug_print123
#define dbgprint(msg) cout << msg << endl
#else
#define dbgprint(msg)
#endif
#ifdef sshp
#define LISTENPORT 2222
#else
#define LISTENPORT 6969
#endif

//#define __asdf
//#define __client
using namespace std;
using namespace xaxaxa;
using namespace Sockets;
class asdf: Object
{
public:
	FUNCTION_DECLARE(function1, int, int);
	virtual int func1(int a)
	{
		cout << "func1 called: this=" << this << "; a=" << a << "\n";
		return 123;
	}
	//static INLINE RETVAL NAME(void* obj,__VA_ARGS__)
	FUNCTION_DECLWRAPPER(func1_, int, int a)
	{
		return ((asdf*) obj)->func1(a);
	}
	;
	//FUNCTION_DECLARE(f,int);
	//FUNCTION_EXPORT(asdf,func1_,func1,int,int a)
	//static int func1_(void* obj,int a){return ((asdf*)obj)->func1(int a);}
};
inline void invert(Buffer* b)
{
	int i;
	for (i = 0; i < b->Length; i++)
	{
		*(((unsigned char*) b->Data) + i) = ~*(((unsigned char*) b->Data) + i);
	}
}
inline void rotate(const Buffer& b, __uint8_t x)
{
	int i;
	for (i = 0; i < b.Length; i++)
	{
		*(((__uint8_t *) b.Data) + i) = (*(((__uint8_t *) b.Data) + i)) + x;
	}
}
FUNCTION_DECLWRAPPER(procbuffer, void, JoinStream* s, Buffer* b)
{
	invert(b);
}
FUNCTION_DECLWRAPPER(procbuffer_n, void, boost::shared_ptr<socketmux> s, boost::shared_ptr<socketmux::item> it, bool direction_out, const Buffer& b)
{
	rotate(b, direction_out ? -69 : 69);
	/*int i;
	 for(i=0;i<b.length;i++)
	 {
	 *(((unsigned char*)b.buf)+i)=~*(((unsigned char*)b.buf)+i);
	 }*/
}
SocketManager* m = SocketManager::GetDefault();
class client: public Object
{
public:
	Socket s1;
	Buffer buf1;
	Buffer buf2;
	Socket s2;
	inline void s1_r(SocketManager* m, Socket s);
	inline void s1_w(SocketManager* m, Socket s);
	inline void s2_r(SocketManager* m, Socket s);
	inline void s2_w(SocketManager* m, Socket s);
	inline void cb3(SocketManager* m, Socket s);
	client(Socket s) :
			buf1(4096 * 4), buf2(4096 * 4), s2(AF_INET, SOCK_STREAM, 0)
	{
		this->s1 = s;
		sockaddr_in dstaddr;
		socklen_t dstlen = sizeof(dstaddr);
		getsockopt(s._f, SOL_IP, SO_ORIGINAL_DST, (struct sockaddr *) &dstaddr, &dstlen);
		char tmp[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(dstaddr.sin_addr), (char*) &tmp, dstlen);
		IPEndPoint ep(dstaddr);
		cout << tmp << ":" << ep.Port << endl;
		void* asdfg = 0;
		//Object obj=asdfg;
		boost::function<void(client*, SocketManager*, Socket)> fn = &client::cb3;
		//SocketManager::Callback(fn,this);
		m->BeginConnect(s2, &ep, SocketManager::Callback(&client::cb3, this));
		//m->BeginConnect(s2,&ep,(SocketManager::Callback)bind(&client::cb3, this, _1, _2));
		//m.BeginRecv(s,buf,SocketManager::Callback(_cb1,this));
	}
	virtual ~client()
	{
		//buf1.Release();
		//buf2.Release();
		m->Cancel(s1);
		m->Cancel(s2);
		s1.Close();
		s2.Close();
	}
};
void client::s1_r(SocketManager* m, Socket s)
{
	dbgprint("s1_r");
	try
	{
		int i = m->EndRecv(s);
		if (i == 0)
		{
			this->Release();
			return;
		}
		//fwrite(buf.buf,i,1,stdout);
		m->BeginSend(this->s2, buf1.SubBuffer(0, i), SocketManager::Callback(&client::s2_w, this));
	} catch (Exception& ex)
	{
		this->Release();
	}
}
void client::s1_w(SocketManager* m, Socket s)
{
	dbgprint("s1_w");
	try
	{
		m->EndSend(s);
		m->BeginRecv(this->s2, buf2, SocketManager::Callback(&client::s2_r, this));
	} catch (Exception& ex)
	{
		this->Release();
	}
}
void client::s2_r(SocketManager* m, Socket s)
{
	dbgprint("s2_r");
	try
	{
		int i = m->EndRecv(s);
		if (i == 0)
		{
			this->Release();
			return;
		}
		//fwrite(buf.buf,i,1,stdout);
		m->BeginSend(this->s1, buf2.SubBuffer(0, i), SocketManager::Callback(&client::s1_w, this));
		//m.BeginRecv(s,buf,SocketManager::Callback(cb1,this));
	} catch (Exception& ex)
	{
		this->Release();
	}
}
void client::s2_w(SocketManager* m, Socket s)
{
	dbgprint("s2_w");
	try
	{
		m->EndSend(s);
		m->BeginRecv(this->s1, buf1, SocketManager::Callback(&client::s1_r, this));
	} catch (Exception& ex)
	{
		this->Release();
	}
}
void client::cb3(SocketManager* m, Socket s)
{
	try
	{
		m->EndConnect(s);
		m->BeginRecv(this->s1, buf1, SocketManager::Callback(&client::s1_r, this));
		m->BeginRecv(this->s2, buf2, SocketManager::Callback(&client::s2_r, this));
	} catch (Exception& ex)
	{
		this->Release();
	}
}

struct tmp123
{
	Socket s1, s2;
	IPEndPoint ep;
	JoinStream* j;
};
static void socks_cb1(void* obj, Stream* s, void* v)
{
	tmp123* tmp = (tmp123*) obj;
	dbgprint("################socks sent_callback##################");
	tmp->j->dowrite2 = true;
	tmp->j->begin2w();
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
	}dbgprint("################socks connected##################");
	tmp->j->Begin();
	delete tmp;
}
FUNCTION_DECLWRAPPER(cb_connect, void, SocketManager* m, Socket sock)
{
	tmp123* tmp = (tmp123*) obj;
	tmp->j = NULL;
	try
	{
		m->EndConnect(sock);
		dbgprint("connected");
	} catch (Exception& ex)
	{
		tmp->s1.Close();
		tmp->s2.Close();
		return;
	}
	try
	{
		SocketStream* str1 = new SocketStream(tmp->s1);
		SocketStream* str2 = new SocketStream(tmp->s2);
		JoinStream* j = new JoinStream(str1, str2);
#ifdef sshp
		j->ProcessBuffer1 = JoinStream::BufferCallback(procbuffer, NULL);
		j->ProcessBuffer2 = j->ProcessBuffer1;
#endif
		str1->Release();
		str2->Release();
		//j->Begin();
		//delete tmp;
		tmp->j = j;
#ifdef nosocks
		j->Begin();
		delete tmp;
#else
		j->begin1r();
		SOCKS5::socks_connect(j->s2, &(tmp->ep), SOCKS5::Callback(socks_cb, tmp),
				SOCKS5::Callback(socks_cb1, tmp));
#endif
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
	dbgprint("accepted: " << s._s);
	//new client(s);
	Socket s2(AF_INET, SOCK_STREAM, 0);
	//dbgprint("asdf2");
	tmp123* tmp = NULL;
	try
	{
#ifndef sshp
		sockaddr_in dstaddr;
		socklen_t dstlen = sizeof(dstaddr);
		//dbgprint("asdf3");
		if (getsockopt(s._f, SOL_IP, SO_ORIGINAL_DST, (struct sockaddr *) &dstaddr, &dstlen) != 0)
			throw Exception(errno);
		//dbgprint("asdf4");
		//char tmp[INET_ADDRSTRLEN];
		//inet_ntop(AF_INET,&(dstaddr.sin_addr),(char*)&tmp,dstlen);
		IPEndPoint ep(dstaddr);
#else
		IPEndPoint ep(IPAddress("127.0.0.1"), 22);
#endif
		tmp = new tmp123();
		tmp->ep = ep;
		//IPEndPoint ep(IPAddress("127.0.0.1"),9999);
		//dbgprint("asdf1");
		tmp->s1 = s;
		tmp->s2 = s2;
#ifdef nosocks
		m->BeginConnect(s2, &ep, SocketManager::Callback(cb_connect, tmp));
#else
		IPEndPoint ep2(IPAddress("127.0.0.1"), 9999);
		m->BeginConnect(s2, &ep2, SocketManager::Callback(cb_connect, tmp));
#endif
	} catch (Exception& ex)
	{
		s.Close();
		s2.Close();
		if (tmp != NULL)
			delete tmp;
	}
	m->BeginAccept(sock, SocketManager::Callback(cb1, NULL));
}
/*class fuck:public Object
 {
 public:
 int i;
 fuck()
 {

 }
 ~fuck()
 {
 cout << "fuck::destructor";
 }
 };*/
class class1
{
public:
	int fgsdhj;
	class1()
	{
		fgsdhj = 34;
	}
	~class1()
	{
		cout << "destruct" << endl;
	}
};
void segvh(int i)
{
	throw new PointerException();
}
#ifdef __asdf
#ifdef __client
shared_ptr<socketmux> asdfg(new socketmux());

FUNCTION_DECLWRAPPER(cb3, void, SocketManager* m, Socket sock)
{
	m->EndConnect(sock);
	asdfg->dorecv = asdfg->dosend = true;
	asdfg->BeginRecv();
	asdfg->BeginSend();
}
#endif
FUNCTION_DECLWRAPPER(srvc_cb, void, SocketManager* m, Socket sock)
{
	shared_ptr<socketmux::item> item = *((shared_ptr<socketmux::item>*)obj);
	item->dorecv = item->dosend = true;
	item->BeginRecv();
	item->BeginSend();
}
FUNCTION_DECLWRAPPER(handle_request, void, socketmux* asdfg, Buffer* Data, shared_ptr<socketmux::item>& it)
{
	//shared_ptr<socketmux::item> item=(*iter).second;
	it->dorecv = false;
	it->dosend = false;
	sockaddr* addr = (sockaddr*)Data->Data;
	Sockets::EndPoint* ep = Sockets::EndPoint::FromSockAddr(addr);
	Socket s(addr->sa_family, SOCK_STREAM, 0);
	it->s = shared_ptr<SocketStream>(new SocketStream(s));
	m->BeginConnect(s, ep, SocketManager::Callback(srvc_cb, &it));
	delete ep;
}
FUNCTION_DECLWRAPPER(cb2, void, SocketManager* m, Socket sock)
{
	Socket s = m->EndAccept(sock);
	m->BeginAccept(sock, SocketManager::Callback(cb2, NULL));
	try
	{
#ifdef __client
		sockaddr_in dstaddr;
		socklen_t dstlen = sizeof(dstaddr);
		if(getsockopt(s._s, SOL_IP, SO_ORIGINAL_DST, (struct sockaddr *)&dstaddr, &dstlen) != 0)throw Exception(errno);
		IPEndPoint ep(dstaddr);
		int size = ep.GetSockAddrSize();
		char tmp[size];
		ep.GetSockAddr((sockaddr*)tmp);
		Buffer tmp1((void*)tmp, size);
		map<int, shared_ptr<socketmux::item> >::iterator iter = asdfg->AddStream(shared_ptr<Stream>(new SocketStream(s)), &tmp1);
		shared_ptr<socketmux::item> it = (*iter).second;
		it->BeginSend();
		it->BeginRecv();
		dbgprint("fuckkkkkkkkkkkkk");
#else
		shared_ptr<socketmux> tmp(new socketmux());
		tmp->main = shared_ptr<SocketStream>(new SocketStream(s));
		tmp->ProcessBuffer = socketmux::BufferCallback(procbuffer_n, NULL);
		tmp->BeginRecv();
		tmp->BeginSend();
		tmp->ConnectionRequest = socketmux::RequestCallback(handle_request, NULL);
#endif
	}
	catch(Exception ex)
	{}
}

#endif
int main(int argc, char** argv)
{
	if (argc > 0 && strcmp(argv[0], "iptsocks") == 0)
		return iptsocks_main(argc, argv);
	if (argc > 1 && strcmp(argv[1], "iptsocks") == 0)
		return iptsocks_main(argc - 1, argv + 1);
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
	//signal(SIGSEGV,segvh);
	//int i=5;
	//cout<<(++i)<<endl;
	/*BitArray x(65);
	 cout<<x.size<<endl;
	 x.Set(5,false);
	 x.Set(40,true);
	 cout<<x.Get(5)<<endl;
	 cout<<x.Get(40)<<endl;
	 cout<<x.Get(42)<<endl;
	 return 0;*/
	/*class1 x;
	 cout << &x << endl;
	 while(1)
	 {
	 sleep(1000);
	 }
	 return 0;*/
	/*asdf* a=new asdf();
	 cout << "a=" << a;
	 asdf::function1 f;
	 FUNCTION_GET1(a,f,func1_);
	 call_func(f);*/
	//m=new SocketManager();
	//m->Release();
	//SocketManager m;
	//cout << "m=" << m;
#ifdef __asdf
	Socket s(AF_INET, SOCK_STREAM, 0);
#ifdef __client
	IPEndPoint ep(IPAddress("0.0.0.0"), 6970);
#else
	IPEndPoint ep(IPAddress("0.0.0.0"), 2012);
#endif
	int tmp12345 = 1;
	setsockopt(s._s, SOL_SOCKET, SO_REUSEADDR, &tmp12345, sizeof(tmp12345));
	s.Bind(&ep);
	s.Listen(10);
#ifdef __client
	asdfg->ProcessBuffer = socketmux::BufferCallback(procbuffer_n, NULL);
	IPEndPoint ep1(IPAddress("24.83.197.188"), 2012);
	Socket ctrlsock(AF_INET, SOCK_STREAM, 0);
	asdfg->main = shared_ptr<SocketStream>(new SocketStream(ctrlsock));
	asdfg->dosend = asdfg->dorecv = false;
	m->BeginConnect(ctrlsock, &ep1, SocketManager::Callback(cb3, NULL));
#endif
	m->BeginAccept(s, SocketManager::Callback(cb2, NULL));
	//cout << s._s;
	//Object *o=&m;
	//o->Dispose();
	m->EventLoop();
	s.Close();
#else
	Socket s(AF_INET, SOCK_STREAM, 0);
	IPEndPoint ep(IPAddress("0.0.0.0"), LISTENPORT);
	s.Bind(&ep);
	s.Listen(10);
	m->BeginAccept(s, SocketManager::Callback(cb1, NULL));
	//cout << s._s;
	//Object *o=&m;
	//o->Dispose();
	m->EventLoop();
	s.Close();
#endif
	return 0;
}

