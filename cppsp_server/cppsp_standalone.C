#include <cpoll/statemachines.H>
#include <cpoll/cpoll.H>
#include <unistd.h>
#include <cpoll/sendfd.H>
#include <iostream>
#include <socketd.H>
#include <signal.h>
#include <cppsp/page.H>
#include <cppsp/cppsp_cpoll.H>
#include <cppsp/common.H>
#include <atomic_ops.h>
#include "server.C"


using namespace std;
using namespace socketd;
using namespace CP;
using namespace cppsp;
using namespace RGC;
string rootDir;

void parseArgs(int argc, char** argv, const function<void(char*, const function<char*()>&)>& cb);

struct workerThread
{
	cppspServer::Server srv;
	CP::Socket listenSock;
	pthread_t thread;
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
	void free(void* ptr) {
		sock.~Socket();
		if(allocator==NULL)free(this);
		else allocator->free(this);
	}
};
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
	
	MemoryPool handlerPool(sizeof(handler1));
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
	cout << "started child #" << getpid() << endl;
	{
		char cwd[255];
		if(getcwd(cwd,255)==NULL) throw runtime_error(strerror(errno));
		rootDir=cwd;
	}
	string listen="0.0.0.0:80";
	int threads=2;
	vector<string> cxxopts;
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
				}
			});
	
	auto i=listen.find(':');
	if(i==string::npos) throw runtime_error("expected \":\" in listen");
	listensock.bind(listen.substr(0,i).c_str(),
		listen.substr(i + 1, listen.length() - i - 1).c_str(), AF_UNSPEC, SOCK_STREAM);
	listensock.listen(512);
	//p.add(listensock);
	printf("starting %i threads\n",threads);
	workerThread* th=(workerThread*)new char[sizeof(workerThread)*threads];
	for(int i=0;i<threads;i++) {
		workerThread& tmp=*(new (th+i) workerThread(dup(listensock.handle),
			listensock.addressFamily, listensock.type, listensock.protocol));
		CXXOpts(tmp.srv.mgr)=cxxopts;
		tmp.threadid=i+1;
		if(threads==1) {
			thread1(&tmp);
			return 0;
		}
		if (pthread_create(&tmp.thread, NULL, thread1, &tmp) != 0) {
			throw runtime_error(strerror(errno));
		}
	}
	while(1)sleep(3600);
}
void parseArgs(int argc, char** argv, const function<void(char*, const function<char*()>&)>& cb) {
	int i = 1;
	function<char*()> func = [&]()->char*
	{
		if(i+1>=argc)return NULL;
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

