/*
 * cpoll.C
 *
 *  Created on: 2012-09-14
 *      Author: xaxaxa
 */

#include "cpoll.H"
#include "cpoll_internal.H"

namespace CP
{
	//CPollException

	CPollException::CPollException() :
			message(strerror(errno)), number(errno) {
	}
	CPollException::CPollException(int number) :
			message(strerror(number)), number(number) {
	}
	CPollException::CPollException(string message, int number) :
			message(message), number(number) {
	}
	CPollException::~CPollException() throw () {
	}
	const char* CPollException::what() const throw () {
		return message.c_str();
	}

	vector<RGC::Ref<EndPoint> > EndPoint::lookupHost(const char* hostname, const char* port,
			int flags, int socktype, int proto) {
		vector<RGC::Ref<EndPoint> > tmp;
		addrinfo hints, *result, *rp;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */
		hints.ai_socktype = socktype;
		hints.ai_flags = flags;
		hints.ai_protocol = proto;

		int s = getaddrinfo(hostname, port, &hints, &result);
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
	EndPoint* EndPoint::create(int AddressFamily) {
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
	IPEndPoint::IPEndPoint(IPAddress address, int port) {
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
	int IPEndPoint::getSockAddrSize() const {
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
		s << address.tostr() << ':' << port;
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
	int IPv6EndPoint::getSockAddrSize() const {
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
	string IPv6EndPoint::tostr() const {
		stringstream s;
		s << '[' << address.tostr() << "]:" << port;
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
	int UNIXEndPoint::getSockAddrSize() const {
		return sizeof(sa_family_t) + name.length() + 1;
	}
	void UNIXEndPoint::clone(EndPoint& to) const {
		if (to.addressFamily != addressFamily) throw CPollException(
				"attempting to clone an EndPoint to another EndPoint with a different addressFamily");
		UNIXEndPoint& tmp((UNIXEndPoint&) to);
		tmp.name = name;
	}

	Handle::Handle(HANDLE handle) :
			handle(checkError(handle)), __deleted(false) {
	}
	int Handle::dispatchMultiple(Events events, const EventData& evtd) {
		//cout << (int)events << endl;
		int ret = 0;
		for (int i = 0; i < numEvents; i++) {
			Events e = indexToEvent(i);
			//cout << (int)e << " " << (((event_t)e)&((event_t)events)) << endl;
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
		/*for(int i=0;i<numEvents;i++) {
		 Events e=indexToEvent(i);
		 short p=eventToPoll(e);
		 if(p&pfd.revents!=0) events=(Events)((event_t)events | (event_t)e);
		 }*/
		return pollToEvents(pfd.revents);
	}
	int Handle::waitAndDispatch() {
		EventData evtd;
		Events e = wait(evtd);
		return dispatchMultiple(e, evtd);
	}
	void Handle::loop() {
		while (waitAndDispatch() > 0)
			;
	}
	/*void Handle::close() {
	 ::close(handle);
	 }*/
	Handle::~Handle() {
		::close(handle);
	}

	//File

	Events File::_getEvents() {
		Events e = Events::none;
		for (int i = 0; i < numEvents; i++)
			if (eventData[i].state != EventHandlerData::States::invalid) e = (Events) (((event_t) e)
					| ((event_t) indexToEvent(i)));
		//cout << "_getEvents: " << (int)e << endl;
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
		if (onEventsChange != nullptr) onEventsChange(old_events);
	}
	int File::read(void* buf, int len) {
		return ::read(handle, buf, len);
	}
	int File::write(const void* buf, int len) {
		return ::write(handle, buf, len);
	}
	int File::send(const void* buf, int len, int flags) {
		return ::send(handle, buf, len, flags);
	}
	int File::recv(void* buf, int len, int flags) {
		return ::recv(handle, buf, len, flags);
	}
	void File::doOperation(EventHandlerData& ed, const EventData& evtd) {
		Operations op = ed.op;
		int r = 0;
		switch (op) {
			case Operations::read:
				r = read(ed.misc.bufferIO.buf, ed.misc.bufferIO.len);
				break;
			case Operations::write:
				r = write(ed.misc.bufferIO.buf, ed.misc.bufferIO.len);
				break;
			case Operations::writeAll:
				r = write(((char*)ed.misc.bufferIO.buf)
					+ ed.misc.bufferIO.len_done,
					ed.misc.bufferIO.len - ed.misc.bufferIO.len_done);
				//cout << "wrote " << r << " bytes on fd " << handle << endl;
				if(r<=0) {
					ed.state = EventHandlerData::States::invalid;
					ed.cb(ed.misc.bufferIO.len_done == 0 ?
						r : ed.misc.bufferIO.len_done);
				}
				ed.misc.bufferIO.len_done+=r;
				//cout << "len_done = " << ed.misc.bufferIO.len_done
				//		<< " of " << ed.misc.bufferIO.len << endl;
				if(ed.misc.bufferIO.len_done >= ed.misc.bufferIO.len) {
					ed.state = EventHandlerData::States::invalid;
					ed.cb(ed.misc.bufferIO.len_done);
				}
				return;
			case Operations::recv:
				r = recv(ed.misc.bufferIO.buf, ed.misc.bufferIO.len,
					ed.misc.bufferIO.flags);
				break;
			case Operations::send:
				r = send(ed.misc.bufferIO.buf, ed.misc.bufferIO.len,
					ed.misc.bufferIO.flags);
				break;
			case Operations::sendAll:
				r = send(((char*)ed.misc.bufferIO.buf)
					+ ed.misc.bufferIO.len_done,
					ed.misc.bufferIO.len - ed.misc.bufferIO.len_done,
					ed.misc.bufferIO.flags);
				if(r<=0) {
					ed.state = EventHandlerData::States::invalid;
					ed.cb(ed.misc.bufferIO.len_done == 0 ?
						-1 : ed.misc.bufferIO.len_done);
				}
				ed.misc.bufferIO.len_done += r;
				if(ed.misc.bufferIO.len_done >= ed.misc.bufferIO.len) {
					ed.state = EventHandlerData::States::invalid;
					ed.cb(ed.misc.bufferIO.len_done);
				}
				return;
			default:
				break;
		}
		if(r<=0) {
			//invalidate the current event listener
			ed.state = EventHandlerData::States::invalid;
		}
		ed.cb(r);
	}
	void File::dispatch(Events event, const EventData& evtd) {
		//cout << (int)event << " dispatched" << endl;
		EventHandlerData& ed = eventData[eventToIndex(event)];
		if (ed.state == EventHandlerData::States::invalid) return;

		if ((ed.state == EventHandlerData::States::once) || evtd.hungUp || evtd.error) ed.state =
				EventHandlerData::States::invalid;

		doOperation(ed, evtd);
		//cout << "clear event" << endl;
		Events old_events = _getEvents();
		//cout << (eventData[eventToIndex(event)].state==EventHandlerData::States::once
		//		? "true" : "false") << endl;
		//if(evtd.hungUp) cout << "socket hung up" << endl;
		//if(evtd.error) cout << "socket error" << endl;
		//if(!b) cout << "doOperation returned false" << endl;
		//cout << "b=" << b << endl;
		///if (!b) ed.state = EventHandlerData::States::invalid;
		
		//else cout << (int)eventData[eventToIndex(event)].state << endl;
		if (onEventsChange != nullptr) onEventsChange(old_events);
	}
	void File::fillIOEventHandlerData(EventHandlerData* ed, void* buf, int len, const Callback& cb,
			Events e, Operations op) {
		ed->cb = cb;
		ed->misc.bufferIO.buf = buf;
		ed->misc.bufferIO.len = len;
		ed->op = op;
	}
	void File::read(void* buf, int len, const Callback& cb, bool repeat) {
		if(!_supportsEPoll) {
		asdfg:
			int r=read(buf, len);
			cb(r);
			if(repeat && r>0) goto asdfg;
			return;
		}
		static const Events e = Events::in;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, buf, len, cb, e, Operations::read);
		endAddEvent(e, repeat);
	}
	void File::write(const void* buf, int len, const Callback& cb, bool repeat) {
		if(!_supportsEPoll) {
			asdfg:
			int r=write(buf, len);
			cb(r);
			if(repeat && r>0) goto asdfg;
			return;
		}
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, (void*) buf, len, cb, e, Operations::write);
		endAddEvent(e, repeat);
	}
	void File::writeAll(const void* buf, int len, const Callback& cb) {
		if(!_supportsEPoll) {
			int bw=0, bw1=-2;
			while(bw<len && (bw1=write(((char*)buf)+bw, len-bw))>0)
				bw+=bw1;
			cb(bw1==-2?-1:bw);
			return;
		}
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, (void*) buf, len, cb, e, Operations::writeAll);
		ed->misc.bufferIO.len_done = 0;
		endAddEvent(e, true);
	}
	void File::recv(void* buf, int len, int flags, const Callback& cb, bool repeat) {
		static const Events e = Events::in;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, buf, len, cb, e, Operations::recv);
		ed->misc.bufferIO.flags = flags;
		endAddEvent(e, repeat);
	}
	void File::send(const void* buf, int len, int flags, const Callback& cb, bool repeat) {
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, (void*) buf, len, cb, e, Operations::send);
		ed->misc.bufferIO.flags = flags;
		endAddEvent(e, repeat);
	}
	void File::sendAll(const void* buf, int len, int flags, const Callback& cb) {
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		fillIOEventHandlerData(ed, (void*) buf, len, cb, e, Operations::sendAll);
		ed->misc.bufferIO.len_done = 0;
		ed->misc.bufferIO.flags = flags;
		endAddEvent(e, true);
	}
	
	

	//Socket

	Socket::Socket(HANDLE handle, int d, int t, int p) :
			File(handle, true), addressFamily(d), type(t), protocol(p) {
	}
	Socket::Socket(int d, int t, int p) :
			File(checkError(socket(d, t, p)), true), addressFamily(d), type(t), protocol(p) {
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
	void Socket::doOperation(EventHandlerData& ed, const EventData& evtd) {
		Operations op = ed.op;
		switch (op) {
			case Operations::accept:
				ed.cb((evtd.hungUp | evtd.error) ? -1 : 0);
				break;
			case Operations::shutdown:
				ed.cb(shutdown(ed.misc.shutdown.how));
				break;
			default:
				File::doOperation(ed, evtd);
		}
	}
	void Socket::connect(const sockaddr *addr, int addr_size) {
		retry: int tmp = ::connect(handle, addr, addr_size);
		if (tmp != 0 && errno != EINPROGRESS) {
			if (errno == EINTR) goto retry;
			throw CPollException(errno);
		}
	}
	void Socket::bind(const sockaddr *addr, int addr_size) {
		int tmp12345 = 1;
		setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &tmp12345, sizeof(tmp12345));
		if (::bind(handle, addr, addr_size) != 0) throw CPollException(errno);
	}
	void Socket::bind(const EndPoint &ep) {
		int size = ep.getSockAddrSize();
		uint8_t tmp[size];
		ep.getSockAddr((sockaddr*) tmp);
		bind((sockaddr*) tmp, size);
	}
	void Socket::listen(int backlog) {
		checkError(::listen(handle, backlog));
	}
	int Socket::shutdown(int how) {
		return ::shutdown(handle, how);
	}
	void Socket::shutdown(int how, const Callback& cb) {
		static const Events e = Events::out;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = cb;
		ed->op = Operations::shutdown;
		endAddEvent(e, false);
	}
	//the caller must release() or free() the returned object;
	//will NOT automatically add the new socket to its Poll instance
	//because the user might want to handle the socket on a different thread
	//which requires a different Poll instance
	Socket* Socket::accept() {
		EndPoint* ep = EndPoint::create(addressFamily);
		socklen_t l = (socklen_t) (ep->getSockAddrSize());
		char addr[l];
		HANDLE h = ::accept(handle, (struct sockaddr*) addr, &l);
		if (h < 0) {
			int e = errno;
			ep->release();
			onError(e);
		}
		ep->setSockAddr((struct sockaddr*) addr);
		ep->release();
		Socket* sock = new Socket(h, addressFamily, type, protocol);
		//int tmp12345 = 1;
		//setsockopt(h, SOL_SOCKET, SO_REUSEADDR, &tmp12345, sizeof(tmp12345));
		//sock->peer=ep;
		return sock;
	}
	//callback function must release() or free() the received object
	void Socket::accept(const function<void(Socket*)>& cb, bool repeat) {
		static const Events e = Events::in;
		EventHandlerData* ed = beginAddEvent(e);
		ed->cb = [cb,this](int i) {
			Socket* s=accept();
			cb(s);
		};
		ed->op = Operations::accept;
		endAddEvent(e, repeat);
	}
	void Socket::connect(const EndPoint &ep) {

	}
	
	
	
	//SignalFD
	int SignalFD::MAX_EVENTS(4);
	SignalFD::SignalFD(HANDLE handle, const sigset_t& mask):
		Handle(handle), mask(mask) {}
	SignalFD::SignalFD(const sigset_t& mask, int flags):
		Handle(signalfd(-1, &mask, flags)), mask(mask) {}
	void SignalFD::dispatch(Events event, const EventData& evtd) {
		Signal sig[MAX_EVENTS];
		int br=::read(handle, sig, sizeof(sig));
		if(callback != nullptr) {
			br /= sizeof(Signal);
			for(int i=0; i<br; i++) {
				callback(sig[i]);
			}
		}
	}
	Events SignalFD::getEvents() {
		return Events::in;
	}
	
	

	//Poll
	int Poll::MAX_EVENTS(32);
	Poll::Poll(HANDLE handle) :
			Handle(handle), active(0), cur_handle(-1) {
		disableSignals();
	}
	Poll::Poll() :
			Handle(checkError(epoll_create1(0))), active(0), cur_handle(-1) {
		disableSignals();
	}
	void Poll::_applyHandle(Handle& h, Events old_e) {
		Events new_e = h.getEvents();
		if (new_e == old_e) return;
		epoll_event evt;
		if (old_e == Events::none) {
			fillEPollEvents(h, evt, new_e);
			//cout << "added " << h.handle << endl;
			checkError(epoll_ctl(this->handle, EPOLL_CTL_ADD, h.handle, &evt));
			h.retain();
			active++;
		} else if (new_e == Events::none) {
			//cout << "deleted " << h.handle << endl;
			checkError(epoll_ctl(this->handle, EPOLL_CTL_DEL, h.handle, NULL));
			h.release();
			active--;
		} else {
			fillEPollEvents(h, evt, new_e);
			//cout << "modified " << h.handle << endl;
			checkError(epoll_ctl(this->handle, EPOLL_CTL_MOD, h.handle, &evt));
		}
	}
	int Poll::_doDispatch(const epoll_event& event) {
		Handle* h = (Handle*) event.data.ptr;
		if (h->__deleted) return 0;
		int ret = 0;
		EventData evtd;
		event_t evt = (event_t) ePollToEvents(event.events);
		evtd.hungUp = (event.events & EPOLLHUP);
		evtd.error = (event.events & EPOLLERR);
		//cerr << "evt=" << (int)evt << endl;

		/*for(int i=0;i<numEvents;i++) {
		 Events e=indexToEvent(i);
		 uint32_t ep=eventToEPoll(e);
		 if(event.events&ep != 0)
		 evt |= (event_t)e;
		 }*/
		//cout << "handle fd: " << h->handle << endl;
		//cout << "getevents: " << (int)(h->getEvents()) << endl;
		cur_handle = h->handle;
		cur_last_events = h->getEvents();
		ret = h->dispatchMultiple((Events) evt, evtd);
		Events new_events = h->getEvents();
		//cout << (int)new_events << " " << (int)cur_last_events << endl;
		if (new_events != cur_last_events) _applyHandle(*h, cur_last_events);
		//else cout << "new_events==cur_last_events==" << (int)new_events << endl;
		return ret;
	}
	int Poll::_doEPoll(int timeout) {
		if (active <= 0) return 0;
		int ret = 0;
		epoll_event evts[MAX_EVENTS];
		retry: int n = checkError(epoll_wait(handle, evts, MAX_EVENTS, timeout));
		if (n < 0) {
			if (errno == EINTR) goto retry;
			else throw CPollException();
		}
		for (int i = 0; i < n; i++)
			ret += _doDispatch(evts[i]);
		for (unsigned int i = 0; i < tmp_deleted.size(); i++)
			tmp_deleted[i]->release();
		tmp_deleted.resize(0);
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
	int Poll::waitAndDispatch() {
		return _doEPoll(-1);
	}
	void Poll::fillEPollEvents(Handle& h, epoll_event& evt, Events e) {
		evt.events = eventsToEPoll(e);
		evt.data.u64 = 0; //work around valgrind warning
		evt.data.ptr = &h;
	}

	void Poll::applyHandle(Handle& h, Events old_e) {
		//cout << "applyHandle" << endl;
		if (h.handle == cur_handle) return;
		_applyHandle(h, old_e);
	}
	void Poll::add(Handle& h) {
		//h.retain();
		h.__deleted = false;
		h.onEventsChange = [this,&h](Events old_events) {this->applyHandle(h,old_events);};
		_applyHandle(h, Events::none);
	}
	void Poll::del(Handle& h) {
		//h.release();
		//tmp_deleted.push_back(&h);
		h.__deleted = true;
		if (h.getEvents() != Events::none) {
			checkError(epoll_ctl(this->handle, EPOLL_CTL_DEL, h.handle, (epoll_event*) 1));
			//h.release();
			tmp_deleted.push_back(&h);
			active--;
		}
	}

}
