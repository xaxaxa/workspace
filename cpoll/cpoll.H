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
#include <vector>
#include <sstream>
#include <sys/signalfd.h>

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

namespace CP
{
	using namespace std;
	typedef int HANDLE; //file descriptor or handle
	typedef function<void(int)> Callback;
	typedef uint8_t event_t;
	class CPollException: public std::exception
	{
	public:
		string message;
		int number;
		CPollException();
		CPollException(int number);
		CPollException(string message, int number = 0);
		~CPollException() throw ();
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
		string tostr() const {
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
		string tostr() const {
			char tmp[INET_ADDRSTRLEN];
			if (inet_ntop(AF_INET6, &a, tmp, INET6_ADDRSTRLEN) == NULL) throw CPollException();
			return string(tmp);
		}
	};
	class EndPoint: public RGC::Object
	{
	public:
		int addressFamily;
		virtual void getSockAddr(sockaddr* addr) const=0;
		virtual void setSockAddr(const sockaddr* addr)=0;
		virtual int getSockAddrSize() const=0;
		static EndPoint* fromSockAddr(const sockaddr* addr);
		static EndPoint* create(int addressFamily);
		virtual void clone(EndPoint& to) const=0;
		virtual ~EndPoint() {
		}
		static vector<RGC::Ref<EndPoint> > lookupHost(const char* hostname, const char* port,
				int family = AF_UNSPEC, int socktype = 0, int proto = 0, int flags = 0);
		//static EndPoint Resolve(
	};
	class IPEndPoint: public EndPoint
	{
	public:
		IPAddress address;
		int port;
		IPEndPoint();
		IPEndPoint(IPAddress address, int port);
		void set_addr(const sockaddr_in& addr);
		virtual void setSockAddr(const sockaddr* addr);
		IPEndPoint(const sockaddr_in& addr);
		virtual void getSockAddr(sockaddr* addr) const;
		virtual int getSockAddrSize() const;
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
		virtual int getSockAddrSize() const;
		virtual void clone(EndPoint& to) const;
		virtual string tostr() const;
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
		virtual int getSockAddrSize() const;
		virtual void clone(EndPoint& to) const;
	};
	
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
			none = 0, read = 1, write, send, recv, accept, writeAll, sendAll, shutdown, connect
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
				int len;
				int flags;
				int len_done;
			} bufferIO;
			struct
			{
				int how;
			} shutdown;
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
	
	static const int numEvents = 3;
	//============================================================
	//============================================================
	//=======================MAIN CLASSES=========================
	//============================================================
	//============================================================
	
	class Handle: public RGC::Object
	{
	public:
		//void* __private;
		HANDLE handle;
		bool __deleted;
		Handle();
		Handle(HANDLE handle);
		function<void(Events old_events)> onEventsChange;

		virtual void init(HANDLE handle);
		///calls the callback associated with the event
		///only accepts one event
		virtual void dispatch(Events event, const EventData& evtd)=0;
		virtual Events getEvents()=0;
		virtual int dispatchMultiple(Events events, const EventData& evtd);
		///get some events from the queue.
		virtual Events wait(EventData& evtd);
		virtual int waitAndDispatch();
		virtual void loop();
		//void close();
		~Handle();
	};
	class File: public Handle
	{
	public:
		
		EventHandlerData eventData[numEvents];
		bool _supportsEPoll;
		File();
		File(HANDLE handle, bool supportsEPoll = false);
		virtual void init(HANDLE handle, bool supportsEPoll);
		Events _getEvents();
		///only accepts one event
		EventHandlerData* beginAddEvent(Events event);
		void endAddEvent(Events event, bool repeat = false);
		int read(void* buf, int len);
		int write(const void* buf, int len);
		int send(const void* buf, int len, int flags = 0);
		int recv(void* buf, int len, int flags = 0);
		virtual void doOperation(EventHandlerData& ed, const EventData& evtd);
		void dispatch(Events event, const EventData& evtd);
		inline void fillIOEventHandlerData(EventHandlerData* ed, void* buf, int len,
				const Callback& cb, Events e, Operations op);
		void read(void* buf, int len, const Callback& cb, bool repeat = false);
		inline void repeatRead(void* buf, int len, const Callback& cb) {
			read(buf, len, cb, true);
		}
		void write(const void* buf, int len, const Callback& cb, bool repeat = false);
		inline void repeatWrite(const void* buf, int len, const Callback& cb) {
			write(buf, len, cb, true);
		}
		void writeAll(const void* buf, int len, const Callback& cb);
		void recv(void* buf, int len, int flags, const Callback& cb, bool repeat = false);
		inline void repeatRecv(void* buf, int len, int flags, const Callback& cb) {
			recv(buf, len, flags, cb, true);
		}
		void send(const void* buf, int len, int flags, const Callback& cb, bool repeat = false);
		inline void repeatSend(const void* buf, int len, int flags, const Callback& cb) {
			send(buf, len, flags, cb, true);
		}
		void sendAll(const void* buf, int len, int flags, const Callback& cb);
		virtual Events getEvents() {
			return _getEvents();
		}
	};
	class Socket: public File
	{
	public:
		int addressFamily, type, protocol;
		//RGC::Ref<EndPoint> peer;
		Socket();
		Socket(HANDLE handle, int d, int t, int p);
		Socket(int d, int t = SOCK_STREAM, int p = 0);
		virtual void init(HANDLE handle, int d, int t, int p);
		virtual void init(int d, int t, int p);

		//the caller must release() or free() the returned object
		EndPoint* getLocalEndPoint();
		//the caller must release() or free() the returned object
		EndPoint* getRemoteEndPoint();
		virtual void doOperation(EventHandlerData& ed, const EventData& evtd);

		void connect(const sockaddr *addr, int addr_size);
		void connect(const EndPoint &ep);
		void connect(const char* hostname, const char* port, int family = AF_UNSPEC, int socktype =
				0, int proto = 0, int flags = 0);
		void bind(const sockaddr *addr, int addr_size);
		void bind(const EndPoint &ep);

		int shutdown(int how);
		void shutdown(int how, const Callback& cb);
		void listen(int backlog = 8);
		//the caller must release() or free() the returned object
		Socket* accept();

		void connect(const sockaddr *addr, int addr_size, const Callback& cb);
		void connect(const EndPoint &ep, const Callback& cb);
		void connect(const char* hostname, const char* port, const Callback& cb, int family =
				AF_UNSPEC, int socktype = 0, int proto = 0, int flags = 0);

		//callback function must release() or free() the received object
		void accept(const function<void(Socket*)>& cb, bool repeat = false);
		inline void repeatAccept(const function<void(Socket*)>& cb) {
			accept(cb, true);
		}

	};
	class SignalFD: public Handle
	{
		static int MAX_EVENTS;
		typedef struct signalfd_siginfo Signal;
		function<void(Signal& sig)> callback;
		sigset_t mask;
		SignalFD(HANDLE handle, const sigset_t& mask);
		SignalFD(const sigset_t& mask, int flags);
		virtual void dispatch(Events event, const EventData& evtd);
		virtual Events getEvents();
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
		static int MAX_EVENTS;
		int active;
		HANDLE cur_handle;
		Events cur_last_events;
		//map<HANDLE, Ref<Handle> > handles;
		vector<Handle*> tmp_deleted;
		Poll(HANDLE handle);
		Poll();
		void _applyHandle(Handle& h, Events old_e);
		int _doDispatch(const epoll_event& event);
		int _doEPoll(int timeout);
		virtual void dispatch(Events event, const EventData& evtd);
		virtual Events getEvents();
		virtual int waitAndDispatch();
		void fillEPollEvents(Handle& h, epoll_event& evt, Events e);

		inline void applyHandle(Handle& h, Events old_e);
		void add(Handle& h);
		void del(Handle& h);
	};

}

#endif
