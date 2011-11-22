#include "headers/asyncsock.hpp"
#include "headers/cplib.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <errno.h>
//#define dbgprint(msg) cout << msg
/*
 * TODO
 *
 * BUG:
 *
 * SIGPIPE on send() and recv()
 *
 *
 *
 */
namespace xaxaxa
{
	namespace Sockets
	{


		EndPoint* EndPoint::FromSockAddr(sockaddr* addr)
		{
			switch(addr->sa_family)
			{
			case AF_INET:
				return new IPEndPoint(*((sockaddr_in*)addr));
			case AF_INET6:
				return new IPv6EndPoint(*((sockaddr_in6*)addr));
			default:
				return NULL;
			}
		}
		const int pollcount = 32;
		SocketManager::SocketManager()
		{
			__current_task_=NULL;
			this->epollfd = epoll_create(pollcount);
			dbgprint("socketmanager " << this->epollfd << " created");
			if (epollfd < 0) perror("epoll_create");
		}
		SocketManager::~SocketManager()
		{
			dbgprint("socketmanager " << this->epollfd << " closed");
			close(this->epollfd);
		}
		void SocketManager::EventLoop()
		{
			epoll_event* events = new epoll_event[pollcount];
			while (1)
			{
				int tmp = epoll_wait(this->epollfd, events, pollcount, -1);
				//cout << tmp << endl;
				if (tmp == -1)
				{
					if(errno == EINTR) continue;
					throw Exception(errno);
				}
				for (int a = 0; a < tmp; a++)
				{
					epoll_event ev = events[a];
					SOCKET s = ev.data.fd;
					if(tmp_erased.count(s)>0)continue;
					dbgprint("s=" << s);
					taskInfo& __current_task = this->info[s];
					__current_task_=&__current_task;
					/*if(!(__current_task.bits&1))
					 {
					 dbgprint(s << "not initialized");
					 this->info.erase(s);
					 continue;
					 }*/
					bool hup=(ev.events & EPOLLHUP) || (ev.events & EPOLLERR);
					__current_task.new_events = __current_task.events
							& (~ev.events);
					if (hup)__current_task.new_events=0;
					dbgprint("__current_task.events=" << __current_task.events);dbgprint("ev.events=" << ev.events);

					if ((__current_task.events & ev.events & EPOLLIN) || (hup && (__current_task.events & EPOLLIN)))
					{
						if (hup)
						{
							__current_task.r_errno = 104;
						}
						else
						{
							if (__current_task.bits & 2)
							{
								int tmp = recv(s, __current_task.r_buf.Data,
										__current_task.r_buf.Length, 0);
								if (tmp < 0)
								{
									if (!__current_task.r_length) __current_task.r_errno
											= errno;
								}
								else if (tmp > 0)
								{
									__current_task.r_length += tmp;
									if (tmp < __current_task.r_buf.Length)
									{
										__current_task.new_events |= EPOLLIN;
										__current_task.r_buf
												= __current_task.r_buf.SubBuffer(
														tmp,
														__current_task.r_buf.Length
																- tmp);
										goto asdf;
									}
								}
							}
						}dbgprint("calling cb_recv=" << __current_task.cb_recv.f);
						FUNCTION_CALL(__current_task.cb_recv,this,Socket(s));
						if(__current_task_==NULL)continue;
					}
					if ((__current_task.events & ev.events & EPOLLOUT) || (hup && (__current_task.events & EPOLLOUT)))
					{
						if (hup)
						{
							__current_task.w_errno = 104;
						}
						else
						{
							if (__current_task.bits & 4)
							{
								int tmp = send(s, __current_task.w_buf.Data,
										__current_task.w_buf.Length, 0);
								if (tmp < 0)
								{
									if (!__current_task.w_length) __current_task.w_errno
											= errno;
								}
								else if (tmp > 0)
								{
									__current_task.w_length += tmp;
									if (tmp < __current_task.w_buf.Length)
									{
										__current_task.new_events |= EPOLLOUT;
										__current_task.w_buf
												= __current_task.w_buf.SubBuffer(
														tmp,
														__current_task.w_buf.Length
																- tmp);
										goto asdf;
									}
								}
							}
						}dbgprint("calling cb_send=" << __current_task.cb_send.f);
						FUNCTION_CALL(__current_task.cb_send,this,Socket(s));
						if(__current_task_==NULL)continue;
					}
					asdf: dbgprint("i.new_events=" << __current_task.new_events);
					if (__current_task.new_events == 0)
					{
						epoll_ctl(this->epollfd, EPOLL_CTL_DEL, s, NULL);
						this->info.erase(s);
						dbgprint("removing fd " << s);
						continue;
					}
					else if (__current_task.new_events != __current_task.events)
					{
						__current_task.events = __current_task.new_events;
						epoll_event ev2;
						ev2.data.fd = s;
						ev2.events = __current_task.events;
						epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s, &ev2);
						dbgprint("epoll events set to: " << ev2.events);
					}
					//this->info[s] = __current_task;
				}
				tmp_erased.clear();
				//__current_task.s = -1;
				__current_task_=NULL;
			}
		}
		void SocketManager::BeginRecv(Socket s, Buffer buf, Callback cb,
				bool fill)
		{
			if (__current_task_!=NULL && s._s == __current_task_->s)
			{
				taskInfo& __current_task=*__current_task_;
				if (__current_task.new_events & EPOLLIN)
				{
					throw Exception(s._s, "socket is already receiving");
				}
				__current_task.new_events |= EPOLLIN;
				__current_task.cb_recv = cb;
				fill ? (__current_task.bits |= 2) : (__current_task.bits &= ~2);
				__current_task.r_buf = buf;
				__current_task.r_length = 0;
				__current_task.r_errno = 0;
				return;
			}
			taskInfo& i = this->info[s._s];
			if (i.bits)
			{
				if (i.new_events & EPOLLIN)
				{
					throw Exception(s._s, "socket is already receiving");
				}
				i.events |= EPOLLIN;
				i.new_events = i.events;
				epoll_event ev;
				ev.data.fd = s._s;
				ev.events = i.events;
				i.cb_recv = cb;
				fill ? (i.bits |= 2) : (i.bits &= ~2);
				i.r_buf = buf;
				i.r_length = 0;
				i.r_errno = 0;
				epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s._s, &ev);
			}
			else
			{
				i.bits = 1;
				i.s = s._s;
				i.events = EPOLLIN;
				i.new_events = EPOLLIN;
				i.cb_recv = cb;
				fill ? (i.bits |= 2) : (i.bits &= ~2);
				i.r_buf = buf;
				i.r_length = 0;
				i.r_errno = 0;
				//this->info[s._s] = i;
				epoll_event ev;
				ev.data.fd = s._s;
				ev.events = i.events;
				if(epoll_ctl(this->epollfd, EPOLL_CTL_ADD, s._s, &ev)!=0)throw Exception(errno);
			}
			//this->info[s._s] = i;
		}
		int SocketManager::EndRecv(Socket s)
		{
			taskInfo *i = ((__current_task_!=NULL && s._s == __current_task_->s) ? __current_task_
					: &this->info[s._s]);
			//if(!i.initialized)throw Exception("");
			if (i->r_errno != 0) throw Exception(i->r_errno);
			if (i->bits & 2) return i->r_length;
			else
			{
				int tmp = recv(s._s, i->r_buf.Data, i->r_buf.Length, 0);
				if (tmp < 0) throw Exception(errno);
				else
					return tmp;
			}
		}

		void SocketManager::BeginSend(Socket s, Buffer buf, Callback cb,
				bool fill)
		{
			if (__current_task_!=NULL && s._s == __current_task_->s)
			{
				taskInfo& __current_task=*__current_task_;
				if (__current_task.new_events & EPOLLOUT)
				{
					throw Exception(s._s, "socket is already sending");
				}
				__current_task.new_events |= EPOLLOUT;
				__current_task.cb_send = cb;
				fill ? (__current_task.bits |= 4) : (__current_task.bits &= ~4);
				__current_task.w_buf = buf;
				__current_task.w_length = 0;
				__current_task.w_errno = 0;
				return;
			}
			taskInfo& i = this->info[s._s];
			if (i.bits)
			{
				if (i.new_events & EPOLLOUT)
				{
					throw Exception(s._s, "socket is already sending");
				}
				i.events |= EPOLLOUT;
				i.new_events = i.events;
				epoll_event ev;
				ev.data.fd = s._s;
				ev.events = i.events;
				i.cb_send = cb;
				fill ? (i.bits |= 4) : (i.bits &= ~4);
				i.w_buf = buf;
				i.w_length = 0;
				i.w_errno = 0;
				epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s._s, &ev);
			}
			else
			{
				i.bits = 1;
				i.s = s._s;
				i.events = EPOLLOUT;
				i.new_events = EPOLLOUT;
				i.cb_send = cb;
				fill ? (i.bits |= 4) : (i.bits &= ~4);
				i.w_buf = buf;
				i.w_length = 0;
				i.w_errno = 0;
				//this->info[s._s] = i;
				epoll_event ev;
				ev.data.fd = s._s;
				ev.events = i.events;
				if(epoll_ctl(this->epollfd, EPOLL_CTL_ADD, s._s, &ev)!=0)throw Exception(errno);
			}
			//this->info[s._s] = i;
		}
		int SocketManager::EndSend(Socket s)
		{
			taskInfo *i = ((__current_task_!=NULL && s._s == __current_task_->s) ? __current_task_
								: &this->info[s._s]);
			if (i->w_errno != 0) throw Exception(i->w_errno);
			//if(!i.initialized)throw Exception("");
			if (i->bits & 4) return i->w_length;
			else
			{
				int tmp = send(s._s, i->w_buf.Data, i->w_buf.Length, 0);
				if (tmp < 0) throw Exception(errno);
				else
					return tmp;
			}
		}
		void SocketManager::BeginAccept(Socket s, Callback cb)
		{
			BeginRecv(s, Buffer(), cb);
		}
		Socket SocketManager::EndAccept(Socket s, int flags)
		{
			return s.Accept(flags);
		}
		void SocketManager::BeginConnect(Socket s, EndPoint* ep, Callback cb)
		{
			s.SetFlags(s.GetFlags() | O_NONBLOCK);
			s.Connect(ep);
			BeginSend(s, Buffer(), cb, false);
		}
		void SocketManager::EndConnect(Socket s)
		{
			taskInfo *i = ((__current_task_!=NULL && s._s == __current_task_->s) ? __current_task_
								: &this->info[s._s]);
			if (i->w_errno != 0) throw Exception(i->w_errno);
		}
		void SocketManager::Cancel(Socket s)
		{
			dbgprint("removing fd " << s._s);
			epoll_ctl(this->epollfd, EPOLL_CTL_DEL, s._s, NULL);
			if (__current_task_!=NULL && s._s == __current_task_->s)
			{
				//__current_task_->events = 0;
				//__current_task_->new_events = 0;
				__current_task_=NULL;
			}
			this->info.erase(s._s);
			tmp_erased[s._s]=true;
		}
		void SocketManager::CancelRead(Socket s)
		{
			taskInfo& i = ((__current_task_!=NULL && s._s == __current_task_->s) ? *__current_task_
											: this->info[s._s]);
			if (i.bits)
			{
				if (__current_task_!=NULL && s._s == __current_task_->s)
				{
					i.new_events &= ~EPOLLIN;
				}
				else
				{
					i.events &= ~EPOLLIN;
					if (i.events)
					{
						i.new_events = i.events;
						epoll_event ev;
						ev.data.fd = s._s;
						ev.events = i.events;
						epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s._s, &ev);
					}
					else
					{
						Cancel(s);
					}
				}
			}
			//s._s == __current_task.s ? __current_task = i : this->info[s._s] = i;
		}
		void SocketManager::CancelWrite(Socket s)
		{
			taskInfo& i = ((__current_task_!=NULL && s._s == __current_task_->s) ? *__current_task_
							: this->info[s._s]);
			if (i.bits)
			{
				if (__current_task_!=NULL && s._s == __current_task_->s)
				{
					i.new_events &= ~EPOLLOUT;
				}
				else
				{
					i.events &= ~EPOLLOUT;
					if (i.events)
					{
						i.new_events = i.events;
						epoll_event ev;
						ev.data.fd = s._s;
						ev.events = i.events;
						epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s._s, &ev);
					}
					else
					{
						Cancel(s);
					}
				}
			}
			//s._s == __current_task.s ? __current_task = i : this->info[s._s] = i;
		}
		void SOCKS5::socks_connect(Stream* s, EndPoint* ep, Callback cb, Callback sent_cb)
		{
			StringBuilder* sb=new StringBuilder(32);
			char c[]="\x05\x01\x00\x05\x01\x00";
			sb->Append(Buffer((Byte*)c,6));
			int port;
			tmp* tmp1=new tmp();
			//tmp1->state=state;
			tmp1->sb=sb;
			tmp1->cb=cb;
			tmp1->sent_cb=sent_cb;
			try
			{
				switch(ep->AddressFamily)
				{
					case AF_INET:
					{
						sb->Append("\x01");
						port=dynamic_cast<IPEndPoint*>(ep)->Port;
						tmp1->addrlen=4;
						sockaddr_in addr;
						ep->GetSockAddr((sockaddr*)&addr);
						sb->Append((char*)&(addr.sin_addr.s_addr),4);
						break;
					}
					case AF_INET6:
					{
						sb->Append("\x04");
						port=dynamic_cast<IPv6EndPoint*>(ep)->Port;
						tmp1->addrlen=16;
						sockaddr_in6 addr;
						ep->GetSockAddr((sockaddr*)&addr);
						sb->Append((char*)&(addr.sin6_addr.__in6_u.__u6_addr8),16);
						break;
					}
					default:
						throw Exception("SOCKS does not support the requested address family");
				}
				port=htons(port);
				sb->Append((char*)&port,2);

				STRING tmp=sb->ToString();
				s->BeginWrite(Buffer(tmp.c,tmp.length),Stream::Callback(cb1,tmp1));
			}
			catch(Exception& ex)
			{
				tmp1->ex=ex;
				FUNCTION_CALL(tmp1->cb,s,tmp1);
				delete tmp1->sb;
				delete tmp1;
			}
		}
		void SOCKS5::cb1(void* obj, Stream* s)
		{
			tmp* tmp1=(tmp*)obj;
			try
			{
				s->EndWrite();
				if(!FUNCTION_ISNULL(tmp1->sent_cb)) FUNCTION_CALL(tmp1->sent_cb,s,NULL);
				tmp1->b=Buffer(8+tmp1->addrlen);
				tmp1->br=0;
				dbgprint("========SOCKS request sent======");
				s->BeginRead(tmp1->b,Stream::Callback(cb2,tmp1));
			}
			catch(Exception& ex)
			{
				tmp1->ex=ex;
				FUNCTION_CALL(tmp1->cb,s,tmp1);
				delete tmp1->sb;
				//tmp1->b.Release();
				delete tmp1;
			}
		}
		void SOCKS5::cb2(void* obj, Stream* s)
		{
			tmp* tmp1=(tmp*)obj;
			try
			{
				int i=s->EndRead();
				if(i<=0)throw Exception("SOCKS error occured");
				dbgprint("========SOCKS response read====== "<<i<<" bytes received");
				tmp1->br+=i;
				if(tmp1->br>=tmp1->b.Length)
				{
					FUNCTION_CALL(tmp1->cb,s,tmp1);
					delete tmp1->sb;
					//tmp1->b.Release();
					delete tmp1;
				}
				else
				{
					//tmp1->b=tmp1->b.SubBuffer(tmp1->br,tmp1->b.length-tmp1->br,false);
					s->BeginRead(tmp1->b.SubBuffer(tmp1->br,tmp1->b.Length-tmp1->br),Stream::Callback(cb2,tmp1));
				}
			}
			catch(Exception& ex)
			{
				tmp1->ex=ex;
				FUNCTION_CALL(tmp1->cb,s,tmp1);
				delete tmp1->sb;
				//tmp1->b.Release();
				delete tmp1;
			}
		}
		void SOCKS5::socks_endconnect(void* v)
		{
			tmp* tmp1=(tmp*)v;
			if(tmp1->ex.Code!=0)throw tmp1->ex;
		}
	}
}
