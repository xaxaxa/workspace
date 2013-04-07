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
			return IPAddress( { htonl(ntohl(a.s_addr) + ntohl(other.a.s_addr)) });
		}
		inline IPAddress operator-(const IPAddress& other) const {
			return IPAddress( { htonl(ntohl(a.s_addr) - ntohl(other.a.s_addr)) });
		}
		inline IPAddress operator+(int i) const {
			//WARN(1,a.s_addr << " " <<ntohl(a.s_addr));
			//cout << "a" << endl;
			return IPAddress( { htonl(ntohl(a.s_addr) + i) });
		}
		inline IPAddress operator-(int i) const {
			return IPAddress( { htonl(ntohl(a.s_addr) - i) });
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
		static int getSize(int32_t addressFamily);
		static EndPoint* construct(void* mem, int32_t addressFamily);
		virtual void clone(EndPoint& to) const=0;
		virtual ~EndPoint() {
		}
		static vector<RGC::Ref<EndPoint> > lookupHost(const char* hostname, const char* port,
				int32_t family = AF_UNSPEC, int32_t socktype = 0, int32_t proto = 0, int32_t flags = 0);
		//static EndPoint Resolve(
		virtual string toStr() const=0;
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
		virtual string toStr() const;
	};
	//===========================================================
	//======================ABSTRACT CLASSES=====================
	//===========================================================
	class BufferedOutput: virtual public RGC::Object
	{
	public:
		uint8_t* buffer;
		int bufferPos;
		int bufferSize;
		BufferedOutput() {
		}
		BufferedOutput(uint8_t* buffer, int bufferPos, int bufferSize) :
				buffer(buffer), bufferPos(bufferPos), bufferSize(bufferSize) {
		}
		//flushBuffer() should flush current buffer to stream, reallocate the buffer,
		//and reset buffer, bufferPos and/or bufferSize accordingly (see below); the function
		//must reserve at least minBufferAllocation bytes of space in the write buffer
		virtual void flushBuffer(int minBufferAllocation)=0;
		void flush() {
			flushBuffer(0);
		}
	};

	class Stream: virtual public RGC::Object
	{
	public:
		//sync
		virtual int32_t read(void* buf, int32_t len)=0; //required
		virtual int32_t readAll(void* buf, int32_t len) { //optional
			int off = 0;
			while (off < len) {
				int tmp = read((uint8_t*) buf + off, len - off);
				if (tmp <= 0) return off;
				off += tmp;
			}
			return off;
		}
		virtual int32_t write(const void* buf, int32_t len)=0; //required
		virtual int32_t writeAll(const void* buf, int32_t len) { //optional
			int off = 0;
			while (off < len) {
				int tmp = write((uint8_t*) buf + off, len - off);
				if (tmp <= 0) return off;
				off += tmp;
			}
			return off;
		}

		//async
		//all new subclasses of Stream now should only need to implement the hybrid[Read|Write] functions
		//to provide async behavior; apps using [read|write](..., cb) will still work through the
		//compatibility wrappers

		//returns -2 to indicate that the operation is scheduled to be completed asynchronously; otherwise
		//the request has been completed synchronously

		//UPDATE: hybrid[Read|Write] has been canceled because it is extremely hard for users to design applications
		// to deal with 2 programming models at once and it is very error-prone
		/*virtual int32_t hybridRead(void* buf, int32_t len, const Callback& cb, bool repeat = false) { //optional (required for async)
		 }
		 virtual int32_t hybridWrite(const void* buf, int32_t len, const Callback& cb, bool repeat =
		 false) { //optional (required for async)
		 }*/
		virtual void read(void* buf, int32_t len, const Callback& cb, bool repeat = false)=0;
		inline void repeatRead(void* buf, int32_t len, const Callback& cb) {
			read(buf, len, cb, true);
		}
		virtual void readAll(void* buf, int32_t len, const Callback& cb)=0;

		virtual void write(const void* buf, int32_t len, const Callback& cb, bool repeat = false)=0;
		inline void repeatWrite(const void* buf, int32_t len, const Callback& cb) {
			write(buf, len, cb, true);
		}
		virtual void writeAll(const void* buf, int32_t len, const Callback& cb)=0;
		//only for async read/write operations
		virtual void cancelRead()=0;
		virtual void cancelWrite()=0;

		//=========misc===========
		//sync
		virtual void close()=0; //may block
		virtual void flush()=0;

		//async
		virtual void close(const Callback& cb)=0;
		virtual void flush(const Callback& cb)=0;

		//only for streams with a read buffer; other streams should just return 0
		//calling this function should "consume" that buffer, but should not perform
		//further reading operations; this function should NOT block
		//returns length of buffer; address of buffer is put in buf
		//if maxlen==-1, consume entire contiguous buffer
		virtual int32_t readBuffer(void*& buf, int32_t maxlen) {
			return 0;
		}
		//freeBuffer() must be called following every readBuffer() call;
		//after calling readBuffer() and before calling freeBuffer(),
		//all other operations are undefined
		virtual void freeBuffer(void* buf, int32_t len) {
		}

		//returns NULL if it doesn't provide a buffer; in that case you need to create
		//a StreamBuffer yourself
		virtual BufferedOutput* getBufferedOutput();
	};

	class StreamBuffer: public BufferedOutput
	{
	public:
		RGC::Ref<Stream> output;
		StreamBuffer();
		StreamBuffer(Stream& s, int bufsize = 4096);
		virtual void flushBuffer(int minBufferAllocation);
		~StreamBuffer();
	}
	;
	class FixedMemoryStream;
	class MemoryStream;

	//StreamWriter will directly access the BufferedOutput's buffer
	//(for performance reasons) and increment bufferPos accordingly.
	//if bufferPos reaches bufferSize, it will call the BufferedOutput's flushBuffer() method
	class StreamWriter: public RGC::Object
	{
	public:
		RGC::Ref<Object> outp;
		BufferedOutput* buffer;
		StreamBuffer sb;
		void write(const void* s, int len) {
			if (buffer->bufferSize - buffer->bufferPos < len) buffer->flushBuffer(len);
			memcpy(buffer->buffer + buffer->bufferPos, s, len);
			buffer->bufferPos += len;
		}
		void write(const char* s) {
			write((const uint8_t*) s, strlen(s));
		}
		void write(char c) {
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 1)) buffer->flushBuffer(1);
			buffer->buffer[buffer->bufferPos] = c;
			buffer->bufferPos++;
		}
		void write(int8_t i) {
			//snprintf() writes a null byte, so gotta reserve 4 bytes of buffer space
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 4)) buffer->flushBuffer(4);
			buffer->bufferPos += snprintf((char*) buffer->buffer + buffer->bufferPos, 4, "%hhi", i);
		}
		void write(int16_t i) {
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 6)) buffer->flushBuffer(6);
			buffer->bufferPos += snprintf((char*) buffer->buffer + buffer->bufferPos, 6, "%hi", i);
		}
		void write(int32_t i) {
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 11)) buffer->flushBuffer(11);
			buffer->bufferPos += snprintf((char*) buffer->buffer + buffer->bufferPos, 11, "%i", i);
		}
		void write(int64_t i) {
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 21)) buffer->flushBuffer(21);
			buffer->bufferPos += snprintf((char*) buffer->buffer + buffer->bufferPos, 21, "%lli", i);
		}
		void write(uint8_t i) {
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 4)) buffer->flushBuffer(4);
			buffer->bufferPos += snprintf((char*) buffer->buffer + buffer->bufferPos, 4, "%hhu", i);
		}
		void write(uint16_t i) {
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 6)) buffer->flushBuffer(6);
			buffer->bufferPos += snprintf((char*) buffer->buffer + buffer->bufferPos, 6, "%hu", i);
		}
		void write(uint32_t i) {
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 11)) buffer->flushBuffer(11);
			buffer->bufferPos += snprintf((char*) buffer->buffer + buffer->bufferPos, 11, "%u", i);
		}
		void write(uint64_t i) {
			if (unlikely(buffer->bufferSize - buffer->bufferPos < 21)) buffer->flushBuffer(21);
			buffer->bufferPos += snprintf((char*) buffer->buffer + buffer->bufferPos, 21, "%llu", i);
		}
		template<class ... P> void writeF(const char* fmt, P&&... p) {
			int minSpace = 0;
			if(minSpace + 1 > buffer->bufferSize - buffer->bufferPos) {
				redo:
				buffer->flushBuffer(minSpace+1);
			}
			minSpace = snprintf((char*) buffer->buffer + buffer->bufferPos, buffer->bufferSize - buffer->bufferPos, fmt, std::forward<P>(p)...);
			if(minSpace + 1 > buffer->bufferSize - buffer->bufferPos)
				goto redo;
			buffer->bufferPos+=minSpace;
		}
		void flush() {
			buffer->flushBuffer(0);
		}
		StreamWriter(BufferedOutput& s);
		StreamWriter(Stream& s);
		StreamWriter(MemoryStream& s);
		~StreamWriter();
	};

	class StreamReader: public Stream
	{ //writes are simply passed through
	public:
		typedef Delegate<void(const string&)> Callback;
		typedef Delegate<void(int)> StreamCallback;
		RGC::Ref<Stream> input;
		void* sr;
		bool* deletionFlag;
		Stream* out_s;
		union
		{
			DelegateBase<void(const string&)> cb;
			DelegateBase<void(int)> cb_s;
		};
		//BufferedOutput* tmp_out;
		string tmp;
		string tmp_delim;

		int tmp_i;

		bool shouldRead;
		bool reading;
		bool eof;
		/*void* curBuffer;
		 int curBufferLen;
		 int bufferSize;
		 bool bufferIsBorrowed;*/

		StreamReader(Stream& input, int bufsize = 4096);
		~StreamReader();

		string readTo(char delim);
		string readTo(const char* delim, int delimLen);
		string readTo(string delim);
		string readLine();
		int readTo(char delim, Stream& s);
		int readTo(const char* delim, int delimLen, Stream& s);
		int readTo(string delim, Stream& s);
		int readLine(Stream& s);

		void readTo(char delim, const Callback& cb);
		//*delim MUST NOT BE DELETED FOR THE ENTIRE DURATION OF THE READTO REQUEST!!!!!
		//it does not make a copy of delim!!!!!
		void readTo(const char* delim, int delimLen, const Callback& cb);
		void readTo(string delim, const Callback& cb);
		void readLine(const Callback& cb);
		void readTo(char delim, Stream& s, const StreamCallback& cb);
		void readTo(const char* delim, int delimLen, Stream& s, const StreamCallback& cb);
		void readTo(string delim, Stream& s, const StreamCallback& cb);
		void readLine(Stream& s, const StreamCallback& cb);

		//sync
		virtual int32_t read(void* buf, int32_t len);
		virtual int32_t write(const void* buf, int32_t len) {
			return input->write(buf, len);
		}
		virtual int32_t writeAll(const void* buf, int32_t len) {
			return input->writeAll(buf, len);
		}

		//async
		virtual void read(void* buf, int32_t len, const CP::Callback& cb, bool repeat = false);
		virtual void readAll(void* buf, int32_t len, const CP::Callback& cb);
		virtual void write(const void* buf, int32_t len, const CP::Callback& cb,
				bool repeat = false) {
			return input->write(buf, len, cb, repeat);
		}
		virtual void writeAll(const void* buf, int32_t len, const CP::Callback& cb) {
			return input->writeAll(buf, len, cb);
		}
		//does NOT cancel readTo() or readLine() operations
		virtual void cancelRead();
		virtual void cancelWrite();

		//=========misc===========
		//sync
		virtual void close();
		virtual void flush();

		//async
		virtual void close(const CP::Callback& cb);
		virtual void flush(const CP::Callback& cb);

		virtual int32_t readBuffer(void*& buf, int32_t maxlen);
		virtual void freeBuffer(void* buf, int32_t len);

		//internal
		void _beginRead();
		void _doSyncRead();
		void _readCB(int i);
		void _CB(uint8_t* data, int len, bool delimReached);
	}
	;

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
	}
	;

	enum class Operations
		: uint8_t
		{
			none = 0, read = 1, write, send, recv, sendTo, recvFrom, accept, readAll, writeAll,
		sendAll, recvAll, shutdown, connect, close, lastItem
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
				union
				{
					EndPoint* ep; //recvFrom
					const EndPoint* const_ep; //sendTo
				};
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
		void cancel(Events event);
		int32_t read(void* buf, int32_t len);
		int32_t readAll(void* buf, int32_t len) {
			return Stream::readAll(buf, len);
		}
		int32_t write(const void* buf, int32_t len);
		int32_t writeAll(const void* buf, int32_t len) {
			return Stream::writeAll(buf, len);
		}
		int32_t send(const void* buf, int32_t len, int32_t flags = 0);
		int32_t sendAll(const void* buf, int32_t len, int32_t flags = 0);
		int32_t recv(void* buf, int32_t len, int32_t flags = 0);
		int32_t recvAll(void* buf, int32_t len, int32_t flags = 0);
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
		void readAll(void* buf, int32_t len, const Callback& cb);

		void write(const void* buf, int32_t len, const Callback& cb, bool repeat = false);
		inline void repeatWrite(const void* buf, int32_t len, const Callback& cb) {
			write(buf, len, cb, true);
		}
		void writeAll(const void* buf, int32_t len, const Callback& cb);

		void recv(void* buf, int32_t len, int32_t flags, const Callback& cb, bool repeat = false);
		inline void repeatRecv(void* buf, int32_t len, int32_t flags, const Callback& cb) {
			recv(buf, len, flags, cb, true);
		}
		void recvAll(void* buf, int32_t len, int32_t flags, const Callback& cb);

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

		//misc
		void cancelRead();
		void cancelWrite();
		void cancelSend() {
			cancelWrite();
		}
		void cancelRecv() {
			cancelRead();
		}
	};
	class Socket: public File
	{
	public:
		union
		{
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

		int32_t recvFrom(void* buf, int32_t len, int32_t flags, EndPoint& ep);
		int32_t sendTo(const void* buf, int32_t len, int32_t flags, const EndPoint& ep);
		//ep has to remain valid for the entire duration of the request
		void recvFrom(void* buf, int32_t len, int32_t flags, EndPoint& ep, const Callback& cb,
				bool repeat = false);
		inline void repeatRecvFrom(void* buf, int32_t len, int32_t flags, EndPoint& ep,
				const Callback& cb) {
			recvFrom(buf, len, flags, ep, cb, true);
		}
		void sendTo(const void* buf, int32_t len, int32_t flags, const EndPoint& ep,
				const Callback& cb, bool repeat = false);
		inline void repeatSendTo(const void* buf, int32_t len, int32_t flags, const EndPoint& ep,
				const Callback& cb) {
			sendTo(buf, len, flags, ep, cb, true);
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
	class StandardStream: public Stream
	{
	public:
		CP::File in, out;
		StandardStream();
		template<class P> void addToPoll(P& p) {
			p.add(in);
			p.add(out);
		}
		template<class P> void delFromPoll(P& p) {
			p.del(in);
			p.del(out);
		}
		//sync
		virtual int32_t read(void* buf, int32_t len);
		virtual int32_t readAll(void* buf, int32_t len);
		virtual int32_t write(const void* buf, int32_t len);
		virtual int32_t writeAll(const void* buf, int32_t len);

		//async
		virtual void read(void* buf, int32_t len, const Callback& cb, bool repeat = false);
		virtual void readAll(void* buf, int32_t len, const Callback& cb);
		virtual void write(const void* buf, int32_t len, const Callback& cb, bool repeat = false);
		virtual void writeAll(const void* buf, int32_t len, const Callback& cb);
		virtual void cancelRead();
		virtual void cancelWrite();

		//=========misc===========
		//sync
		virtual void close(); //may block
		virtual void flush();

		//async
		virtual void close(const Callback& cb);
		virtual void flush(const Callback& cb);
	};

	class FixedMemoryStream: public Stream, public BufferedOutput
	{
	public:
		//uint8_t* data;
		//int len, pos;
		int len;
		FixedMemoryStream();
		FixedMemoryStream(void* data, int len);
		//sync
		virtual int32_t read(void* buf, int32_t len);
		virtual int32_t readAll(void* buf, int32_t len);
		virtual int32_t write(const void* buf, int32_t len);
		virtual int32_t writeAll(const void* buf, int32_t len);

		//async
		virtual void read(void* buf, int32_t len, const Callback& cb, bool repeat = false);
		virtual void readAll(void* buf, int32_t len, const Callback& cb);
		virtual void write(const void* buf, int32_t len, const Callback& cb, bool repeat = false);
		virtual void writeAll(const void* buf, int32_t len, const Callback& cb);
		virtual void cancelRead();
		virtual void cancelWrite();

		//=========misc===========
		//sync
		virtual void close();
		virtual void flush();

		//async
		virtual void close(const Callback& cb);
		virtual void flush(const Callback& cb);

		virtual int32_t readBuffer(void*& buf, int32_t maxlen);
		virtual void flushBuffer(int minBufferAllocation);

		virtual BufferedOutput* getBufferedOutput() override;
	};
	class MemoryStream: public FixedMemoryStream
	{
	public:
		MemoryStream(int capacity = 4096);
		~MemoryStream();
		void ensureCapacity(int c);
		virtual int32_t write(const void* buf, int32_t len);
		virtual int32_t writeAll(const void* buf, int32_t len);
		virtual void close();
		void clear();
		virtual void flushBuffer(int minBufferAllocation);
		//user must delete the MemoryStream instance after this call
		void keepBuffer();
	};
}

#endif
