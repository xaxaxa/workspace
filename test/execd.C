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

#include "execd.H"
using namespace std;
using namespace std::placeholders;
using namespace GenericStruct;
using namespace GenericStruct::Aliases;
using namespace EXECD;



void* waiterThread(void* v);
struct waiter
{
	pthread_t thread;
	function<void(pid_t,int32_t)> cb;
	int32_t efd;
	waiter(const function<void(pid_t,int)>& cb): cb(cb) {
		efd=checkError(eventfd(0, EFD_SEMAPHORE));
		pthread_create(&thread,NULL,waiterThread,(void*)this);
	}
	~waiter() {
		pthread_kill(thread, 9);
		void* retval;
		pthread_join(thread, &retval);
		//pthread_destroy(thread);
	}
	void notifyFork() {
		eventfd_write(efd, 1);
	}
};

void* waiterThread(void* v) {
	waiter* inf=(waiter*)v;
	while(true) {
		eventfd_t evt;
		int32_t status;
		eventfd_read(inf->efd, &evt);
		while(true) {
			pid_t pid=::wait(&status);
			if(pid<1) break;
			//process the exited child
			inf->cb(pid, status);
		}
	}
	return NULL;
}


class execd
{
public:
	function<void(int8_t*,int32_t)> onWrite;
	function<void()> onEnd;
	EXECD::tmpbuffer readBuffer;
	void* beginPutData(int32_t len) {
		return readBuffer.beginWrite(len);
	}
	struct procInfo {
		//pipes
		int32_t stdin;
		int32_t stdout;
		int32_t stderr;
		//process id
		pid_t pid;
		bool returnOutput;
		void kill(int32_t sig) {
			//fprintf(::stderr, "killing %i with signal %i\n",pid,sig);
			if(pid>0) ::kill(pid, sig);
		}
		void send(const char* data, int len) {
			write(stdin, data, len);
		}
		void send(const char* data) {
			send(data, strlen(data));
		}
	};
	map<int32_t, procInfo> procs;
	map<pid_t, int32_t> pidmap;
	pthread_mutex_t procs_lock;
	pthread_mutex_t write_lock;
	
	
	
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
	
	
	
	
	void unregisterProcess(pid_t pid) {
		scopeLock l(procs_lock);
		auto it=pidmap.find(pid);
		if(it==pidmap.end())return;
		int32_t tid=(*it).second;
		pidmap.erase(it);
		procs.erase(tid);
	}
	void childExited(pid_t pid, int32_t status) {
		int32_t tid;
		int32_t stdinfd;
		{
			scopeLock l(procs_lock);
			auto it=pidmap.find(pid);
			if(it==pidmap.end()) return;
			tid=(*it).second;
			stdinfd=procs[tid].stdin;
		}
		close(stdinfd);
		send({O((int8_t)EXECD::events::exit), O{tid, status}});
		
	}
	waiter w;
	

	/*struct procBuffer {
		int8_t stdout_buf[BUFSIZE];
		int8_t stdin_buf[BUFSIZE];
	};
	map<int32_t, procBuffer*> buffers;*/
	
	RGC::Ref<CP::Poll> p;
	
	//fd is not used
	void startRead(CP::File& f, int32_t tid, int32_t fd) {
		DEBUGPRINT("creating output fd %i (hostfd %i)\n", fd, f.handle);
		int8_t* buf=new int8_t[BUFSIZE];
		f.retain();
		f.repeatRead(buf, BUFSIZE, [this,buf,tid,fd,&f](int l) {
			if(l<=0) {
				DEBUGPRINT("deleting stdout/stderr buffer for fd %i (hostfd %i)\n", fd, f.handle);
				f.release();
				delete buf;
				//buf=NULL;
				return;
			}
			DEBUGPRINT("got %i bytes of data from process\n",l);
			A a((int8_t*)buf, l);
			send(A{O((int8_t)EXECD::events::output), O{O(tid),O(fd),O(a)}});
		});
	}
	//can only be called from eventloop
	void registerProcess(int32_t tid, procInfo& pinfo) {
		{
			scopeLock l(procs_lock);
			procs.insert({tid, pinfo});
			pidmap.insert({pinfo.pid, tid});
		}
		w.notifyFork();
		
		CP::File* f;
		
		f=new CP::File(pinfo.stdout, true);
		p->add(*f);
		startRead(*f,tid,1);
		f->release();
		
		
		f=new CP::File(pinfo.stderr, true);
		p->add(*f);
		startRead(*f,tid,2);
		f->release();
	}
	
	
	
	
	
	
	
	int execProcess(procInfo& pinfo, A param) {
		//cout << param << endl;
		A command1=param;//param[0].toArray();
		char* cmdline[command1.length()+1];
		for(int32_t i=0;i<command1.length();i++) {
			//printf("%i\n",command1[i].type);
			//printf("%p\n",command1[i].data.array);
			cmdline[i]=(char*)command1[i].toString();
		}
		cmdline[command1.length()]=NULL;
		int32_t I[2];
		int32_t O[2];
		int32_t E[2];
		checkError(pipe(I));
		checkError(pipe(O));
		checkError(pipe(E));
		
		pid_t tmp=fork();
		if(tmp>0) {
			close(I[0]);
			close(O[1]);
			close(E[1]);
			pinfo.pid=tmp;
			pinfo.stdin=I[1];
			pinfo.stdout=O[0];
			pinfo.stderr=E[0];
			return 0; //success
		} else if(tmp==0) {
			close(I[1]);
			close(O[0]);
			close(E[0]);
			dup2(I[0],0);
			dup2(O[1],1);
			dup2(E[1],2);
			close(I[0]);
			close(O[1]);
			close(E[1]);
			execvp(cmdline[0], cmdline+1);
			perror("execvp");
			_exit(1);
		} else {
			return errno;
		}
	}
	
	
	void processCommand(O o) {
		///{cmds command, int reqid, Object params}
		A a(o.toArray());
		
		A params=a[2].toArray();
		EXECD::cmds command=(EXECD::cmds)a[0].getInt8();
		int32_t reqid=a[1].getInt32();
		
		switch(command) {
			case EXECD::cmds::execute:
			{
				A command=params[0].toArray();
				int32_t taskid=params[1].toInt32();
				bool returnOutput=params[2].toInt8()!=0;
				auto it=procs.find(taskid);
				if(it!=procs.end()) {
					send({O((int8_t)EXECD::events::r_execute), O{reqid, EINVAL, 0}});
					break;
				}
				procInfo pinfo;
				pinfo.returnOutput=returnOutput;
				int32_t err=execProcess(pinfo, command);
				if(err==0)
					registerProcess(taskid, pinfo);
				send({O((int8_t)EXECD::events::r_execute), O{reqid, err, (int32_t)pinfo.pid}});
				break;
			}
			case EXECD::cmds::setReturnOutput:
			{
				int32_t taskid=params[0].toInt32();
				bool returnOutput=params[1].toInt8()!=0;
				auto it=procs.find(taskid);
				if(it==procs.end()) {
					send({O((int8_t)EXECD::events::r_setReturnOutput), O{reqid, ENOENT}});
					break;
				}
				(*it).second.returnOutput=returnOutput;
				send({O((int8_t)EXECD::events::r_setReturnOutput), O{reqid, 0}});
				break;
			}
			case EXECD::cmds::kill:
			{
				int32_t taskid=params[0].toInt32();
				int32_t sig=params[1].toInt32();
				auto it=procs.find(taskid);
				if(it==procs.end()) {
					send({O((int8_t)EXECD::events::r_kill), O{reqid, ENOENT}});
					break;
				}
				(*it).second.kill(sig);
				send({O((int8_t)EXECD::events::r_kill), O{reqid, 0}});
				break;
			}
			case EXECD::cmds::sendInput:
			{
				int32_t taskid=params[0].toInt32();
				A data=params[1].toArray();
				auto it=procs.find(taskid);
				if(it==procs.end()) {
					send({O((int8_t)EXECD::events::r_sendInput), O{reqid, ENOENT}});
					break;
				}
				(*it).second.send(data.getString(), data.length());
				send({O((int8_t)EXECD::events::r_sendInput), O{reqid, 0}});
				break;
			}
		}
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
		processCommand(o);
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
	void endPutData(int32_t len) {
		if(unlikely(len<=0)) return;
		DEBUGPRINT("got %i bytes of data...\n", len);
		readBuffer.endWrite(len);
		checkBuffer();
	}
	
	
	RGC::Ref<CP::File> inp;
	void beginReadStream() {
		DEBUGPRINT("beginReadStream()\n");
		int8_t* buf=(int8_t*)beginPutData(BUFSIZE);
		inp->read(buf, BUFSIZE, [this](int l) {
			DEBUGPRINT("read cb called... l=%i\n",l);
			if(l==0) {
				if(onEnd!=nullptr)onEnd();
				return;
			}
			endPutData(l);
			beginReadStream();
		});
	}
	void start(CP::Poll& p, CP::File& f) {
		this->p=&p;
		inp=&f;
		p.add(f);
		beginReadStream();
	}
	
	execd(): w(bind(&execd::childExited,this,_1,_2)) {
		initMutex(procs_lock);
		initMutex(write_lock);
	}
	~execd() {
		pthread_mutex_destroy(&procs_lock);
		pthread_mutex_destroy(&write_lock);
	}
};

int main(int argc, char** argv) {
	DEBUGPRINT("aaaaa\n");
	CP::File f(0,true); //stdin
	CP::File f1(1,true); //stdout
	execd ed;
	ed.onWrite=[&f1](int8_t* buf,int32_t len) {
		DEBUGPRINT("onWrite: len=%i\n",len);
		f1.write(buf, len);
	};
	CP::Poll p;
	ed.onEnd=[]() {
		//abort event loop
		throw CP::AbortException();
	};
	ed.start(p,f);
	p.loop();
	DEBUGPRINT("end\n");
}

