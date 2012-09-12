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
#include <functional>
#include <string>
#include <exception>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>
#include <rgc.H>
/*
 * CPoll: low level c++ wrapper for poll() and epoll(); can be emulated on other OS's
 * 
 * usage example:
 * 
 * char buf[4096];
 * char buf2[4096];
 * File f(1); //stdout
 * File f2(2); //stderr
 * f.read(buf, 4096, [](){cout << "read1 done" << endl;});
 * f2.read(buf2, 4096, [](){cout << "read2 done" << endl;});
 * 
 * Poll p;
 * p.add(f);
 * p.add(f2);
 * p.loop(); //epoll is used
 * 
 * 
 * 
 * 
 * complex example:
 * 
 * File f(0); //stdin
 * char buf[4096];
 * f.read([](){}, buf,4096);
 * Poll poll;
 * poll.add(f);
 * Poll poll2;
 * poll2.add(poll);
 * poll2.loop();
 */

namespace CP
{
	using namespace std;
	typedef int HANDLE; //file descriptor or handle
	typedef function<void(int)> Callback;
	typedef uint8_t event_t;
	enum class Events: event_t
	{
		none=0,
		in=1,
		out=2,
		other=4,
		all=7
	};
	
	enum class Operations: uint8_t
	{
		none=0,
		read=1,
		write,
		send,
		recv,
		accept
	};
	class CPollException: public std::exception
	{
	public:
		string message;
		int number;
		CPollException(): message(strerror(errno)),number(errno)
		{}
		CPollException(int number): message(strerror(number)), number(number)
		{
		}
		CPollException(string message, int number=0): message(message), number(number)
		{
		}
		~CPollException() throw()
		{
		}
		const char* what() const throw()
		{
			return message.c_str();
		}
	};
	struct EventData
	{
	public:
		Callback cb;
		union miscUnion
		{
			struct {
				void* buf;
				int len;
				int flags;
			} bufferIO;
			struct {
				
			} socketAccept;
		} misc;
		Operations op;
		//bool valid;
		enum class States: uint8_t {
			invalid=0,
			once=1,
			repeat
		} state;
		EventData(): state(States::invalid) {}
	};
	static inline int eventToIndex(Events event)
	{
		if(event==Events::in) return 0;
		if(event==Events::out) return 1;
		if(event==Events::other) return 2;
	}
	static inline short eventToPoll(Events event)
	{
		switch(event) {
			case Events::in:
				return POLLIN|POLLPRI|POLLRDHUP;
			case Events::out:
				return POLLOUT|POLLHUP|POLLERR|POLLNVAL;
		}
		return 0;
	}
	static inline short eventsToPoll(Events events)
	{
		short ret=0;
		if((event_t)events & (event_t)Events::in)
			ret|=POLLIN|POLLPRI|POLLRDHUP;
		if((event_t)events & (event_t)Events::out)
			ret|=POLLOUT|POLLHUP|POLLERR|POLLNVAL;
		return ret;
	}
	static inline Events pollToEvents(short events)
	{
		event_t ret=0;
		if(events & (POLLIN|POLLPRI|POLLRDHUP))
			ret|=(event_t)Events::in;
		if(events & (POLLOUT|POLLHUP|POLLERR|POLLNVAL))
			ret|=(event_t)Events::out;
		return (Events)ret;
	}
	static inline uint32_t eventToEPoll(Events event)
	{
		switch(event) {
			case Events::in:
				return EPOLLIN|EPOLLPRI|EPOLLRDHUP;
			case Events::out:
				return EPOLLOUT|EPOLLHUP|EPOLLERR;
		}
		return 0;
	}
	static inline uint32_t eventsToEPoll(Events events)
	{
		uint32_t ret=0;
		if((event_t)events & (event_t)Events::in)
			ret|=EPOLLIN|EPOLLPRI|EPOLLRDHUP;
		if((event_t)events & (event_t)Events::out)
			ret|=EPOLLOUT|EPOLLHUP|EPOLLERR;
		return ret;
	}
	static inline Events ePollToEvents(uint32_t events)
	{
		event_t ret=0;
		if(events & (EPOLLIN|EPOLLPRI|EPOLLRDHUP))
			ret|=(event_t)Events::in;
		if(events & (EPOLLOUT|EPOLLHUP|EPOLLERR))
			ret|=(event_t)Events::out;
		return (Events)ret;
	}
	static const int numEvents=3;
	static inline Events indexToEvent(int index)
	{
		return (Events)(1 << index);
	}
	class Handle: public RGC::Object
	{
	public:
		//void* __private;
		HANDLE handle;
		Handle(HANDLE handle): handle(handle) {}
		function<void(Events old_events)> eventsChange;
		///calls the callback associated with the event
		///only accepts one event
		virtual void dispatch(Events event)=0;
		virtual Events getEvents()=0;
		virtual int dispatchMultiple(Events events)
		{
			//cout << (int)events << endl;
			int ret=0;
			for(int i=0;i<numEvents;i++) {
				Events e=indexToEvent(i);
				//cout << (int)e << " " << (((event_t)e)&((event_t)events)) << endl;
				if((((event_t)e)&((event_t)events)) == (event_t)e) {
					dispatch(e);
					ret++;
				}
			}
			//cout << ret << endl;
			return ret;
		}
		
		///get some events from the queue.
		virtual Events wait()
		{	//since this is single-file, poll() will be used.
			//Events events=Events::none;
			Events w=getEvents();
			pollfd pfd;
			pfd.fd=handle;
			pfd.events=eventsToPoll(w);
			if(pfd.events==0) return Events::none;
			poll(&pfd,1,-1);
			/*for(int i=0;i<numEvents;i++) {
				Events e=indexToEvent(i);
				short p=eventToPoll(e);
				if(p&pfd.revents!=0) events=(Events)((event_t)events | (event_t)e);
			}*/
			return pollToEvents(pfd.revents);
		}
		virtual int waitAndDispatch()
		{
			Events e=wait();
			return dispatchMultiple(e);
		}
		virtual void loop()
		{
			while(waitAndDispatch()>0);
		}
		
	};
	class File: public Handle
	{
	public:
		File(HANDLE handle): Handle(handle){}
		EventData eventData[numEvents];
		Events _getEvents()
		{
			Events e=Events::none;
			for(int i=0;i<numEvents;i++)
				if(eventData[i].state!=EventData::States::invalid)
					e=(Events)(((event_t)e)|((event_t)indexToEvent(i)));
			return e;
		}
		///only accepts one event
		EventData* beginAddEvent(Events event)
		{
			EventData *ed=&eventData[eventToIndex(event)];
			if(ed->state!=EventData::States::invalid)
				throw CPollException("Already listening for the specified event on the specified file. "
				"For example, you may not read() and recv() on one socket at the same time.");
			return ed;
		}
		void endAddEvent(Events event, bool repeat=false)
		{
			Events old_events=_getEvents();
			eventData[eventToIndex(event)].state=
				repeat ? (EventData::States::repeat) : (EventData::States::once);
			if(eventsChange!=nullptr)eventsChange(old_events);
		}
		int read(void* buf, int len) {
			return ::read(handle, buf, len);
		}
		int write(void* buf, int len) {
			return ::write(handle, buf, len);
		}
		int send(void* buf, int len) {
			return ::send(handle, buf, len, 0);
		}
		int recv(void* buf, int len) {
			return ::recv(handle, buf, len, 0);
		}
		void dispatch(Events event)
		{
			//cout << (int)event << " dispatched" << endl;
			EventData& ed=eventData[eventToIndex(event)];
			Operations op=ed.op;
			switch(op)
			{
				case Operations::read:
					ed.cb(read(ed.misc.bufferIO.buf, ed.misc.bufferIO.len));
					break;
				case Operations::write:
					ed.cb(write(ed.misc.bufferIO.buf, ed.misc.bufferIO.len));
					break;
				case Operations::recv:
					ed.cb(recv(ed.misc.bufferIO.buf, ed.misc.bufferIO.len));
					break;
				case Operations::send:
					ed.cb(send(ed.misc.bufferIO.buf, ed.misc.bufferIO.len));
					break;
				case Operations::accept:
					break;
					
			}
			//cout << "clear event" << endl;
			Events old_events=_getEvents();
			if(eventData[eventToIndex(event)].state==EventData::States::once)
				eventData[eventToIndex(event)].state=EventData::States::invalid;
			if(eventsChange!=nullptr)eventsChange(old_events);
		}
		inline void fillIOEventData(EventData* ed, void* buf, int len, const Callback& cb
			, Events e, Operations op)
		{
			ed->cb=cb;
			ed->misc.bufferIO.buf=buf;
			ed->misc.bufferIO.len=len;
			ed->op=op;
		}
		int read(void* buf, int len, const Callback& cb) {
			static const Events e=Events::in;
			EventData* ed=beginAddEvent(e);
			fillIOEventData(ed, buf, len, cb, e, Operations::read);
			endAddEvent(e);
		}
		int write(const void* buf, int len, const Callback& cb) {
			static const Events e=Events::out;
			EventData* ed=beginAddEvent(e);
			fillIOEventData(ed, (void*)buf, len, cb, e, Operations::write);
			endAddEvent(e);
		}
		int recv(void* buf, int len, int flags, const Callback& cb) {
			static const Events e=Events::in;
			EventData* ed=beginAddEvent(e);
			fillIOEventData(ed, buf, len, cb, e, Operations::recv);
			ed->misc.bufferIO.flags=flags;
			endAddEvent(e);
		}
		int send(const void* buf, int len, int flags, const Callback& cb) {
			static const Events e=Events::out;
			EventData* ed=beginAddEvent(e);
			fillIOEventData(ed, (void*)buf, len, cb, e, Operations::send);
			ed->misc.bufferIO.flags=flags;
			endAddEvent(e);
		}
		virtual Events getEvents()
		{
			return _getEvents();
		}
		/*int endRead(void* eventArgs) {
			EventData ed=(EventData*)eventArgs;
			return read(ed->misc->bufferIO->buf, ed->misc->bufferIO->len);
		}
		int endWrite(void* eventArgs) {
			EventData ed=(EventData*)eventArgs;
			return write(ed->misc->bufferIO->buf, ed->misc->bufferIO->len);
		}*/
		
	};

	//epoll wrapper
	class Poll: public Handle
	{
	public:
		static const int EPOLL_MAX_EVENTS=32;
		int active;
		HANDLE cur_handle;
		Events cur_last_events;
		//map<HANDLE, Ref<Handle> > handles;
		Poll(HANDLE handle): Handle(handle), active(0){}
		Poll(): Handle(epoll_create1(0)), active(0){}
		void _applyHandle(Handle& h, Events old_e)
		{
			Events new_e=h.getEvents();
			if(new_e==old_e) return;
			epoll_event evt;
			if(old_e==Events::none) {
				fillEPollEvents(h, evt, new_e);
				//cout << "added " << h.handle << endl;
				epoll_ctl(this->handle, EPOLL_CTL_ADD, h.handle, &evt);
				active++;
			} else if(new_e==Events::none) {
				//cout << "deleted " << h.handle << endl;
				epoll_ctl(this->handle, EPOLL_CTL_DEL, h.handle, &evt);
				active--;
			} else {
				fillEPollEvents(h, evt, new_e);
				//cout << "modified " << h.handle << endl;
				epoll_ctl(this->handle, EPOLL_CTL_MOD, h.handle, &evt);
			}
		}
		int _doDispatch(const epoll_event& event)
		{
			int ret=0;
			event_t evt=(event_t)ePollToEvents(event.events);
			Handle* h=(Handle*)event.data.ptr;
			/*for(int i=0;i<numEvents;i++) {
				Events e=indexToEvent(i);
				uint32_t ep=eventToEPoll(e);
				if(event.events&ep != 0)
					evt |= (event_t)e;
			}*/
			//cout << "handle fd: " << h->handle << endl;
			//cout << "getevents: " << (int)(h->getEvents()) << endl;
			cur_handle=h->handle;
			cur_last_events=h->getEvents();
			ret=h->dispatchMultiple((Events)evt);
			Events new_events=h->getEvents();
			//cout << (int)new_events << " " << (int)cur_last_events << endl;
			if(new_events!=cur_last_events)
				_applyHandle(*h, cur_last_events);
			return ret;
		}
		int _doEPoll(int timeout)
		{
			if(active<=0)return 0;
			int ret=0;
			epoll_event evts[EPOLL_MAX_EVENTS];
		retry:
			int n=epoll_wait(handle,evts,EPOLL_MAX_EVENTS,timeout);
			if(n<0)
				if(errno==EINTR) goto retry;
				else throw CPollException();
			for(int i=0;i<n;i++)
				ret+=_doDispatch(evts[i]);
			return ret;
		}
		virtual void dispatch(Events event) {
			//throw CPollException("Poll::dispatch() not implemented");
			_doEPoll(0);
		}
		virtual Events getEvents() {
			//throw CPollException("Poll::getEvents() not implemented");
			return active?(Events::all):(Events::none);
		}
		virtual int waitAndDispatch()
		{
			return _doEPoll(-1);
		}
		void fillEPollEvents(Handle& h, epoll_event& evt, Events e)
		{
			evt.events=eventsToEPoll(e);
			evt.data.ptr=&h;
		}
		
		inline void applyHandle(Handle& h, Events old_e)
		{
			if(h.handle==cur_handle) return;
			_applyHandle(h, old_e);
		}
		void add(Handle& h)
		{
			h.retain();
			h.eventsChange=[this,&h](Events old_events){this->applyHandle(h,old_events);};
			_applyHandle(h, Events::none);
		}
		void del(Handle& h)
		{
			h.release();
			if(h.getEvents()!=Events::none)
				epoll_ctl(this->handle, EPOLL_CTL_DEL, h.handle, (epoll_event*)1);
			active--;
		}
	};

}
