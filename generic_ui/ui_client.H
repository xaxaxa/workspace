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
#include <string>
#include <functional>
#include <stdexcept>
#include <rgc.H>
#include "generic_ui.H"
#include "cout_object.H"
#include <stdlib.h>
#include "ui_include.H"
#include <limits.h>
#include <cpoll.H>
#include <sys/eventfd.h>
#include <pthread.h>
using namespace GenericUI;
using namespace GenericUI::Aliases;
using namespace CP;
using namespace std;
using namespace std::placeholders;
#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif
volatile uint32_t cnt=1;
int8_t* safeMalloc(size_t l) {
	int8_t* tmp=(int8_t*)malloc(l);
	if(tmp==NULL) throw runtime_error("cannot allocate memory");
	return tmp;
}
int8_t* safeRealloc(int8_t* ptr, size_t l) {
	int8_t* tmp=(int8_t*)realloc(ptr, l);
	if(tmp==NULL) throw runtime_error("cannot allocate memory");
	return tmp;
}
/*uint64_t htonll(uint64_t value) {
	// The answer is 42
	static const int32_t num = 42;

	// Check the endianness
	if (*reinterpret_cast<const char*>(&num) == num) {
		const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
		const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

		return (static_cast<uint64_t>(low_part) << 32) | high_part;
	} else {
		return value;
	}
}
uint64_t ntohll(uint64_t value) {
	// The answer is 42
	static const int32_t num = 42;

	// Check the endianness
	if (*reinterpret_cast<const char*>(&num) == num) {
		const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
		const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

		return (static_cast<uint64_t>(low_part) << 32) | high_part;
	} else {
		return value;
	}
}*/
struct tmpbuffer
{
	int8_t* buf;
	int32_t capacity;
	int32_t length;
	tmpbuffer(): buf((int8_t*)safeMalloc(4096)), capacity(4096), length(0)
	{
		
	}
	void reset() {
		length=0;
		if(capacity>4096) {
			if(buf!=NULL)free(buf);
			buf=NULL;
			buf=safeMalloc(4096);
			capacity=4096;
		}
	}
	~tmpbuffer() {
		free(buf);
	}
	void reallocate(int32_t c) {
		do {
			capacity*=2;
		} while(c>capacity);
		buf=(int8_t*)safeRealloc(buf, capacity);
	}
	void ensureCapacity(int32_t c) {
		if(c>capacity)
			reallocate(c);
	}
	inline void write(const int8_t* data, int32_t len) {
		if(unlikely(length+len>capacity)) {
			reallocate(length+len);
		}
		memcpy(buf+length, data, len);
		length+=len;
	}
	
	//DMA-like zero-copy scheme; does not do
	//any sanity checking
	int8_t* beginWrite(int len) {
		if(unlikely(length+len>capacity))
			reallocate(length+len);
		return buf+length;
	}
	void endWrite(int len) {
		length+=len;
	}
};
struct scopeLock
{
	pthread_mutex_t& mutex;
	scopeLock(pthread_mutex_t& m):mutex(m)
	{pthread_mutex_lock(&mutex);}
	~scopeLock()
	{pthread_mutex_unlock(&mutex);}
};
void initMutex(pthread_mutex_t& mutex) {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	int e;
	if((e=pthread_mutex_init(&mutex,&attr))!=0) {
		const char* err=strerror(e);
		throw runtime_error("mutex initialization failed: "+string(err));
	}
	pthread_mutexattr_destroy(&attr);
}

//thread safe
class FDPool: public RGC::Object
{
public:
	vector<int32_t> fds;
	function<int32_t()> cb;
	pthread_mutex_t mutex;
	int32_t max;
	FDPool(const function<int32_t()>& cb, int32_t max=3): cb(cb), max(max) {
		initMutex(mutex);
	}
	~FDPool() {
		pthread_mutex_destroy(&mutex);
		for(size_t i=0;i<fds.size();i++) {
			close(fds[i]);
		}
	}
	int32_t get() {
		scopeLock l(mutex);
		if(fds.size()<=0) return cb();
		else {
			int32_t fd=fds[fds.size()-1];
			fds.resize(fds.size()-1);
			return fd;
		}
	}
	void put(int32_t fd) {
		scopeLock l(mutex);
		if((int32_t)fds.size()<max) fds.push_back(fd);
		else close(fd);
	}
};

struct IDLETHREAD
{
	function<void()> func;
	function<void()> cb;
	pthread_t thread;
	//void* arg;
	FDPool& fdp;
	int32_t efd;
	enum {
		none=0,
		invoke,
		kill
	} op;
	IDLETHREAD(FDPool& fdp): fdp(fdp), efd(fdp.get()),op(none) {}
	~IDLETHREAD() {
		fdp.put(efd);
	}
	void signal() {
		eventfd_t i=1;
		eventfd_write(efd, i);
	}
};
void* idleThread(void* v) {
	while(true) {
		{
			eventfd_t i;
			eventfd_read(((IDLETHREAD*)v)->efd, &i);
		}
		switch(((IDLETHREAD*)v)->op) {
		case IDLETHREAD::invoke:
			try {
				((IDLETHREAD*)v)->func();
			} catch(...) {}
			((IDLETHREAD*)v)->cb();
			break;
		case IDLETHREAD::kill:
			delete (IDLETHREAD*)v;
			return NULL;
		default:
			break;
		}
	}
}


//protected by mutex; all methods are thread-safe
class ThreadPool: public RGC::Object
{
public:
	FDPool fdp;
	vector<IDLETHREAD*> threads;
	pthread_mutex_t mutex;
	int32_t max;
	ThreadPool(int32_t max=3): fdp([]()->int32_t{return eventfd(0, EFD_SEMAPHORE);}), max(max) {
		initMutex(mutex);
	}
	~ThreadPool() {
		pthread_mutex_destroy(&mutex);
		for(size_t i=0;i<threads.size();i++) {
			threads[i]->op=IDLETHREAD::kill;
			threads[i]->signal();
		}
	}
	IDLETHREAD* create() {
		IDLETHREAD* tmp=new IDLETHREAD(fdp);
		if(pthread_create(&tmp->thread,NULL,idleThread,(void*)tmp)==0)
			return tmp;
		else {
			delete tmp;
			return NULL;
		}
	}
	IDLETHREAD* get() {
		size_t threadcount;
		{
			scopeLock l(mutex);
			threadcount=threads.size();
		}
		if(threadcount<=0) return create();
		else {
			scopeLock l(mutex);
			IDLETHREAD* t=threads[threads.size()-1];
			threads.resize(threads.size()-1);
			return t;
		}
	}
	void put(IDLETHREAD* t) {
		bool b;
		{
			scopeLock l(mutex);
			if((b=((int32_t)threads.size()<max))) threads.push_back(t);
		}
		if(!b) {
			t->op=IDLETHREAD::kill;
			t->signal();
		}
	}
	void invoke(const function<void()>& func) {
		IDLETHREAD* t=get();
		t->cb=[this,t]() {
			put(t);
		};
		t->func=func;
		t->op=IDLETHREAD::invoke;
		t->signal();
	}
};
class UIClient: public RGC::Object
{
public:
	Instance* inst;
	ThreadPool tp;
	FDPool fdp;
	volatile uint64_t call_id;
	bool allow_module_loading;
	struct CALLERINFO {
		int32_t efd;
		O ret; //return value
		CALLERINFO(int32_t efd): efd(efd) {}
		
	};
	map<uint64_t, CALLERINFO> callers;
	function<void(const int8_t*, int32_t)> onWrite;
	tmpbuffer readBuffer;
	//pthread_mutex_t fdp_mutex;
	pthread_mutex_t write_mutex;
	pthread_mutex_t mutex;
	
	UIClient(): fdp([]()->int32_t{return eventfd(0, EFD_SEMAPHORE);}), call_id(1) {
		allow_module_loading=false;
		initMutex(mutex);
		initMutex(write_mutex);
	}
	~UIClient() {
		pthread_mutex_destroy(&write_mutex);
		pthread_mutex_destroy(&mutex);
	}
	//writes struct generic_ui_cmd, iface_name, and slot_name, but does
	//not write payload
	void __write_partial(const generic_ui_cmd& tmp, GenericUI::EventData& ed) {
		onWrite((int8_t*)&tmp, sizeof(tmp));
		if(ed.interface!=NULL) {
			onWrite((int8_t*)ed.interface->name.c_str(), ed.interface->name.length());
		}
		if(ed.slot!=NULL) {
			onWrite((int8_t*)ed.slot->name.c_str(), ed.slot->name.length());
		}
	}
	void __write(EventTypes e, generic_ui_cmd& tmp, GenericUI::EventData& ed) {
		if(ed.o.type==Types::undefined) {
			tmp.payload_len=htonl(0);
			__write_partial(tmp,ed);
		} else {
			tmpbuffer buf;
			std::function<void(tmpbuffer&,const int8_t*,int32_t)> tmpf=&tmpbuffer::write;
			
			ed.o.serialize(bind(tmpf, buf, _1, _2));
			tmp.payload_len=htonl(buf.length);
			__write_partial(tmp,ed);
			onWrite(buf.buf, buf.length);
		}
	}
	bool cb(EventTypes e, GenericUI::EventData& ed)
	{
		generic_ui_cmd tmp;
		memset(&tmp, 0, sizeof(tmp));
		tmp.e=e;
		//uint32_t tmp_i;
		if(e==EventTypes::ret)
			tmp.id=htonll(ed.scope);
		else
			tmp.id=htonll(ed.scope=__sync_fetch_and_add(&call_id,1));
		
		tmp.iface_name_len=htonl((ed.interface==NULL)?0:ed.interface->name.length());
		tmp.slot_name_len=htonl((ed.slot==NULL)?0:ed.slot->name.length());
		
		if(e!=EventTypes::invocationSync) {
			scopeLock l(write_mutex);
			__write(e, tmp, ed);
		}
		
		std::function<bool(UIClient*,EventTypes,GenericUI::EventData&)> tmpcb=&UIClient::cb;
		std::function<bool(EventTypes,GenericUI::EventData&)> tmpcb1=bind(tmpcb,this,_1,_2);
		
		switch(e) {
			case EventTypes::newInterface:
				fprintf(stderr, "debug_ui: newInterface: %s\n", ed.name.c_str());
				ed.interface->listen(tmpcb1);
				break;
			case EventTypes::newSlot:
				fprintf(stderr, "debug_ui: newSlot: %s.%s\n", ed.interface->name.c_str(), ed.name.c_str());
				ed.slot->listen(tmpcb1);
				break;
			case EventTypes::newAttachment:
				fprintf(stderr, "debug_ui: newAttachment: %s.%s\n", ed.interface->name.c_str(), ed.name.c_str());
				break;
			case EventTypes::invocation:
			{
				//uint32_t tmp_i=__sync_fetch_and_add(&call_id,1);
				fprintf(stderr, "debug_ui: invocation %lli: %s.%s; arg=", ed.scope, ed.interface->name.c_str(), ed.name.c_str());
				//*(uint32_t*)&ed.scope = tmp_i;
				cerr << ed.o << endl;
				return false;
			}
			case EventTypes::invocationSync:
			{
				//uint32_t tmp_i=__sync_fetch_and_add(&call_id,1);
				fprintf(stderr, "debug_ui: invocationSync %lli: %s.%s; arg=", ed.scope, ed.interface->name.c_str(), ed.name.c_str());
				//*(uint32_t*)&ed.scope = tmp_i;
				cerr << ed.o << endl;
				int32_t efd;
				{
					efd=fdp.get();
					scopeLock l(mutex);
					callers.insert({ed.scope,efd});
				}
				{
					scopeLock l(write_mutex);
					__write(e, tmp, ed);
				}
				eventfd_t efddata;
				eventfd_read(efd, &efddata);
				{
					scopeLock l(mutex);
					auto it=callers.find(ed.scope);
					ed.o=(*it).second.ret;
					callers.erase(ed.scope);
				}
				fdp.put(efd);
				return false;
			}
			case EventTypes::ret:
				fprintf(stderr, "debug_ui: ret %i: %s.%s; val=", *(uint32_t*)&ed.scope, ed.interface->name.c_str(), ed.name.c_str());
				cerr << ed.o << endl;
				break;
			case EventTypes::loadUI:
				fprintf(stderr, "debug_ui: loadUI: %s\n", ed.name.c_str());
				break;
			default:
				fprintf(stderr, "debug_ui: unknown event %i\n", (int)e);
		}
		return false;
	}
	int8_t* beginPutData(int length) {
		return readBuffer.beginWrite(length);
	}
	
	int32_t processBuffer(int8_t* b, int32_t length) {
		if(length<(int)sizeof(generic_ui_cmd)) return 0;
		generic_ui_cmd* tmp=(generic_ui_cmd*)b;
		int32_t iface_name_len=ntohl(tmp->iface_name_len);
		int32_t slot_name_len=ntohl(tmp->slot_name_len);
		int32_t payload_len=ntohl(tmp->payload_len);
		uint64_t id=ntohll(tmp->id);
		int64_t l=sizeof(generic_ui_cmd)+iface_name_len+slot_name_len+payload_len;
		if(l>(int64_t)INT_MAX) throw overflow_error("protocol error: overflow");
		if(length<(int)l) return 0;
		
		const int8_t* dataptr=b+sizeof(generic_ui_cmd);
		const int8_t* payload=dataptr+iface_name_len+slot_name_len;
		switch(tmp->e) {
			case EventTypes::invocation:
			{
				O o;
				if(payload_len>0) {
					int32_t offset=0;
					o.deserialize([&offset,payload,payload_len](int8_t* buf, int len) {
						if(offset+len>payload_len) throw overflow_error("overflow while deserializing");
						memcpy(buf,payload+offset,len);
						offset+=len;
					});
				}
				auto iface=&(inst->attach(string((char*)dataptr, iface_name_len)));
				auto slot=&(iface->attach(string((char*)(dataptr+iface_name_len), slot_name_len)));
				tp.invoke([iface, slot, this, id, o]() {
					O ret=slot->invokeLocal(o);
					GenericUI::EventData ed{string(), iface, slot, ret, id, nullptr};
					cb(EventTypes::ret, ed);
				});
				break;
			}
			case EventTypes::ret:
				{
					O o;
					if(payload_len>0) {
						int32_t offset=0;
						o.deserialize([&offset,payload,payload_len](int8_t* buf, int len) {
							if(offset+len>payload_len) throw overflow_error("overflow while deserializing");
							memcpy(buf,payload+offset,len);
							offset+=len;
						});
					}
					scopeLock l(mutex);
					auto it=callers.find(id);
					if(it==callers.end()) {
						printf("ui_client warning: received EventTypes::ret with an unknown id %lli", id);
						break;
					}
					(*it).second.ret=o;
					eventfd_write((*it).second.efd, 1);
				}
				break;
			case EventTypes::loadUI:
				break;
			default: break;
		}
		return l;
	}
	
	void checkBuffer() {
		int32_t offset=0;
		int32_t tmp;
		while((tmp=processBuffer(readBuffer.buf+offset, readBuffer.length-offset))>0)
			offset+=tmp;
		if(offset>=readBuffer.length) {
			readBuffer.reset();
		} else {
			memmove(readBuffer.buf, readBuffer.buf+offset, readBuffer.length-offset);
			readBuffer.length-=offset;
		}
	}
	void endPutData(int32_t length) {
		if(unlikely(length<=0)) return;
		readBuffer.endWrite(length);
		checkBuffer();
	}
	void doListen() {
		std::function<bool(UIClient*,EventTypes,GenericUI::EventData&)> tmpcb=&UIClient::cb;
		std::function<bool(EventTypes,GenericUI::EventData&)> tmpcb1=bind(tmpcb,this,_1,_2);
		inst->listen(tmpcb1);
	}
};


