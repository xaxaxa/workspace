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
/*
 * TODO:
 * 
 * * add support for eventfd()
 * * automatic AIO + eventfd for file async IO
 * * socket connect()
 * * timerfd() ?
 * * signalfd() ?
 * * verify that all system call return values are checked
 * * verify that all code paths that may encounter exceptions
 *   	release their resources (memory etc) properly when 
 * 		exceptions are encountered
 * * HTTP support
 * 
 * 
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>
#include <rgc.H>
#include <set>
#include <vector>
#include <sstream>
#include <sys/signalfd.h>
#include <sys/eventfd.h>
#include <delegate.H>

#ifndef __INCLUDED_CPOLL_H
#define __INCLUDED_CPOLL_H
/*
 * CPoll: low level c++ wrapper for poll() and epoll(); can be implemented on
 * other OS's using mechanisms such as overlapped I/O(windows), but currently
 * there is only a linux implementation; for other OS's patches are welcome ;)
 *
 * simple usage example:
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
 * nested example:
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

#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif

namespace CP
{
	using namespace std;
	typedef int32_t HANDLE; //file descriptor or handle
	typedef Delegate<void(int32_t)> Callback;
	typedef uint8_t event_t;
	class CPollException: public std::exception
	{
	public:
		string message;
		int32_t number;
		CPollException();
		CPollException(int32_t number);
		CPollException(string message, int32_t number = 0);
		~CPollException() throw ();
		const char* what() const throw ();
	};
	class AbortException: public std::exception
	{ //used for aborting the event loop
	public:
		AbortException();
		~AbortException() throw ();
		const char* what() const throw ();
	};
	class CancelException: public std::exception
	{ //used for cancelling a repeat[Read|Write] operation
	  //do NOT throw this exception on one-shot operations such as read()
	public:
		CancelException();
		~CancelException() throw ();
		const char* what() const throw ();
	};
	
	//==============================================================
	//==============================================================
	//=================NETWORK UTILITY CLASSES======================
	//=====================taken from cplib=========================
	//==============================================================
	//==============================================================
	
	struct IPAddress
	{
		in_addr a;
		inline IPAddress() {
		}
		inline IPAddress(const char* addr/*NOT hostname*/) {
			inet_pton(AF_INET, addr, &a.s_addr);
		}
		inline IPAddress(const in_addr& a) {
			this->a = a;
		}
		inline bool operator<(const IPAddress& other) const {
			return ntohl(a.s_addr) < ntohl(other.a.s_addr);
		}
		inline bool operator>(const IPAddress& other) const {
			return ntohl(a.s_addr) > ntohl(other.a.s_addr);
		}
		inline bool operator<=(const IPAddress& other) const {
			return ntohl(a.s_addr) <= ntohl(other.a.s_addr);
		}
		inline bool operator>=(const IPAddress& other) const {
			return ntohl(a.s_addr) >= ntohl(other.a.s_addr);
		}
		inline bool operator==(const IPAddress& other) const {
			return a.s_addr == other.a.s_addr;
		}
		inline IPAddress operator+(const IPAddress& other) const {
			return IPAddress( { htonl(ntohl(a.s_addr) + other.a.s_addr) });
		}
		inline IPAddress operator-(const IPAddress& other) const {
			return IPAddress( { htonl(ntohl(a.s_addr) - other.a.s_addr) });
		}
		inline IPAddress operator+(const in_addr_t& other) const {
			//WARN(1,a.s_addr << " " <<ntohl(a.s_addr));
			//cout << "a" << endl;
			return IPAddress( { htonl(ntohl(a.s_addr) + other) });
		}
		inline IPAddress operator-(const in_addr_t& other) const {
			return IPAddress( { htonl(ntohl(a.s_addr) - other) });
		}
		string toStr() const {
			char tmp[INET_ADDRSTRLEN];
			if (inet_ntop(AF_INET, &a, tmp, INET_ADDRSTRLEN) == NULL) throw CPollException();
			return string(tmp);
		}
	};
	struct IPv6Address
	{
		in6_addr a;
		inline IPv6Address() {
		}
		inline IPv6Address(const char* addr) {
			inet_pton(AF_INET6, addr, &a.__in6_u);
		}
		inline IPv6Address(const in6_addr& a) {
			this->a = a;
		}
		string toStr() const {
			char tmp[INET_ADDRSTRLEN];
			if (inet_ntop(AF_INET6, &a, tmp, INET6_ADDRSTRLEN) == NULL) throw CPollException();
			return string(tmp);
		}
	};
	class EndPoint: virtual public RGC::Object
	{
	public:
		int32_t addressFamily;
		virtual void getSockAddr(sockaddr* addr) const=0;
		virtual void setSockAddr(const sockaddr* addr)=0;
		virtual int32_t getSockAddrSize() const=0;
		static EndPoint* fromSockAddr(const sockaddr* addr);
		static EndPoint* create(int32_t addressFamily);
		virtual void clone(EndPoint& to) const=0;
		virtual ~EndPoint() {
		}
		static vector<RGC::Ref<EndPoint> > lookupHost(const char* hostname, const char* port,
				int32_t family = AF_UNSPEC, int32_t socktype = 0, int32_t proto = 0, int32_t flags = 0);
		//static EndPoint Resolve(
	};
	class IPEndPoint: public EndPoint
	{
	public:
		IPAddress address;
		in_port_t port;
		IPEndPoint();
		IPEndPoint(IPAddress address, in_port_t port);
		void set_addr(const sockaddr_in& addr);
		virtual void setSockAddr(const sockaddr* addr);
		IPEndPoint(const sockaddr_in& addr);
		virtual void getSockAddr(sockaddr* addr) const;
		virtual int32_t getSockAddrSize() const;
		virtual void clone(EndPoint& to) const;
		virtual string toStr() const;
	};
	class IPv6EndPoint: public EndPoint
	{
	public:
		IPv6Address address;
		in_port_t port;
		uint32_t flowInfo;
		uint32_t scopeID;
		IPv6EndPoint();
		IPv6EndPoint(IPv6Address address, in_port_t port);
		void set_addr(const sockaddr_in6& addr);
		IPv6EndPoint(const sockaddr_in6& addr);
		virtual void setSockAddr(const sockaddr* addr);
		virtual void getSockAddr(sockaddr* addr) const;
		virtual int32_t getSockAddrSize() const;
		virtual void clone(EndPoint& to) const;
		virtual string toStr() const;
	};
	class UNIXEndPoint: public EndPoint
	{
	public:
		string name;
		UNIXEndPoint();
		UNIXEndPoint(string name);
		void set_addr(const sockaddr_un& addr);
		UNIXEndPoint(const sockaddr_un& addr);
		virtual void setSockAddr(const sockaddr* addr);
		virtual void getSockAddr(sockaddr* addr) const;
		virtual int32_t getSockAddrSize() const;
		virtual void clone(EndPoint& to) const;
	};
	//===========================================================
	//======================ABSTRACT CLASSES=====================
	//===========================================================
	class Stream: virtual public RGC::Object
	{
	public:
		//sync
		virtual int32_t read(void* buf, int32_t len)=0;
		virtual int32_t write(const void* buf, int32_t len)=0;
		virtual int32_t writeAll(const void* buf, int32_t len)=0;

		//async
		virtual void read(void* buf, int32_t len, const Callback& cb, bool repeat = false)=0;
		inline void repeatRead(void* buf, int32_t len, const Callback& cb) {
			read(buf, len, cb, true);
		}
		virtual void write(const void* buf, int32_t len, const Callback& cb, bool repeat = false)=0;
		inline void repeatWrite(const void* buf, int32_t len, const Callback& cb) {
			write(buf, len, cb, true);
		}
		virtual void writeAll(const void* buf, int32_t len, const Callback& cb)=0;

		//=========misc===========
		//sync
		virtual void close()=0; //may block
		virtual void flush()=0;

		//async
		virtual void close(const Callback& cb)=0;
		virtual void flush(const Callback& cb)=0;

	};
	
	//===========================================================
	//======================UTILITY CLASSES======================
	//===========================================================

	//===========================================================
	//======================DATA STRUCTURES======================
	//===========================================================
	
	enum class Events
		: event_t
		{
			none = 0, in = 1, out = 2, other = 4, all = 7
	};

	enum class Operations
		: uint8_t
		{
			none = 0, read = 1, write, send, recv, accept, writeAll, sendAll, shutdown, connect, close,
		lastItem
	};

	struct EventHandlerData
	{
	public:
		Callback cb;
		union miscUnion
		{
			struct
			{
				void* buf;
				int32_t len;
				int32_t flags;
				int32_t len_done;
			} bufferIO;
			struct
			{
				int32_t how;
			} shutdown;
			struct
			{
				eventfd_t evt;
			} eventfd;
		} misc;
		Operations op;
		enum class States
			: uint8_t {
				invalid = 0, once = 1, repeat
		} state;
		EventHandlerData() :
				state(States::invalid) {
		}
	};
	struct EventData
	{
	public:
		bool hungUp, error;
	};
	
	static const int32_t numEvents = 2;
	//============================================================
	//============================================================
	//=======================MAIN CLASSES=========================
	//============================================================
	//============================================================
	
	class Handle: virtual public RGC::Object
	{
	public:
		//void* __private;
		HANDLE handle;
		bool _supportsEPoll;
		Handle();
		Handle(HANDLE handle);
		Delegate<void(Handle& h, Events old_events)> onEventsChange;
		Delegate<void(Handle& h)> onClose;

		virtual void init(HANDLE handle);
		virtual void deinit();
		///calls the callback associated with the event
		///only accepts one event
		virtual void dispatch(Events event, const EventData& evtd)=0;
		virtual Events getEvents()=0;
		virtual int32_t dispatchMultiple(Events events, const EventData& evtd);
		///get some events from the queue.
		virtual Events wait(EventData& evtd);
		virtual int32_t waitAndDispatch();
		virtual void loop();
		//void close();
		~Handle();
	};
	class File: public Handle, public Stream
	{
	public:
		EventHandlerData eventData[numEvents];
		bool* deletionFlag;
		Events preDispatchEvents;
		bool dispatching;

		File();
		File(HANDLE handle);
		virtual void init(HANDLE handle);
		Events _getEvents();
		///only accepts one event
		EventHandlerData* beginAddEvent(Events event);
		void endAddEvent(Events event, bool repeat = false);
		int32_t read(void* buf, int32_t len);
		int32_t write(const void* buf, int32_t len);
		int32_t writeAll(const void* buf, int32_t len);
		int32_t send(const void* buf, int32_t len, int32_t flags = 0);
		int32_t sendAll(const void* buf, int32_t len, int32_t flags = 0);
		int32_t recv(void* buf, int32_t len, int32_t flags = 0);
		virtual void doOperation(EventHandlerData& ed, const EventData& evtd,
				EventHandlerData::States oldstate);
		void dispatch(Events event, const EventData& evtd, bool& deletionFlag);
		void dispatch(Events event, const EventData& evtd) {
			bool d = false;
			deletionFlag = &d;
			dispatch(event, evtd, d);
			if (!d) deletionFlag = NULL;
		}
		int32_t dispatchMultiple(Events events, const EventData& evtd);
		inline void fillIOEventHandlerData(EventHandlerData* ed, void* buf, int32_t len,
				const Callback& cb, Events e, Operations op);
		void read(void* buf, int32_t len, const Callback& cb, bool repeat = false);
		inline void repeatRead(void* buf, int32_t len, const Callback& cb) {
			read(buf, len, cb, true);
		}
		void write(const void* buf, int32_t len, const Callback& cb, bool repeat = false);
		inline void repeatWrite(const void* buf, int32_t len, const Callback& cb) {
			write(buf, len, cb, true);
		}
		void writeAll(const void* buf, int32_t len, const Callback& cb);
		void recv(void* buf, int32_t len, int32_t flags, const Callback& cb, bool repeat = false);
		inline void repeatRecv(void* buf, int32_t len, int32_t flags, const Callback& cb) {
			recv(buf, len, flags, cb, true);
		}
		void send(const void* buf, int32_t len, int32_t flags, const Callback& cb,
				bool repeat = false);
		inline void repeatSend(const void* buf, int32_t len, int32_t flags, const Callback& cb) {
			send(buf, len, flags, cb, true);
		}
		void sendAll(const void* buf, int32_t len, int32_t flags, const Callback& cb);
		virtual Events getEvents() {
			return _getEvents();
		}
		~File();

		//=========misc===========
		//sync
		virtual void close(); //may block
		virtual void flush();

		//async
		virtual void close(const Callback& cb);
		virtual void flush(const Callback& cb);
	};
	class Socket: public File
	{
	public:
		union{
			DelegateBase<void(Socket*)> _acceptCB;
			DelegateBase<void(HANDLE)> _acceptHandleCB;
		};
		//Delegate<void(Socket*)> _acceptCB;
		int32_t addressFamily, type, protocol;
		//RGC::Ref<EndPoint> peer;
		Socket();
		Socket(HANDLE handle, int32_t d, int32_t t, int32_t p);
		Socket(int32_t d, int32_t t = SOCK_STREAM, int32_t p = 0);
		virtual void init(HANDLE handle, int32_t d, int32_t t, int32_t p);
		virtual void init(int32_t d, int32_t t, int32_t p);

		//the caller must release() or free() the returned object
		EndPoint* getLocalEndPoint();
		//the caller must release() or free() the returned object
		EndPoint* getRemoteEndPoint();
		virtual void doOperation(EventHandlerData& ed, const EventData& evtd,
				EventHandlerData::States oldstate);

		void connect(const sockaddr *addr, int32_t addr_size);
		void connect(const EndPoint &ep);
		void connect(const char* hostname, const char* port, int32_t family = AF_UNSPEC,
				int32_t socktype = 0, int32_t proto = 0, int32_t flags = 0);
		void bind(const sockaddr *addr, int32_t addr_size);
		void bind(const EndPoint &ep);
		void bind(const char* hostname, const char* port, int32_t family = AF_UNSPEC,
				int32_t socktype = 0, int32_t proto = 0, int32_t flags = 0);

		int32_t shutdown(int32_t how);
		void shutdown(int32_t how, const Callback& cb);
		void listen(int32_t backlog = 8);
		//the caller must release() or free() the returned object
		Socket* accept();
		HANDLE acceptHandle();

		void connect(const sockaddr *addr, int32_t addr_size, const Callback& cb);
		void connect(const EndPoint &ep, const Callback& cb);
		void connect(const char* hostname, const char* port, const Callback& cb, int32_t family =
				AF_UNSPEC, int32_t socktype = 0, int32_t proto = 0, int32_t flags = 0);

		//callback function must release() or free() the received object
		void accept(const Delegate<void(Socket*)>& cb, bool repeat = false);
		void acceptHandle(const Delegate<void(HANDLE)>& cb, bool repeat = false);
		inline void repeatAccept(const Delegate<void(Socket*)>& cb) {
			accept(cb, true);
		}
		inline void repeatAcceptHandle(const Delegate<void(HANDLE)>& cb) {
			acceptHandle(cb, true);
		}

	};
	class SignalFD: public Handle
	{
	public:
		static int32_t MAX_EVENTS;
		typedef struct signalfd_siginfo Signal;
		Delegate<void(Signal& sig)> callback;
		sigset_t mask;
		SignalFD(HANDLE handle, const sigset_t& mask);
		SignalFD(const sigset_t& mask, int32_t flags);
		virtual void dispatch(Events event, const EventData& evtd);
		virtual Events getEvents();
	};
	class EventFD: public File
	{
	public:
		Delegate<void(eventfd_t)> cb;
		EventFD(HANDLE handle);
		EventFD(uint32_t initval = 0, int32_t flags = 0);
		virtual void doOperation(EventHandlerData& ed, const EventData& evtd,
				EventHandlerData::States oldstate);
		eventfd_t getEvent();
		void getEvent(const Delegate<void(eventfd_t)>& cb, bool repeat = false);
		void repeatGetEvent(const Delegate<void(eventfd_t)>& cb) {
			getEvent(cb, true);
		}
		int32_t sendEvent(eventfd_t evt = 1);
		void sendEvent(eventfd_t evt, const Delegate<void(int32_t)>& cb);
	};
	//XXX: AIO support in the linux kernel is incomplete, and
	//	has many serious limitations such as:
	//	- files have to be opened as O_DIRECT
	//	- O_DIRECT implies that all I/O requests have to
	//		be block-aligned
	//Because of the said reasons, AIO will not be implemented
	//for now
	/*class AIO: public SignalFD
	 {

	 };*/

	//epoll wrapper
	class Poll: public Handle
	{
	public:
		static int32_t MAX_EVENTS;
		epoll_event* curEvents;
		int32_t curIndex, curLength;
		int32_t active;
		HANDLE cur_handle;
		//bool debug;
		//Events cur_last_events;
		//map<HANDLE, Ref<Handle> > handles;
		//set<Handle*> tmp_deleted;
		//bool has_deleted;
		Poll(HANDLE handle);
		Poll();
		void _applyHandle(Handle& h, Events old_e);
		int32_t _doDispatch(const epoll_event& event);
		int32_t _doEPoll(int32_t timeout);
		virtual void dispatch(Events event, const EventData& evtd);
		virtual Events getEvents();
		virtual int32_t waitAndDispatch();
		void fillEPollEvents(Handle& h, epoll_event& evt, Events e);

		inline void applyHandle(Handle& h, Events old_e);
		void add(Handle& h);
		void del(Handle& h);
	};

}

#endif
