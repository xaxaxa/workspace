#include <iostream>
#include <vector>
//#include <SFML/Audio.hpp>
//#include <SFML/Graphics.hpp>
#include <cplib/asyncsock.hpp>
#include <set>
using namespace std;
using namespace xaxaxa;
using namespace Sockets;
struct client
{
	Property<EndPoint> ep;
	Socket s;
	SocketStream st;
	Buffer b;
	CircularQueue<Buffer> sendbuffer;
	SocketManager::Callback cb;
	bool writing;
	int write_n;
	string name;
	bool name_init;
	void beginflush()
	{
		if(writing)return;
		write_n=sendbuffer.BeginDequeue();
		if(write_n==-1) return;
		writing=true;
		st.BeginWrite(sendbuffer.GetPointer(write_n), Stream::Callback([this](void* v, Stream* s)
		{
			sendbuffer.EndDequeue(write_n);
			writing=false;
			beginflush();
		}));
	}
	void send(const BufferRef& buf)
	{
		if(buf.Length<=0)return;
		int tmp=sendbuffer.BeginAppend();
		if(tmp<0)
		{
			WARN(2,"buffer overrun"); return;
		}
		Buffer& b=sendbuffer.GetPointer(tmp);
		b=Buffer(buf.Length);
		memcpy(b.Data, buf.Data, buf.Length);
		sendbuffer.EndAppend(tmp);
		beginflush();
	}
	client(Socket s):s(s),st(s),b(4096),sendbuffer(32),writing(false),name_init(false)
	{
		ep=newobj<IPEndPoint>();
		s.GetPeerEndPoint(*ep);
	}
};
set<client*> clients;
void broadcast(BufferRef buf, client* from=NULL, bool system_msg=false)
{
	//if(system_msg) while(buf.Length>0 && buf.Data[buf.Length-1]=='\n')buf.Length--;
	const BufferRef* tmp=&buf;
	if(from!=NULL || system_msg)
	{
		StringBuilder sb;
		if(system_msg)
			sb << "\x1B[41;1;33m";
		if(from!=NULL)
		{
			if(from->name_init)
				sb << from->name << " (";
			sb << from->ep->ToStr();
			if(from->name_init)
				sb << ')';
			sb << ": ";
		}
		sb << buf;
		if(system_msg) sb << "\x1B[0;0;0m\n";
		Buffer* tmpb=new Buffer(sb.ToBuffer());
		tmp=tmpb;
	}
	StandardStream().Write(*tmp);
	for(auto it=clients.begin();it!=clients.end();it++)
	{
		if((*it)!=from && (*it)->name_init)
			(*it)->send(*tmp);
	}
}
BufferRef strip_newline(BufferRef b)
{
	while(b.Length>0 && b.Data[b.Length-1]=='\n')b.Length--;
	return b;
}
int main()
{
	/*
	IPEndPoint ep(IPAddress("0.0.0.0"),26969);
	Socket s(AF_INET,SOCK_DGRAM,0);
	s.Bind(ep);
	Buffer b(4096);
	StandardStream ss;
	while(true)
	{
		int br=s.Recv(b);
		ss << b.SubBuffer(0,br);
	}
	//*/
	/*IPEndPoint ep(IPAddress("0.0.0.0"),7899);
	Socket s1(AF_INET,SOCK_STREAM,0);
	s1.Bind(ep);
	s1.Listen(10);
	while(true)
	{
		Socket s=s1.Accept();
		try{
			Buffer b(4096);
			StandardStream ss;
			while(true)
			{
				int br=s.Recv(b);
				if(br<=0)break;
				ss << b.SubBuffer(0,br);
			}
		}catch(Exception&ex){
			s.Close();
		}
	}*/
	const char* motd="You are now connected to the chat server.\nPlease enter a nickname: ";
	IPEndPoint ep(IPAddress("0.0.0.0"),16969);
	Socket s1(AF_INET,SOCK_STREAM,0);
	s1.Bind(ep);
	s1.Listen(10);
	SocketManager* m = SocketManager::GetDefault();
	struct
	{
		SocketManager::Callback cb;
	} tmp1;
	tmp1.cb=SocketManager::Callback([&](void* v, SocketManager* m, Socket sock)
	{
		Socket s=sock.Accept();
		client* zxcv=new client(s);
		zxcv->send(motd);
		broadcast("Connected", zxcv, true);
		clients.insert(zxcv);
		zxcv->cb=SocketManager::Callback([](void* v, SocketManager* m, Socket sock)
		{
			client* zxcv=(client*)v;
			//try
			//{
				int br;
				if((br=m->EndRecv(sock))<=0)goto close;
				if(zxcv->name_init)
					broadcast(zxcv->b.SubBuffer(0,br), zxcv);
				else
				{
					if(br>50)
					{
						zxcv->send("\x1B[41;1;33mNickname can not be longer than 50 chars.\x1B[0;0;0m\nEnter a new one: ");
						goto aaaaa;
					}
					BufferRef tmp=strip_newline(zxcv->b.SubBuffer(0,br));
					zxcv->name=string((char*)tmp.Data,tmp.Length);
					zxcv->name_init=true;
					broadcast("Set name", zxcv, true);
					StringBuilder sb;
					sb << "List of users (";
					sb << (int)(clients.size()-1);
					sb << "): \n";
					for(auto it=clients.begin();it!=clients.end();it++)
					{
						if((*it)!=zxcv)
						{
							if((*it)->name_init)
								sb << (*it)->name << " (";
							sb << (*it)->ep->ToStr();
							if((*it)->name_init)
								sb << ')';
							sb << '\n';
						}
					}
					sb << "You can now chat.\n";
					zxcv->send(sb.ToBuffer());
				}
			aaaaa:
				m->BeginRecv(sock, zxcv->b, zxcv->cb, false);
			//}catch(Exception& ex){goto close;}
			return;
		close:
			m->Cancel(sock);
			sock.Close();
			clients.erase(zxcv);
			broadcast("Disconnected", zxcv, true);
			delete zxcv;
		}, zxcv);
		m->BeginRecv(s, zxcv->b, zxcv->cb, false);
		m->BeginAccept(sock, tmp1.cb);
	});
	m->BeginAccept(s1, tmp1.cb);
	m->EventLoop();
}

