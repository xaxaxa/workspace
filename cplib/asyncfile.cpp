#include "headers/asyncfile.hpp"
#include "headers/cplib.hpp"
#include <sys/epoll.h>
#include <errno.h>
//#define dbgprint(msg) cout << msg
/*
 *
 *
 *
 */
namespace xaxaxa
{
	namespace IO
	{
		const int pollcount = 32;
		FileStreamManager::FileStreamManager()
		{
			__current_task.s = -1;
			this->epollfd = epoll_create(pollcount);
			dbgprint("socketmanager " << this->epollfd << " created");
			if (epollfd < 0) ::perror("epoll_create");
		}
		FileStreamManager::~FileStreamManager()
		{
			dbgprint("socketmanager " << this->epollfd << " closed");
			close(this->epollfd);
		}
		void FileStreamManager::EventLoop()
		{
			epoll_event* events = new epoll_event[pollcount];
			while (1)
			{
				int tmp = epoll_wait(this->epollfd, events, pollcount, -1);
				//cout << tmp;
				if (tmp == -1) throw Exception(errno);
				for (int a = 0; a < tmp; a++)
				{
					epoll_event ev = events[a];
					FILEDES s = ev.data.fd;
					dbgprint("s=" << s);
					__current_task = this->info[s];
					/*if(!(__current_task.bits&1))
					 {
					 dbgprint(s << "not initialized");
					 this->info.erase(s);
					 continue;
					 }*/
					__current_task.new_events = __current_task.events
							& (~ev.events);
					dbgprint("__current_task.events=" << __current_task.events);dbgprint("ev.events=" << ev.events);
					if (__current_task.events & ev.events & EPOLLIN)
					{
						if (ev.events & EPOLLHUP)
						{
							__current_task.r_errno = 104;
						}
						else
						{
							if (__current_task.bits & 2)
							{
								int tmp = read(s, __current_task.r_buf.Data,
										__current_task.r_buf.Length);
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
						FUNCTION_CALL(__current_task.cb_recv,this,File(s));
					}
					if (__current_task.events & ev.events & EPOLLOUT)
					{
						if (ev.events & EPOLLHUP)
						{
							__current_task.w_errno = 104;
						}
						else
						{
							if (__current_task.bits & 4)
							{
								int tmp = write(s, __current_task.w_buf.Data,
										__current_task.w_buf.Length);
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
						FUNCTION_CALL(__current_task.cb_send,this,File(s));
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
					this->info[s] = __current_task;
				}
				__current_task.s = -1;
			}
		}
		void FileStreamManager::BeginRecv(File s, Buffer buf, Callback cb,
				bool fill)
		{
			if (s._f == __current_task.s)
			{
				if (__current_task.new_events & EPOLLIN)
				{
					throw Exception(s._f, "socket is already receiving");
				}
				__current_task.new_events |= EPOLLIN;
				__current_task.cb_recv = cb;
				fill ? (__current_task.bits |= 2) : (__current_task.bits &= ~2);
				__current_task.r_buf = buf;
				__current_task.r_length = 0;
				__current_task.r_errno = 0;
				return;
			}
			taskInfo i = this->info[s._f];
			if (i.bits)
			{
				if (i.new_events & EPOLLIN)
				{
					throw Exception(s._f, "socket is already receiving");
				}
				i.events |= EPOLLIN;
				i.new_events = i.events;
				epoll_event ev;
				ev.data.fd = s._f;
				ev.events = i.events;
				i.cb_recv = cb;
				fill ? (i.bits |= 2) : (i.bits &= ~2);
				i.r_buf = buf;
				i.r_length = 0;
				i.r_errno = 0;
				epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s._f, &ev);
			}
			else
			{
				i.bits = 1;
				i.s = s._f;
				i.events = EPOLLIN;
				i.new_events = EPOLLIN;
				i.cb_recv = cb;
				fill ? (i.bits |= 2) : (i.bits &= ~2);
				i.r_buf = buf;
				i.r_length = 0;
				i.r_errno = 0;
				this->info[s._f] = i;
				epoll_event ev;
				ev.data.fd = s._f;
				ev.events = i.events;
				if(epoll_ctl(this->epollfd, EPOLL_CTL_ADD, s._f, &ev)!=0)throw Exception(errno);
			}
			this->info[s._f] = i;
		}
		int FileStreamManager::EndRecv(File s)
		{
			taskInfo *i = (s._f == __current_task.s ? &__current_task
					: &this->info[s._f]);
			;
			//if(!i.initialized)throw Exception("");
			if (i->r_errno != 0) throw Exception(i->r_errno);
			if (i->bits & 2) return i->r_length;
			else
			{
				int tmp = read(s._f, i->r_buf.Data, i->r_buf.Length);
				if (tmp < 0) throw Exception(errno);
				else
					return tmp;
			}
		}

		void FileStreamManager::BeginSend(File s, Buffer buf, Callback cb,
				bool fill)
		{
			if (s._f == __current_task.s)
			{
				if (__current_task.new_events & EPOLLOUT)
				{
					throw Exception(s._f, "socket is already sending");
				}
				__current_task.new_events |= EPOLLOUT;
				__current_task.cb_send = cb;
				fill ? (__current_task.bits |= 4) : (__current_task.bits &= ~4);
				__current_task.w_buf = buf;
				__current_task.w_length = 0;
				__current_task.w_errno = 0;
				return;
			}
			taskInfo i = this->info[s._f];
			if (i.bits)
			{
				if (i.new_events & EPOLLOUT)
				{
					throw Exception(s._f, "socket is already sending");
				}
				i.events |= EPOLLOUT;
				i.new_events = i.events;
				epoll_event ev;
				ev.data.fd = s._f;
				ev.events = i.events;
				i.cb_send = cb;
				fill ? (i.bits |= 4) : (i.bits &= ~4);
				i.w_buf = buf;
				i.w_length = 0;
				i.w_errno = 0;
				epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s._f, &ev);
			}
			else
			{
				i.bits = 1;
				i.s = s._f;
				i.events = EPOLLOUT;
				i.new_events = EPOLLOUT;
				i.cb_send = cb;
				fill ? (i.bits |= 4) : (i.bits &= ~4);
				i.w_buf = buf;
				i.w_length = 0;
				i.w_errno = 0;
				this->info[s._f] = i;
				epoll_event ev;
				ev.data.fd = s._f;
				ev.events = i.events;
				if(epoll_ctl(this->epollfd, EPOLL_CTL_ADD, s._f, &ev)!=0)throw Exception(errno);
			}
			this->info[s._f] = i;
		}
		int FileStreamManager::EndSend(File s)
		{
			taskInfo *i = (s._f == __current_task.s ? &__current_task
					: &this->info[s._f]);
			if (i->w_errno != 0) throw Exception(i->w_errno);
			//if(!i.initialized)throw Exception("");
			if (i->bits & 4) return i->w_length;
			else
			{
				int tmp = write(s._f, i->w_buf.Data, i->w_buf.Length);
				if (tmp < 0) throw Exception(errno);
				else
					return tmp;
			}
		}
		void FileStreamManager::Cancel(File s)
		{
			dbgprint("removing fd " << s._f);
			this->info.erase(s._f);
			epoll_ctl(this->epollfd, EPOLL_CTL_DEL, s._f, NULL);
			if (s._f == __current_task.s)
			{
				__current_task.events = 0;
				__current_task.new_events = 0;
			}
		}
		void FileStreamManager::CancelRead(File s)
		{
			taskInfo i = (s._f == __current_task.s ? __current_task
					: this->info[s._f]);
			if (i.bits)
			{
				if (s._f == __current_task.s)
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
						ev.data.fd = s._f;
						ev.events = i.events;
						epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s._f, &ev);
					}
					else
					{
						Cancel(s);
					}
				}
			}
			s._f == __current_task.s ? __current_task = i : this->info[s._f] = i;
		}
		void FileStreamManager::CancelWrite(File s)
		{
			taskInfo i = (s._f == __current_task.s ? __current_task
					: this->info[s._f]);
			if (i.bits)
			{
				if (s._f == __current_task.s)
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
						ev.data.fd = s._f;
						ev.events = i.events;
						epoll_ctl(this->epollfd, EPOLL_CTL_MOD, s._f, &ev);
					}
					else
					{
						Cancel(s);
					}
				}
			}
			s._f == __current_task.s ? __current_task = i : this->info[s._f] = i;
		}

	}
}
