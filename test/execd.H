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
 
#ifndef EXECD_H
#define EXECD_H

#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif
#define DEBUGPRINT(...) fprintf(stderr,__VA_ARGS__)
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include <errno.h>
#include <stdexcept>
#include <exception>
#include <generic_struct.H>
#include <limits.h>
#include <cpoll.H>
#include <sys/wait.h>
#include <functional>

#include <cout_object.H>
using namespace std;
using namespace std::placeholders;
using namespace GenericStruct;
using namespace GenericStruct::Aliases;
#define BUFSIZE 4096
namespace EXECD
{
	/*
	 * format for requests(client to daemon):
	 * {cmds command, uint32 reqid, Array params}
	 * cmdid is an arbitrary integer chosen by the client
	 * for identifying the response
	 * 
	 * format for events(daemon to client):
	 * {events evt, Array params}
	 * 
	 * format of "params" for responses(events beginning with r_)
	 * {uint32 reqid, ...}
	 * 
	 * */

	template<class T> inline T checkError(T asdfg) {
		if(asdfg<0) throw runtime_error(strerror(errno));
		return asdfg;
	}
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
	struct execd_header {
		int32_t obj_len;
	};
	///typedef int error_t
	enum class cmds: int8_t {
		//command[0] is executable name, command[1] is argv[0]
		execute=1,			///{pid_t, error_t} execute(string[] command, int taskid, bool returnOutput);
		setReturnOutput,	///void setReturnOutput(int taskid, bool returnOutput);		//taskid is not request id
		kill,				///{error_t} kill(int taskid, int signal);
		sendInput			///{error_t} sendInput(int taskid, string data);
	};
	
	//for responses, params[0] is always cmdid
	//the cmdid parameter is ommited here
	enum class events: int8_t {
		r_execute=1,		///{int cmdid, error_t err, pid_t pid}
		r_setReturnOutput,	///{int cmdid, error_t err}
		r_kill,				///{int cmdid, error_t err}
		r_sendInput,		///{int cmdid, error_t err}
		output,				///{int id, int fd, string data}
		///fd is from the process's point of view
		exit				///{int id, int ret}
	};
	
	
	class execd_client
	{
	public:
		function<void(int8_t*,int32_t)> onWrite;
		function<void(O o)> onEvent;
		tmpbuffer readBuffer;
		pthread_mutex_t write_lock;
		int32_t cmdid,taskid;
		execd_client(): cmdid(1), taskid(1) {
			initMutex(write_lock);
		}
		~execd_client() {
			pthread_mutex_destroy(&write_lock);
		}
		void send(O o) {
			execd_header h;
			tmpbuffer buf;
			std::function<void(tmpbuffer*,const int8_t*,int32_t)> tmpf=&tmpbuffer::write;
			o.serialize(bind(tmpf, &buf, _1, _2));
			h.obj_len=htonl(buf.length);
			scopeLock l(write_lock);
			onWrite((int8_t*)&h, sizeof(h));
			onWrite(buf.buf, buf.length);
		}
		int32_t processBuffer(int8_t* b, int32_t len) {
			if(len<(int32_t)sizeof(execd_header)) return 0;
			execd_header* h=(execd_header*)b;
			int32_t obj_len=ntohl(h->obj_len);
			int64_t tmp=((int64_t)obj_len+(int64_t)sizeof(execd_header));
			if(tmp>(int64_t)INT_MAX) throw overflow_error("protocol error: overflow");
			if(len<(int32_t)tmp) return 0;
			
			const int8_t* objdata=b+(int32_t)sizeof(execd_header);
			O o;
			if(obj_len>0) {
				DEBUGPRINT("deserializing data... (%i bytes)\n", obj_len);
				int32_t offset=0;
				o.deserialize([&offset,objdata,obj_len](int8_t* buf, int len) {
					if(offset+len>obj_len) throw overflow_error("overflow while deserializing");
					//DEBUGPRINT("objdata[offset]=%i\n", objdata[offset]);
					memcpy(buf,objdata+offset,len);
					offset+=len;
				});
				DEBUGPRINT("offset=%i\n", offset);
			}
			onEvent(o);
			return (int32_t)tmp;
		}
		void checkBuffer() {
			DEBUGPRINT("checking buffer...\n");
			int32_t offset=0;
			int32_t tmp;
			while((tmp=processBuffer(readBuffer.buf+offset, readBuffer.length-offset))>0)
				offset+=tmp;
			if(offset>=readBuffer.length) {
				readBuffer.reset();
			} else if(offset>0) {
				memmove(readBuffer.buf, readBuffer.buf+offset, readBuffer.length-offset);
				readBuffer.length-=offset;
			}
		}
		void* beginPutData(int32_t len) {
			return readBuffer.beginWrite(len);
		}
		void endPutData(int32_t len) {
			if(unlikely(len<=0)) return;
			DEBUGPRINT("got %i bytes of data...\n", len);
			readBuffer.endWrite(len);
			checkBuffer();
		}
		RGC::Ref<CP::File> inp;
		RGC::Ref<CP::Poll> p;
		void beginReadStream() {
			DEBUGPRINT("beginReadStream()\n");
			int8_t* buf=(int8_t*)beginPutData(BUFSIZE);
			inp->read(buf, BUFSIZE, [this](int l) {
				DEBUGPRINT("read cb called... l=%i\n",l);
				if(l==0) return;
				endPutData(l);
				beginReadStream();
			});
		}
		void start(CP::Poll& p, CP::File& f) {
			inp=&f;
			this->p=&p;
			p.add(f);
			beginReadStream();
		}
		void do_request(cmds cmd, A params) {
			send(A{O((int8_t)cmd),
				O(__sync_fetch_and_add(&cmdid,1)),O(params)});
		}
		//returns: taskid
		int32_t do_exec(A command, bool returnOutput) {
			int32_t tid=__sync_fetch_and_add(&taskid,1);
			do_request(cmds::execute, {O(command), O(tid),
				O(int8_t(returnOutput?1:0))});
			return tid;
		}
		void do_setReturnOutput(int32_t taskid, bool returnOutput) {
			do_request(cmds::setReturnOutput, {O(taskid), O(int8_t(returnOutput?1:0))});
		}
		void do_kill(int32_t taskid, int32_t sig) {
			do_request(cmds::kill, {O(taskid), O(sig)});
		}
		void do_sendInput(int32_t taskid, A data) {
			do_request(cmds::sendInput, {O(taskid), O(data)});
		}
	};
}
#endif
