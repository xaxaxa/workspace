/*
 * cpoll.C
 *
 *  Created on: 2012-09-14
 *      Author: xaxaxa
 */
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
#include "cpoll.H"
#include "cpoll_internal.H"
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
namespace CP
{
	//CPollException

	CPollException::CPollException() :
			message(strerror(errno)), number(errno) {
	}
	CPollException::CPollException(int32_t number) :
			message(strerror(number)), number(number) {
	}
	CPollException::CPollException(string message, int32_t number) :
			message(message), number(number) {
	}
	CPollException::~CPollException() throw () {
	}
	const char* CPollException::what() const throw () {
		return message.c_str();
	}

	AbortException::AbortException() {
	}
	AbortException::~AbortException() throw () {
	}
	const char* AbortException::what() const throw () {
		return "aborting cpoll loop";
	}

	CancelException::CancelException() {
	}
	CancelException::~CancelException() throw () {
	}
	const char* CancelException::what() const throw () {
		return "cancelling current cpoll operation";
	}

	vector<RGC::Ref<EndPoint> > EndPoint::lookupHost(const char* hostname, const char* port,
			int32_t family, int32_t socktype, int32_t proto, int32_t flags) {
		vector<RGC::Ref<EndPoint> > tmp;
		addrinfo hints, *result, *rp;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = family; /* Allow IPv4 or IPv6 */
		hints.ai_socktype = socktype;
		hints.ai_flags = flags;
		hints.ai_protocol = proto;

		int32_t s = getaddrinfo(hostname, port, &hints, &result);
		if (s != 0) {
			throw CPollException(gai_strerror(s));
		}
		for (rp = result; rp != NULL; rp = rp->ai_next) {
			EndPoint* ep = fromSockAddr(rp->ai_addr);
			tmp.push_back(ep);
			ep->release();
		}
		freeaddrinfo(result);
		return tmp;
	}
	EndPoint* EndPoint::fromSockAddr(const sockaddr* addr) {
		switch (addr->sa_family) {
			case AF_INET:
				return new IPEndPoint(*((sockaddr_in*) addr));
			case AF_INET6:
				return new IPv6EndPoint(*((sockaddr_in6*) addr));
			case AF_UNIX:
				return new UNIXEndPoint(*((sockaddr_un*) addr));
			default:
				return NULL;
		}
	}
	EndPoint* EndPoint::create(int32_t AddressFamily) {
		switch (AddressFamily) {
			case AF_INET:
				return new IPEndPoint();
			case AF_INET6:
				return new IPv6EndPoint();
			case AF_UNIX:
				return new UNIXEndPoint();
			default:
				return NULL;
		}
	}

	//IPEndPoint

	IPEndPoint::IPEndPoint() {
		this->addressFamily = AF_INET;
	}
	IPEndPoint::IPEndPoint(IPAddress address, in_port_t port) {
		this->addressFamily = AF_INET;
		this->address = address;
		this->port = port;
	}
	void IPEndPoint::set_addr(const sockaddr_in& addr) {
		this->addressFamily = AF_INET;
		this->address = IPAddress(addr.sin_addr);
		this->port = ntohs(addr.sin_port);
	}
	void IPEndPoint::setSockAddr(const sockaddr* addr) {
		if (addr->sa_family != AF_INET) throw CPollException(
				"attemting to set the address of an IPEndPoint to a sockaddr that is not AF_INET");
		set_addr(*(sockaddr_in*) addr);
	}
	IPEndPoint::IPEndPoint(const sockaddr_in& addr) {
		set_addr(addr);
	}
	void IPEndPoint::getSockAddr(sockaddr* addr) const {
		sockaddr_in* addr_in = (sockaddr_in*) addr;
		addr_in->sin_family = AF_INET;
		addr_in->sin_port = htons(port);
		addr_in->sin_addr = address.a;
	}
	int32_t IPEndPoint::getSockAddrSize() const {
		return sizeof(sockaddr_in);
	}
	void IPEndPoint::clone(EndPoint& to) const {
		if (to.addressFamily != addressFamily) throw CPollException(
				"attempting to clone an EndPoint to another EndPoint with a different addressFamily");
		IPEndPoint& tmp((IPEndPoint&) to);
		tmp.address = address;
		tmp.port = port;
	}
	string IPEndPoint::toStr() const {
		stringstream s;
		s << address.toStr() << ':' << port;
		return s.str();
	}

	//IPv6EndPoint

	IPv6EndPoint::IPv6EndPoint() {
		this->addressFamily = AF_INET6;
	}
	IPv6EndPoint::IPv6EndPoint(IPv6Address address, in_port_t port) {
		this->addressFamily = AF_INET6;
		this->address = address;
		this->port = port;
	}
	void IPv6EndPoint::set_addr(const sockaddr_in6& addr) {
		this->addressFamily = AF_INET6;
		this->address = IPv6Address(addr.sin6_addr);
		this->port = ntohs(addr.sin6_port);
		flowInfo = addr.sin6_flowinfo;
		scopeID = addr.sin6_scope_id;
	}
	IPv6EndPoint::IPv6EndPoint(const sockaddr_in6& addr) {
		set_addr(addr);
	}
	void IPv6EndPoint::setSockAddr(const sockaddr* addr) {
		if (addr->sa_family != AF_INET6) throw CPollException(
				"attemting to set the address of an IPv6EndPoint to a sockaddr that is not AF_INET6");
		set_addr(*(sockaddr_in6*) addr);
	}
	void IPv6EndPoint::getSockAddr(sockaddr* addr) const {
		sockaddr_in6* addr_in = (sockaddr_in6*) addr;
		addr_in->sin6_family = AF_INET6;
		addr_in->sin6_port = htons(port);
		addr_in->sin6_addr = address.a;
		addr_in->sin6_flowinfo = flowInfo;
		addr_in->sin6_scope_id = scopeID;
	}
	int32_t IPv6EndPoint::getSockAddrSize() const {
		return sizeof(sockaddr_in);
	}
	void IPv6EndPoint::clone(EndPoint& to) const {
		if (to.addressFamily != addressFamily) throw CPollException(
				"attempting to clone an EndPoint to another EndPoint with a different addressFamily");
		IPv6EndPoint& tmp((IPv6EndPoint&) to);
		tmp.address = address;
		tmp.port = port;
		tmp.flowInfo = flowInfo;
		tmp.scopeID = scopeID;
	}
	string IPv6EndPoint::toStr() const {
		stringstream s;
		s << '[' << address.toStr() << "]:" << port;
		return s.str();
	}

	//UNIXEndPoint
	UNIXEndPoint::UNIXEndPoint() {
		this->addressFamily = AF_UNIX;
	}
	UNIXEndPoint::UNIXEndPoint(string name) {
		this->addressFamily = AF_UNIX;
		this->name = name;
	}
	void UNIXEndPoint::set_addr(const sockaddr_un& addr) {
		this->addressFamily = AF_UNIX;
		this->name = addr.sun_path;
	}
	UNIXEndPoint::UNIXEndPoint(const sockaddr_un& addr) {
		set_addr(addr);
	}
	void UNIXEndPoint::setSockAddr(const sockaddr* addr) {
		if (addr->sa_family != AF_UNIX) throw CPollException(
				"attemting to set the address of an UNIXEndPoint to a sockaddr that is not AF_UNIX");
		set_addr(*(sockaddr_un*) addr);
	}
	void UNIXEndPoint::getSockAddr(sockaddr* addr) const {
		sockaddr_un* a = (sockaddr_un*) addr;
		a->sun_family = AF_UNIX;
		strncpy(a->sun_path, name.c_str(), name.length());
		a->sun_path[name.length()] = '\0';
	}
	int32_t UNIXEndPoint::getSockAddrSize() const {
		return sizeof(sa_family_t) + name.length() + 1;
	}
	void UNIXEndPoint::clone(EndPoint& to) const {
		if (to.addressFamily != addressFamily) throw CPollException(
				"attempting to clone an EndPoint to another EndPoint with a different addressFamily");
		UNIXEndPoint& tmp((UNIXEndPoint&) to);
		tmp.name = name;
	}

	Handle::Handle() {
		deinit();
	}
	Handle::Handle(HANDLE handle) {
		init(handle);
	}
	void Handle::init(HANDLE handle) {
		this->handle = handle;
		checkError(handle);
	}
	void Handle::deinit() {
		_supportsEPoll = true;
		handle = -1;
	}
	int32_t Handle::dispatchMultiple(Events events, const EventData& evtd) {
		//cout << (int32_t)events << endl;
		int32_t ret = 0;
		for (int32_t i = 0; i < numEvents; i++) {
			Events e = indexToEvent(i);
			//cout << (int32_t)e << " " << (((event_t)e)&((event_t)events)) << endl;
			if ((((event_t) e) & ((event_t) events)) == (event_t) e) {
				dispatch(e, evtd);
				ret++;
			}
		}
		//cout << ret << endl;
		return ret;
	}
	Events Handle::wait(EventData& evtd) { //since this is single-file, poll() will be used.
		//Events events=Events::none;
		Events w = getEvents();
		pollfd pfd;
		pfd.fd = handle;
		pfd.events = eventsToPoll(w);
		if (pfd.events == 0) return Events::none;
		poll(&pfd, 1, -1);
		evtd.hungUp = (pfd.revents & POLLHUP);
		evtd.error = (pfd.revents & POLLERR);
		/*for(int32_t i=0;i<numEvents;i++) {
		 Events e=indexToEvent(i);
		 short p=eventToPoll(e);
		 if(p&pfd.revents!=0) events=(Events)((event_t)events | (event_t)e);
		 }*/
		return pollToEvents(pfd.revents);
	}
	int32_t Handle::waitAndDispatch() {
		EventData evtd;
		Events e = wait(evtd);
		if (e == Events::none) return -1;
		return dispatchMultiple(e, evtd);
	}
	void Handle::loop() {
		try {
			while (waitAndDispatch() >= 0)
				;
		} catch (const AbortException& ex) {

		}
	}
	/*void Handle::close() {
	 ::close(handle);
	 }*/
	Handle::~Handle() {
		//if (onClose != nullptr) onClose();
		//::close(handle);
	}

	//File
	File::File() :
			deletionFlag(NULL), dispatching(false) {
	}
	File::File(HANDLE handle) :
			deletionFlag(NULL), dispatching(false) {
		init(handle);
	}
	void File::init(HANDLE handle) {
		Handle::init(handle);
	}
	Events File::_getEvents() {
		if (dispatching) return preDispatchEvents;
		Events e = Events::none;
		for (int32_t i = 0; i < numEvents; i++)
			if (eventData[i].state != EventHandlerData::States::invalid) (event_t&) e |=
					(event_t) indexToEvent(i);
		
		//cout << "_getEvents: " << (int32_t)e << endl;
		return e;
	}
	///only accepts one event
	EventHandlerData* File::beginAddEvent(Events event) {
		EventHandlerData *ed = &eventData[eventToIndex(event)];
		if (ed->state != EventHandlerData::States::invalid) throw CPollException(
				"Already listening for the specified event on the specified file. "
						"For example, you may not read() and recv() on one socket at the same time.");
		return ed;
	}
	void File::endAddEvent(Events event, bool repeat) {
		Events old_events = _getEvents();
		eventData[eventToIndex(event)].state =
				repeat ? (EventHandlerData::States::repeat) : (EventHandlerData::States::once);
		if (onEventsChange != nullptr && !dispatching) onEventsChange(*this, old_events);
	}
	int32_t File::read(void* buf, int32_t len) {
		return ::read(handle, buf, len);
	}
	int32_t File::write(const void* buf, int32_t len) {
		return ::write(handle, buf, len);
	}
	int32_t File::writeAll(const void* buf, int32_t len) {
		int32_t bw = 0, bw1 = 0;
		while (bw < len && (bw1 = write(((char*) buf) + bw, len - bw)) > 0)
			bw += bw1;
		return (bw1 < 0 && bw <= 0) ? -1 : bw;
	}
	int32_t File::send(const void* buf, int32_t len, int32_t flags) {
		return ::send(handle, buf, len, flags);
	}
	int32_t File::sendAll(const void* buf, int32_t len, int32_t flags) {
		int32_t bw = 0, bw1 = 0;
		while (bw < len && (bw1 = send(((char*) buf) + bw, len - bw, flags)) > 0)
			bw += bw1;
		return (bw1 < 0 && bw <= 0) ? -1 : bw;
	}
	int32_t File::recv(void* buf, int32_t len, int32_t flags) {
		return ::recv(handle, buf, len, flags);
	}
	void File::doOperation(EventHandlerData& ed, const EventData& evtd,
			EventHandlerData::States oldstate) {
		Operations op = ed.op;
		int32_t r = 0;
		if (unlikely(handle<0)) {
			r = -1;
			goto asdf;
		}
		switch (op) {
			case Operations::read:
				r = read(ed.misc.bufferIO.buf, ed.misc.bufferIO.len);
				break;
			case Operations::write:
				r = write(ed.misc.bufferIO.buf, ed.misc.bufferIO.len);
				break;
			case Operations::writeAll:
				r = write(((char*) ed.misc.bufferIO.buf) + ed.misc.bufferIO.len_done,
						ed.misc.bufferIO.len - ed.misc.bufferIO.len_done);
				//cout << "wrote " << r << " bytes on fd " << handle << endl;
				if (r <= 0) {
					ed.state = EventHandlerData::States::invalid;
					if (ed.cb != nullptr)ed.cb(ed.misc.bufferIO.len_done == 0 ? r : ed.misc.bufferIO.len_done);
				}
				ed.misc.bufferIO.len_done += r;
				//cout << "len_done = " << ed.misc.bufferIO.len_done
				//		<< " of " << ed.misc.bufferIO.len << endl;
				if (ed.misc.bufferIO.len_done >= ed.misc.bufferIO.len) {
					ed.state = EventHandlerData::States::invalid;
					if (ed.cb != nullptr)ed.cb(ed.misc.bufferIO.len_done);
				}
				return;
			case Operations::recv:
				r = recv(ed.misc.bufferIO.buf, ed.misc.bufferIO.len, ed.misc.bufferIO.flags);
				break;
			case Operations::send:
				r = send(ed.misc.bufferIO.buf, ed.misc.bufferIO.len, ed.misc.bufferIO.flags);
				break;
			case Operations::sendAll:
				r = send(((char*) ed.misc.bufferIO.buf) + ed.misc.bufferIO.len_done,
						ed.misc.bufferIO.len - ed.misc.bufferIO.len_done, ed.misc.bufferIO.flags);
				if (r <= 0) {
					ed.state = EventHandlerData::States::invalid;
					if (ed.cb != nullptr)ed.cb(ed.misc.bufferIO.len_done == 0 ? -1 : ed.misc.bufferIO.len_done);
				}
				ed.misc.bufferIO.len_done += r;
				if (ed.misc.bufferIO.len_done >= ed.misc.bufferIO.len) {
					ed.state = EventHandlerData::States::invalid;
					if (ed.cb != nullptr)ed.cb(ed.misc.bufferIO.len_done);
				}
				return;
			case Operations::close:
				close();
			default:
				break;
		}
		if (r <= 0) {
			//invalidate the current event listener
			ed.state = EventHandlerData::States::invalid;
		}
		asdf: if (ed.cb != nullptr) ed.cb(r);
		if (r > 0 && (evtd.error || evtd.hungUp) && oldstate == EventHandlerData::States::repeat) if (ed.cb
				!= nullptr) ed.cb(-1);
	}
	void File::dispatch(Events event, const EventData& evtd, bool& deletionFlag) {
		//cout << (int32_t)event << " dispatched" << endl;
		EventHandlerData& ed = eventData[eventToIndex(event)];
		if (ed.state == EventHandlerData::States::invalid) return;
		bool tmp;
		if (!(tmp = dispatching)) preDispatchEvents = _getEvents();
		EventHandlerData::States oldstate = ed.state;
		if ((ed.state == EventHandlerData::States::once) || evtd.hungUp || evtd.error) ed.state =
				EventHandlerData::States::invalid;
		dispatching = true;

		//this->deletionFlag = &deletionFlag;
		try {
			doOperation(ed, evtd, oldstate);
		} catch (const CancelException& ex) {
			ed.state = EventHandlerData::States::invalid;
		}
		if (deletionFlag) return;
		//this->deletionFlag = NULL;
		dispatching = tmp;
		//cout << "clear event" << endl;

		//cout << (eventData[eventToIndex(event)].state==EventHandlerData::States::once
		//		? "true" : "false") << endl;
		//if(evtd.hungUp) cout << "socket hung up" << endl;
		//if(evtd.error) cout << "socket error" << endl;
		//if(!b) cout << "doOperation returned false" << endl;
		//cout << "b=" << b << endl;
		///if (!b) ed.state = EventHandlerData::States::invalid;
		
		//else cout << (int32_t)eventData[eventToIndex(event)].state << endl;
		if (!tmp && onEventsChange != nullptr) onEventsChange(*this, preDispatchEvents);
	}
	int32_t File::dispatchMultiple(Events events, const EventData& evtd) {
		preDispatchEvents = _getEvents();
		dispatching = true;
		int32_t ret = 0;
		bool d = false;
		this->deletionFlag = &d;
		for (int32_t i = 0; i < numEvents; i++) {
			Events e = indexToEvent(i);
			//cout << (int32_t)e << " " << (((event_t)e)&((event_t)events)) << endl;
			if ((((event_t) e) & ((event_t) events)) == (event_t) e) {
				dispatch(e, evtd, d);
				ret++;
				if (d) break;
			}
		}
		if (d) return ret;
		this->deletionFlag = NULL;
		dispatching = false;
		if (onEventsChange != nullptr) {
			//printf("onEventsChange\n");
			onEventsChange(*this, preDispatchEvents);
		}
		return ret;
	}
	void File::fillIOEventHandlerData(EventHandlerData* ed, void* buf, int32_t len,
			const Callback& cb, Events e, Operations op) {
		ed->cb = cb;
		ed->misc.bufferIO.buf = buf;
		ed->misc.bufferIO.len = len;
		ed->op = op;
	}
	void File::read(void* buf, int32_t len, const Callback& cb, bool repeat) {
		if (!_supportsEPoll) {
			asdfg: int32_t r = read(buf, len);
			cb(r);
			if (repeat && r > 0) goto asdfg;
			return;
		}
		static const Events e = Events::in;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, buf, len, cb, e, Operations::read);
		endAddEvent(e, repeat);
	}
	void File::write(const void* buf, int32_t len, const Callback& cb, bool repeat) {
		if (!_supportsEPoll) {
			asdfg: int32_t r = write(buf, len);
			cb(r);
			if (repeat && r > 0) goto asdfg;
			return;
		}
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, (void*) buf, len, cb, e, Operations::write);
		endAddEvent(e, repeat);
	}
	void File::writeAll(const void* buf, int32_t len, const Callback& cb) {
		if (!_supportsEPoll) {
			int32_t bw = 0, bw1 = 0;
			while (bw < len && (bw1 = write(((char*) buf) + bw, len - bw)) > 0)
				bw += bw1;
			cb((bw1 < 0 && bw <= 0) ? -1 : bw);
			return;
		}
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, (void*) buf, len, cb, e, Operations::writeAll);
		ed->misc.bufferIO.len_done = 0;
		endAddEvent(e, true);
	}
	void File::recv(void* buf, int32_t len, int32_t flags, const Callback& cb, bool repeat) {
		static const Events e = Events::in;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, buf, len, cb, e, Operations::recv);
		ed->misc.bufferIO.flags = flags;
		endAddEvent(e, repeat);
	}
	void File::send(const void* buf, int32_t len, int32_t flags, const Callback& cb, bool repeat) {
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, (void*) buf, len, cb, e, Operations::send);
		ed->misc.bufferIO.flags = flags;
		endAddEvent(e, repeat);
	}
	void File::sendAll(const void* buf, int32_t len, int32_t flags, const Callback& cb) {
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, (void*) buf, len, cb, e, Operations::sendAll);
		ed->misc.bufferIO.len_done = 0;
		ed->misc.bufferIO.flags = flags;
		endAddEvent(e, true);
	}
	File::~File() {
		if (deletionFlag != NULL) *deletionFlag = true;
		if (handle < 0) return;
		close();
	}

	void File::close() {
		//if(handle<0)throw runtime_error("asdf");
		if (onClose != nullptr) onClose(*this);
		::close(handle);
		handle = -1;
		deinit();
	}
	void File::flush() {

	}
	void File::close(const Callback& cb) {
		if (!_supportsEPoll) {
			close();
			cb(0);
			return;
		}
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = cb;
		ed->op = Operations::close;
		endAddEvent(e, true);
	}
	void File::flush(const Callback& cb) {
		cb(0);
	}
	
	//Socket
	Socket::Socket() :
			addressFamily(AF_UNSPEC), type(0), protocol(0) {

	}
	Socket::Socket(HANDLE handle, int32_t d, int32_t t, int32_t p) {
		init(handle, d, t, p);
	}
	Socket::Socket(int32_t d, int32_t t, int32_t p) {
		init(d, t, p);
	}
	void Socket::init(HANDLE handle, int32_t d, int32_t t, int32_t p) {
		File::init(handle);
		addressFamily = d;
		type = t;
		protocol = p;
	}
	void Socket::init(int32_t d, int32_t t, int32_t p) {
		File::init(socket(d, t | SOCK_CLOEXEC, p));
		addressFamily = d;
		type = t;
		protocol = p;
	}
	//the caller must release() or free() the returned object
	EndPoint* Socket::getLocalEndPoint() {
		EndPoint* ep = EndPoint::create(addressFamily);
		socklen_t l = (socklen_t) (ep->getSockAddrSize());
		char addr[l];
		getsockname(handle, (struct sockaddr*) addr, &l);
		ep->setSockAddr((struct sockaddr*) addr);
		return ep;
	}
	//the caller must release() or free() the returned object
	EndPoint* Socket::getRemoteEndPoint() {
		EndPoint* ep = EndPoint::create(addressFamily);
		socklen_t l = (socklen_t) (ep->getSockAddrSize());
		char addr[l];
		getpeername(handle, (struct sockaddr*) addr, &l);
		ep->setSockAddr((struct sockaddr*) addr);
		return ep;
	}
	void Socket::doOperation(EventHandlerData& ed, const EventData& evtd,
			EventHandlerData::States oldstate) {
		Operations op = ed.op;
		switch (op) {
			case Operations::accept:
				ed.cb((evtd.hungUp | evtd.error) ? -1 : 0);
				break;
			case Operations::shutdown:
				ed.cb(shutdown(ed.misc.shutdown.how));
				break;
			case Operations::connect:
				ed.cb((evtd.error || evtd.hungUp) ? -1 : 0);
				break;
			default:
				File::doOperation(ed, evtd, oldstate);
		}
	}

	void Socket::bind(const sockaddr *addr, int32_t addr_size) {
		if (handle == -1) init(addr->sa_family, SOCK_STREAM, 0);
		int32_t tmp12345 = 1;
		setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &tmp12345, sizeof(tmp12345));
		if (::bind(handle, addr, addr_size) != 0) throw CPollException(errno);
	}
	void Socket::bind(const EndPoint &ep) {
		int32_t size = ep.getSockAddrSize();
		uint8_t tmp[size];
		ep.getSockAddr((sockaddr*) tmp);
		bind((sockaddr*) tmp, size);
	}
	void Socket::bind(const char* hostname, const char* port, int32_t family, int32_t socktype,
			int32_t proto, int32_t flags) {
		//XXX
		if (handle != -1) throw CPollException(
				"Socket::bind(string, ...) creates a socket, but the socket is already initialized");
		auto hosts = EndPoint::lookupHost(hostname, port, 0, socktype, proto);
		unsigned int i;
		for (i = 0; i < hosts.size(); i++) {
			int _f = socket(hosts[i]->addressFamily, socktype | SOCK_CLOEXEC, proto);
			if (_f < 0) continue;
			int32_t tmp12345 = 1;
			setsockopt(_f, SOL_SOCKET, SO_REUSEADDR, &tmp12345, sizeof(tmp12345));
			int size = hosts[i]->getSockAddrSize();
			uint8_t tmp[size];
			hosts[i]->getSockAddr((sockaddr*) tmp);
			if (::bind(_f, (sockaddr*) tmp, size) == 0) {
				init(_f, hosts[i]->addressFamily, socktype, proto);
				return;
			} else {
				::close(_f);
				continue;
			}
		}
		throw CPollException("no bindable hosts were found; last error: " + string(strerror(errno)));
	}
	void Socket::listen(int32_t backlog) {
		checkError(::listen(handle, backlog));
	}
	int32_t Socket::shutdown(int32_t how) {
		return ::shutdown(handle, how);
	}
	void Socket::shutdown(int32_t how, const Callback& cb) {
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = cb;
		ed->op = Operations::shutdown;
		endAddEvent(e, false);
	}

	void __socket_init_if_not_already(Socket* s, int32_t af) {
		if (s->handle < 0) s->init(af, SOCK_STREAM, 0);
	}
	void Socket::connect(const sockaddr *addr, int32_t addr_size) {
		__socket_init_if_not_already(this, addr->sa_family);
		retry: int32_t tmp = ::connect(handle, addr, addr_size);
		if (tmp != 0 && errno != EINPROGRESS) {
			if (errno == EINTR) goto retry;
			throw CPollException(errno);
		}
	}
	void Socket::connect(const EndPoint &ep) {
		int32_t l = ep.getSockAddrSize();
		char tmp[l];
		ep.getSockAddr((sockaddr*) tmp);
		connect((sockaddr*) tmp, l);
	}
	void Socket::connect(const char* hostname, const char* port, int32_t family, int32_t socktype,
			int32_t proto, int32_t flags) {
		//XXX
		if (handle != -1) throw CPollException(
				"Socket::connect(string, ...) creates a socket, but the socket is already initialized");
		auto hosts = EndPoint::lookupHost(hostname, port, 0, socktype, proto);
		unsigned int i;
		for (i = 0; i < hosts.size(); i++) {
			int _f = socket(hosts[i]->addressFamily, socktype, proto);
			if (_f < 0) continue;
			int size = hosts[i]->getSockAddrSize();
			uint8_t tmp[size];
			hosts[i]->getSockAddr((sockaddr*) tmp);
			if (::connect(_f, (sockaddr*) tmp, size) == 0) {
				init(_f, hosts[i]->addressFamily, socktype, proto);
				break;
			} else {
				::close(_f);
				continue;
			}
		}
		throw CPollException("no reachable hosts were found; last error: " + string(strerror(errno)));
	}
	//the caller must release() or free() the returned object;
	//also this will NOT automatically add the new socket to this Poll instance
	//because the user might want to handle the socket on a different thread
	//which requires a different Poll instance
	Socket* Socket::accept() {
		Socket* sock = new Socket(acceptHandle(), addressFamily, type, protocol);
		return sock;
	}
	HANDLE Socket::acceptHandle() {
		HANDLE h = ::accept4(handle, NULL, NULL, SOCK_CLOEXEC);
		if (h < 0) {
			int32_t e = errno;
			onError(e);
		}
		return h;
	}

	void Socket::connect(const sockaddr* addr, int32_t addr_size, const Callback& cb) {
		__socket_init_if_not_already(this, addr->sa_family);
		checkError(fcntl(handle, F_SETFL, checkError(fcntl(handle, F_GETFL, 0)) | O_NONBLOCK));
		connect(addr, addr_size);
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = cb;
		ed->op = Operations::connect;
		endAddEvent(e, false);
	}

	void Socket::connect(const EndPoint& ep, const Callback& cb) {
		__socket_init_if_not_already(this, ep.addressFamily);
		checkError(fcntl(handle, F_SETFL, checkError(fcntl(handle, F_GETFL, 0)) | O_NONBLOCK));
		connect(ep);
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = cb;
		ed->op = Operations::connect;
		endAddEvent(e, false);
	}
	void Socket_acceptStub(Socket* th, int32_t i) {
		Socket* s = th->accept();
		th->_acceptCB(s);
	}
	void Socket_acceptHandleStub(Socket* th, int32_t i) {
		HANDLE h = th->acceptHandle();
		th->_acceptHandleCB(h);
	}
	//user must eventually release() or free() the received object
	void Socket::accept(const Delegate<void(Socket*)>& cb, bool repeat) {
		_acceptCB = cb;
		static const Events e = Events::in;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = Callback(&Socket_acceptStub, this);
		ed->op = Operations::accept;
		endAddEvent(e, repeat);
	}
	void Socket::acceptHandle(const Delegate<void(HANDLE)>& cb, bool repeat) {
		_acceptHandleCB = cb;
		static const Events e = Events::in;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = Callback(&Socket_acceptHandleStub, this);
		ed->op = Operations::accept;
		endAddEvent(e, repeat);
	}

	//SignalFD
	int32_t SignalFD::MAX_EVENTS(4);
	SignalFD::SignalFD(HANDLE handle, const sigset_t& mask) :
			Handle(handle), mask(mask) {
	}
	SignalFD::SignalFD(const sigset_t& mask, int32_t flags) :
			Handle(signalfd(-1, &mask, flags | SFD_CLOEXEC)), mask(mask) {
	}
	void SignalFD::dispatch(Events event, const EventData& evtd) {
		Signal sig[MAX_EVENTS];
		int32_t br = ::read(handle, sig, sizeof(sig));
		if (callback != nullptr) {
			br /= sizeof(Signal);
			for (int32_t i = 0; i < br; i++) {
				callback(sig[i]);
			}
		}
	}
	Events SignalFD::getEvents() {
		return Events::in;
	}

	//EventFD
	EventFD::EventFD(HANDLE handle) :
			File(handle) {
	}
	EventFD::EventFD(uint32_t initval, int32_t flags) :
			File(eventfd(initval, flags | EFD_CLOEXEC)) {
	}
	void EventFD::doOperation(EventHandlerData& ed, const EventData& evtd,
			EventHandlerData::States oldstate) {
		int32_t r = 0;
		switch (ed.op) {
			case Operations::read:
				r = eventfd_read(handle, &ed.misc.eventfd.evt);
				break;
			case Operations::write:
				r = eventfd_write(handle, ed.misc.eventfd.evt);
				break;
			default:
				break;
		}
		ed.cb(r);
	}
	eventfd_t EventFD::getEvent() {
		eventfd_t tmp;
		if (eventfd_read(handle, &tmp) == 0) return tmp;
		return -1;
	}
	void EventFD_getEventStub(EventFD* th, int i) {
		th->cb((i < 0) ? -1 : (th->eventData[eventToIndex(Events::in)].misc.eventfd.evt));
	}
	void EventFD::getEvent(const Delegate<void(eventfd_t)>& cb, bool repeat) {
		Events e = Events::in;
		EventHandlerData* ed = beginAddEvent(e);
		this->cb = cb;
		ed->cb = Callback(&EventFD_getEventStub, this);
		ed->op = Operations::read;
		endAddEvent(e, repeat);
	}
	int32_t EventFD::sendEvent(eventfd_t evt) {
		return eventfd_write(handle, evt);
	}
	void EventFD::sendEvent(eventfd_t evt, const Delegate<void(int32_t)>& cb) {
		Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = cb;
		ed->misc.eventfd.evt = evt;
		ed->op = Operations::write;
		endAddEvent(e, false);
	}

	//Poll
	int32_t Poll::MAX_EVENTS(32);
	Poll::Poll(HANDLE handle) :
			Handle(handle), curEvents(NULL), active(0), cur_handle(-1) {
		disableSignals();
	}
	Poll::Poll() :
			Handle(checkError(epoll_create1(EPOLL_CLOEXEC))), curEvents(NULL), active(0),
					cur_handle(-1) {
		disableSignals();
	}
	void Poll::_applyHandle(Handle& h, Events old_e) {
		if (!h._supportsEPoll) {
			//if (debug) printf("_applyHandle: h=%i, h._supportsEPoll=false\n", h.handle);
			return;
		}
//if (unlikely(has_deleted) && tmp_deleted.find(&h) != tmp_deleted.end()) return;
		Events new_e = h.getEvents();
//if (debug) printf("_applyHandle: h=%i, old_e=%i, new_e=%i\n", h.handle, old_e, new_e);
		if (new_e == old_e) return;

		if (h.handle < 0) {
			//throw runtime_error("test");
			EventData evtd;
			evtd.hungUp = evtd.error = true;
			while (new_e != Events::none) {
				h.dispatchMultiple(new_e, evtd);
				new_e = h.getEvents();
			}
			return;
		}
		epoll_event evt;
		if (old_e == Events::none) {
			fillEPollEvents(h, evt, new_e);
			//cout << "added " << h.handle << endl;
			int r = epoll_ctl(this->handle, EPOLL_CTL_ADD, h.handle, &evt);
			if (r < 0 && errno == EPERM) {
				h._supportsEPoll = false;
				EventData evtd;
				evtd.hungUp = evtd.error = false;
				while (new_e != Events::none) {
					h.dispatchMultiple(new_e, evtd);
					new_e = h.getEvents();
				}
				return;
			}
			checkError(r);
			h.retain();
			active++;
		} else if (new_e == Events::none) {
			//cout << "deleted " << h.handle << endl;
			checkError(epoll_ctl(this->handle, EPOLL_CTL_DEL, h.handle, NULL));
			if (likely(curEvents!=NULL)) for (int i = curIndex; i < curLength; i++) {
				if (curEvents[i].data.ptr == (void*) &h) curEvents[i].data.ptr = NULL;
			}
			h.release();
			active--;
		} else {
			fillEPollEvents(h, evt, new_e);
			//cout << "modified " << h.handle << endl;
			checkError(epoll_ctl(this->handle, EPOLL_CTL_MOD, h.handle, &evt));
			uint32_t ep_e = eventsToEPoll(new_e);
			if (likely(curEvents!=NULL)) for (int i = curIndex; i < curLength; i++) {
				if (curEvents[i].data.ptr == (void*) &h) {
					curEvents[i].events &= ep_e;
					if (curEvents[i].events == 0) curEvents[i].data.ptr = NULL;
				}
			}
		}
	}
	int32_t Poll::_doDispatch(const epoll_event& event) {
		Handle* h = (Handle*) event.data.ptr;
		if (unlikely(h==NULL)) return 0;
//if (unlikely(has_deleted) && tmp_deleted.find(h) != tmp_deleted.end()) return 0;
		int32_t ret = 0;
		EventData evtd;
		event_t evt = (event_t) ePollToEvents(event.events);
		evtd.hungUp = (event.events & EPOLLHUP);
		evtd.error = (event.events & EPOLLERR);
//cerr << "evt=" << (int32_t)evt << endl;

		/*for(int32_t i=0;i<numEvents;i++) {
		 Events e=indexToEvent(i);
		 uint32_t ep=eventToEPoll(e);
		 if(event.events&ep != 0)
		 evt |= (event_t)e;
		 }*/
//cout << "handle fd: " << h->handle << endl;
//cout << "getevents: " << (int32_t)(h->getEvents()) << endl;
		cur_handle = h->handle;
//cur_last_events = h->getEvents();
		ret = h->dispatchMultiple((Events) evt, evtd);
//if (unlikely(has_deleted) && tmp_deleted.find(h) != tmp_deleted.end()) return 0;
//Events new_events = h->getEvents();
//cout << (int32_t)new_events << " " << (int32_t)cur_last_events << endl;
//if (new_events != cur_last_events) _applyHandle(*h, cur_last_events);
//else cout << "new_events==cur_last_events==" << (int32_t)new_events << endl;
		return ret;
	}
	int32_t Poll::_doEPoll(int32_t timeout) {
		if (active <= 0) {
			//printf("active=%i\n", active);
			return -1;
		}
		int32_t ret = 0;
		epoll_event evts[MAX_EVENTS];
		retry: int32_t n = checkError(epoll_wait(handle, evts, MAX_EVENTS, timeout));
		if (unlikely(n < 0)) {
			goto retry;
		}
		if (n <= 0) ret = -1;
		curEvents = evts;
		curLength = n;
		for (curIndex = 0; curIndex < n; curIndex++)
			ret += _doDispatch(evts[curIndex]);

		/*if (likely(!has_deleted)) return ret;
		 for (auto it = tmp_deleted.begin(); it != tmp_deleted.end(); it++)
		 (*it)->release();
		 tmp_deleted.clear();
		 has_deleted = false;*/

		return ret;
	}
	void Poll::dispatch(Events event, const EventData& evtd) {
//throw CPollException("Poll::dispatch() not implemented");
		_doEPoll(0);
	}
	Events Poll::getEvents() {
//throw CPollException("Poll::getEvents() not implemented");
		return active ? (Events::all) : (Events::none);
	}
	int32_t Poll::waitAndDispatch() {
		return _doEPoll(-1);
	}
	void Poll::fillEPollEvents(Handle& h, epoll_event& evt, Events e) {
		evt.events = eventsToEPoll(e);
		evt.data.u64 = 0; //work around valgrind warning
		evt.data.ptr = &h;
	}

	void Poll::applyHandle(Handle& h, Events old_e) {
//cout << "applyHandle" << endl;
//if (h.handle == cur_handle) return;
		_applyHandle(h, old_e);
	}
	void Poll::add(Handle& h) {
//h.retain();
		h.onEventsChange = Delegate<void(Handle&, Events)>(&Poll::applyHandle, this);
		//h.onEventsChange = [this,&h](Events old_events) {this->applyHandle(h,old_events);};
		_applyHandle(h, Events::none);
		h.onClose = Delegate<void(Handle& h)>(&Poll::del, this);
	}
	void Poll::del(Handle& h) {
//h.release();
//tmp_deleted.push_back(&h);
//throw 0;
//printf("Poll::del()\n");
		if (h.getEvents() != Events::none) {
			/*if (h.handle < 0) {
			 //throw runtime_error("test");
			 Events new_e = h.getEvents();
			 EventData evtd;
			 evtd.hungUp = evtd.error = true;
			 while (new_e != Events::none) {
			 h.dispatchMultiple(new_e, evtd);
			 new_e = h.getEvents();
			 }
			 }*/
			//printf("Poll::del()\n");
			//if we're in the middle of a _doEPoll() loop, disable all pending events in queue
			//relating to this handle since it might not even exist anymore after this function
			//returns
			if (likely(curEvents!=NULL)) for (int i = curIndex; i < curLength; i++) {
				if (curEvents[i].data.ptr == (void*) &h) curEvents[i].data.ptr = NULL;
			}
			checkError(epoll_ctl(this->handle, EPOLL_CTL_DEL, h.handle, (epoll_event*) 1));
			h.release();
			//tmp_deleted.insert(&h);
			//has_deleted = true;
			active--;
		}
		h.onEventsChange = nullptr;
		h.onClose = nullptr;
	}

}
