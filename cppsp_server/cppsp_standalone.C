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
#include <cpoll/cpoll.H>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <cppsp/page.H>
#include <cppsp/cppsp_cpoll.H>
#include <cppsp/common.H>
#include "server.C"
#define PRINTSIZE(x) printf("sizeof("#x") = %i\n",sizeof(x))

using namespace std;
using namespace CP;
using namespace cppsp;
using namespace RGC;
string rootDir;

void parseArgs(int argc, char** argv, const function<void(char*, const function<char*()>&)>& cb);

struct workerThread
{
	cppspServer::Server srv;
	CP::Socket listenSock;
	union {
		pid_t pid;
		pthread_t thread;
	};
	int threadid;
	workerThread(int sock,int d,int t,int p): srv(rootDir.c_str()),
		listenSock(sock,d,t,p) {}
};
class handler1: public RGC::Allocator
{
public:
	Socket sock;
	cppspServer::handler h;
	handler1(cppspServer::Server& thr,CP::Poll& poll,HANDLE s,int d,int t,int p):
		sock(s,d,t,p),h(thr,poll,sock) {
			h.allocator=this;
	}
	void* alloc(int s) { return NULL; }
	void dealloc(void* ptr) {
		sock.~Socket();
		if(allocator==NULL)free(this);
		else allocator->dealloc(this);
	}
};
String globalHandler{(char*)nullptr,0};
void* thread1(void* v) {
	workerThread& thr=*(workerThread*)v;
	cppspServer::Server& srv=thr.srv;
	Poll p;
	
	/*
	p.add(thr->efd);
	struct {
		Poll& p;
		serverThread* thr;
		void operator()(eventfd_t eventcount) {
			cppsp_request* req;
			while((req=thr->req_queue.beginDequeue())!=NULL) {
				Socket* sock=new Socket(req->fd,listensock.addressFamily,
					listensock.type,listensock.protocol);
				//printf("new socket: %p\n",sock);
				p.add(*sock);
				processRequest(*thr,p,*sock);
				sock->release();
				thr->req_queue.endDequeue();
			}
		}
	} cb {p, thr};
	thr->efd.repeatGetEvent(&cb);*/
	MemoryPool handlerPool(sizeof(handler1),256);
	struct {
		Poll& p;
		workerThread& thr;
		MemoryPool& handlerPool;
		void operator()(HANDLE sock) {
			//printf("thread %i: accepted socket: %p (%i)\n",thr->threadid,sock,sock->handle);
			handler1* hdlr=new (handlerPool.alloc())
				handler1(thr.srv,p,sock,thr.listenSock.addressFamily,
					thr.listenSock.type,thr.listenSock.protocol);
			hdlr->allocator=&handlerPool;
		}
	} cb {p, thr, handlerPool};
	thr.listenSock.repeatAcceptHandle(&cb);
	p.add(thr.listenSock);
	Timer t((uint64_t)2000);
	struct {
		cppspServer::Server& srv;
		void operator()(int count) {
			srv.updateTime();
		}
	} cb1 {srv};
	t.setCallback(&cb1);
	p.add(t);
	p.loop();
	return NULL;
}
CP::Socket listensock;
int main(int argc, char** argv) {
	{
		char cwd[255];
		if(getcwd(cwd,255)==NULL) throw runtime_error(strerror(errno));
		rootDir=cwd;
	}
	string listen="0.0.0.0:80";
	int threads=2;
	bool f0rk=false;
	vector<string> cxxopts;
	try {
		parseArgs(argc, argv,
				[&](char* name, const std::function<char*()>& getvalue)
				{
					if(strcmp(name,"r")==0) {
						rootDir=getvalue();
					} else if(strcmp(name,"c")==0) {
						cxxopts.push_back(getvalue());
					} else if(strcmp(name,"l")==0) {
						listen=getvalue();
					} else if(strcmp(name,"t")==0) {
						threads=atoi(getvalue());
					} else if(strcmp(name,"h")==0) {
						globalHandler=getvalue();
					} else if(strcmp(name,"f")==0) {
						f0rk=true;
					} else {
						printf("usage: %s [options]...\noptions:\n"
						"\t-l <host:port>: listen on specified host:port\n"
						"\t-c <option>: specify a compiler option to be passed to g++\n"
						"\t-r <root>: set root directory\n"
						"\t-t <threads>: # of worker processes/threads to start up\n"
						"\t-h </relative/path/to/handler.cppsp>: specify the handler to direct all requests to\n"
						"\t-f: use multi-processing (forking) instead of multi-threading (pthreads)\n",argv[0]);
						exit(1);
					}
				});
	} catch(exception& ex) {
		printf("error: %s\nspecify -? for help\n",ex.what());
		return 1;
	}
	printf("specify -? for help\n");
	auto i=listen.find(':');
	if(i==string::npos) throw runtime_error("expected \":\" in listen");
	listensock.bind(listen.substr(0,i).c_str(),
		listen.substr(i + 1, listen.length() - i - 1).c_str(), AF_UNSPEC, SOCK_STREAM);
	listensock.listen(512);
	//p.add(listensock);
	PRINTSIZE(CP::Socket);
	PRINTSIZE(cppspServer::handler);
	PRINTSIZE(handler1);
	printf("starting %i threads\n",threads);
	workerThread* th=(workerThread*)new char[sizeof(workerThread)*threads];
	for(int i=0;i<threads;i++) {
		workerThread& tmp=*(new (th+i) workerThread(f0rk ? 
			listensock.handle : dup(listensock.handle),
			listensock.addressFamily, listensock.type, listensock.protocol));
		tmp.srv.globalHandler=globalHandler;
		CXXOpts(tmp.srv.mgr)=cxxopts;
		tmp.threadid=i+1;
		if(threads==1) {
			thread1(&tmp);
			return 0;
		}
		if(f0rk) {
			pid_t pid=fork();
			if(pid==0) {
				tmp.pid=getpid();
				thread1(&tmp);
				return 0;
			} else if(pid>0) {
				tmp.pid=pid;
			} else {
				perror("fork");
				return 1;
			}
		} else {
			if (pthread_create(&tmp.thread, NULL, thread1, &tmp) != 0) {
				throw runtime_error(strerror(errno));
			}
		}
	}
	if(f0rk) {
		static workerThread* _threads;
		static int _threadcount;
		struct sig_handler
		{
			static void a(int sig) {
				for(int i=0;i<_threadcount;i++) {
					kill(_threads[i].pid, 9);
				}
				exit(0);
			}
		};
		_threads=th;
		_threadcount=threads;
		struct sigaction sa;
		sa.sa_handler = &sig_handler::a;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGTERM, &sa, NULL);
		sigaction(SIGSEGV, &sa, NULL);
	}
	while(1)sleep(3600);
}
void parseArgs(int argc, char** argv, const function<void(char*, const function<char*()>&)>& cb) {
	int i = 1;
	function<char*()> func = [&]()->char*
	{
		if(i+1>=argc)throw logic_error(string(argv[i])+" requires an argument");
		return argv[(++i)];
	};
	for (; i < argc; i++) {
		if (argv[i][0] == '\x00') continue;
		if (argv[i][0] == '-') {
			cb(argv[i] + 1, func);
		} else {
			cb(NULL, [argv,i]()
			{	return argv[i];});
		}
	}
}

