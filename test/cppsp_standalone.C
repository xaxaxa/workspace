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

#define rmb() AO_nop_read()
#define wmb() AO_nop_write()
#define mb() AO_nop_full()

using namespace std;
using namespace socketd;
using namespace CP;
using namespace cppsp;
string rootDir;
class cppsp_Server: public cppsp::Server {
	virtual const char* rootDir() {
		return ::rootDir.c_str();
	}
};
cppsp_Server _server;
struct cppsp_request
{
	HANDLE fd;
};
struct serverThread;
void processRequest(serverThread& thr,Poll& p,Socket& s);
void parseArgs(int argc, char** argv, const function<void(char*, const function<char*()>&)>& cb);
#define CACHELINE_SIZE 64
template<class T> class RingBuffer
{
public:
	union {
		struct {
			T* items;
			int length;
		};
		char padding1[CACHELINE_SIZE];
	};
	union {
		int rpos;
		char padding2[CACHELINE_SIZE];
	};
	union {
		int wpos;
		char padding3[CACHELINE_SIZE];
	};
	
	RingBuffer(int length): length(length),rpos(0), wpos(0) {
		items=new T[length];
	}
	inline int __getlength(int i1, int i2, int wrap)
	{
		return (i2 < i1 ? i2 + wrap : i2) - i1;
	}
	inline bool canEnqueue()
	{
		return __getlength(rpos, wpos, (length*2)) < length;
	}
	inline bool canDequeue()
	{
		return __getlength(rpos, wpos, (length*2)) > 0;
	}
	T* beginEnqueue() {
		if(canEnqueue()) return items+(wpos%length);
		else return NULL;
	}
	void endEnqueue() {
		wmb();
		wpos=(wpos+1)%(length*2);
	}
	T* beginDequeue() {
		if(!canDequeue()) return NULL;
		rmb();
		return items+(rpos%length);
	}
	void endDequeue() {
		rpos=(rpos+1)%(length*2);
	}
};
struct serverThread
{
	cppspManager* mgr;
	//EventFD efd;
	pthread_t thr;
	Socket* listensock;
	//char padding[256-sizeof(cppspManager*)-sizeof(EventFD)-sizeof(pthread_t)];
	//RingBuffer<cppsp_request> req_queue;
	serverThread():mgr(cppspManager_new())
		/*,efd(0,EFD_SEMAPHORE),req_queue(32)*/{
		//printf("%i\n",sizeof(cppspManager*)+sizeof(EventFD)+sizeof(pthread_t));
	}
	~serverThread() {
		cppspManager_delete(mgr);
	}
} __attribute__((aligned(CACHELINE_SIZE)));

CP::Socket listensock;
void* thread1(void* v) {
	serverThread* thr=(serverThread*)v;
	Poll p(true);	//enable edge triggered mode
	
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
	
	struct {
		Poll& p;
		serverThread* thr;
		void operator()(Socket* sock) {
			cppsp_request* req;
			p.add(*sock);
			processRequest(*thr,p,*sock);
			sock->release();
		}
	} cb {p, thr};
	thr->listensock->repeatAccept(&cb);
	p.add(*thr->listensock);
	p.loop();
	
}
int main(int argc, char** argv) {
	cout << "started child #" << getpid() << endl;
	{
		char cwd[255];
		if(getcwd(cwd,255)==NULL) throw runtime_error(strerror(errno));
		rootDir=cwd;
	}
	string listen="0.0.0.0:80";
	int threads=2;
	parseArgs(argc, argv,
			[&](char* name, const std::function<char*()>& getvalue)
			{
				if(strcmp(name,"r")==0) {
					rootDir=getvalue();
				} else if(strcmp(name,"c")==0) {
					CXXOpts().push_back(getvalue());
				} else if(strcmp(name,"l")==0) {
					listen=getvalue();
				} else if(strcmp(name,"t")==0) {
					threads=atoi(getvalue());
				}
			});
	int reqs=0;
	//CP::Poll p;
	
	int i=(int)listen.find(':');
	if(i<0) throw runtime_error("expected \":\" in listen");;
	listensock.bind(listen.substr(0,i).c_str(),
		listen.substr(i + 1, listen.length() - i - 1).c_str(), AF_UNSPEC, SOCK_STREAM);
	listensock.listen(512);
	//p.add(listensock);
	printf("starting %i threads\n",threads);
	serverThread* th=new serverThread[threads];
	for(int i=0;i<threads;i++) {
		CXXOpts(th[i].mgr)=CXXOpts();
		Socket* tmps=new Socket(dup(listensock.handle), listensock.addressFamily,
			listensock.type, listensock.protocol);
		th[i].listensock=tmps;
		if (pthread_create(&th[i].thr, NULL, thread1, &th[i]) != 0) {
			throw runtime_error(strerror(errno));
		}
	}
	/*struct
	{
		serverThread* th;
		int threads;
		int curThread;
		void operator()(HANDLE h) {
			cppsp_request* req;
			//printf("th=%p threads=%i curThread=%i\n",th,threads,curThread);
			//printf("rpos=%i wpos=%i\n",th[curThread].req_queue.rpos,th[curThread].req_queue.wpos);
			//printf("items=%p length=%i\n",th[curThread].req_queue.items,th[curThread].req_queue.length);
			if((req=th[curThread].req_queue.beginEnqueue())==NULL)
				close(h);
			else {
				req->fd=h;
				th[curThread].req_queue.endEnqueue();
				th[curThread].efd.sendEvent();
			}
			curThread=(curThread+1)%threads;
		}
	} cb {th,threads,0};
	listensock.repeatAcceptHandle(&cb);
	p.loop();*/
	while(1)sleep(3600);
}
void processRequest(serverThread& thr,Poll& p,Socket& s) {
	struct handler:public RGC::Object {
		serverThread& thr;
		CP::Poll& p;
		RGC::Ref<Socket> s;
		cppsp::CPollRequest req;
		cppsp::Response resp;
		//Page* p;
		//MemoryStream ms;
		uint8_t buf[4096];
		string path;
		handler(serverThread& thr,CP::Poll& p,Socket* s):thr(thr),p(p),s(s),req(*s),resp(*s) {
			//printf("handler()\n");
			this->retain(2);
			req.readHeaders({&handler::readCB,this});
		}
		void readCB() {
			//printf("readCB()\n");
			char tmp[req.path.length()+rootDir.length()];
			int l=cppsp::combinePathChroot(rootDir.c_str(),req.path.c_str(),tmp);
			path=string(tmp,l);
			cppsp::loadPage(thr.mgr,p,rootDir,path,{&handler::loadCB,this});
		}
		
		void loadCB(Page* p, exception* ex) {
			//printf("loadCB()\n");
			if(ex!=NULL) {
				cppsp::handleError(ex,resp,path);
				resp.flush( { &handler::flushCB, this });
				goto doFinish;
			}
			{
				//this->p=p;
				//p->filePath=path;
				p->request=&req;
				p->response=&resp;
				p->poll=&this->p;
				p->server=&_server;
				p->handleRequest({&handler::handleRequestCB,this});
				return;
			}
		doFinish:
			//s->write(ms.data(),ms.length(),{&handler::writeCB,this});
			s->repeatRead(buf,sizeof(buf),{&handler::sockReadCB,this});
		}
		void sockReadCB(int r) {
			if(r<=0) release();
		}
		void flushCB(Response& resp) {
			//s->shutdown(SHUT_WR);
			release();
		}
		void handleRequestCB(Page& p) {
			p.release();
			s->shutdown(SHUT_WR);
			release();
			s->repeatRead(buf,sizeof(buf),{&handler::sockReadCB,this});
		}
		~handler() {
			//printf("~handler()\n");
		}
	}* hdlr=new handler(thr,p,&s);
	hdlr->release();
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

