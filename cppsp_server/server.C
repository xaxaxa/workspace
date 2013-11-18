/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
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
#include <sys/stat.h>

using namespace std;
using namespace CP;
using namespace cppsp;
using namespace RGC;
#define rmb() /**/
#define wmb() /**/
#define CPPSP_SENDFILE_MIN_SIZE (1024*1024)
#define CPPSP_SENDFILE_BUFSIZE (1024*16)

namespace cppspServer
{
	#define CACHELINE_SIZE 64
	//currently not used
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
	
	template<class T>
	class ObjectPool: public RGC::Object
	{
	public:
		T** items;
		int size;
		int length;
		ObjectPool(int size):size(size),length(0) {
			items=new T*[size];
		}
		~ObjectPool() {
			for(int i=0;i<length;i++) delete items[i];
			delete[] items;
		}
		T* tryGet() {
			if(length<=0) return NULL;
			return items[--length];
		}
		void put(T* obj) {
			if(length>=size) delete obj;
			else {
				items[length++]=obj;
			}
		}
	};
	class Host: public cppsp::DefaultHost {
	public:
		cppsp::Server server;
		Timer t;
		ObjectPool<Response> _responsePool;
		int _lastRequests=0;
		int timerState=0;
		void timerCB(int i) {
			if(!updateTime() && timerState==1) {
				disableTimer();
				return;
			}
			if(timerState==2 && performanceCounters.totalRequestsReceived<=_lastRequests) {
				slowTimer();
			}
			_lastRequests=performanceCounters.totalRequestsReceived;
		}
		Host(Poll* p, string root): _responsePool(128) {
			this->poll=p;
			defaultServer=&server;
			addServer(&server);
			server.root=root;
			updateTime();
			t.setCallback({&Host::timerCB,this});
			p->add(t);
		}
		void loadDefaultMimeDB() {
			File f("/usr/share/mime/globs",O_RDONLY);
			f.setBlocking(true);
			StreamReader sr(f);
			mgr->loadMimeDB(sr);
		}
		void enableTimer() {
			if(timerState==0) printf("enabling timer\n");
			t.setInterval(timerShortInterval*1000);
			timerState=2;
			updateTime(true); //true indicates to inhibit cache cleaning
		}
		void slowTimer() {
			t.setInterval(timerLongInterval*1000);
			timerState=1;
		}
		void disableTimer() {
			printf("disabling timer\n");
			t.setInterval(0);
			timerState=0;
		}
		inline void _requestReceived() {
			if(unlikely(timerState<2)) enableTimer();
			performanceCounters.totalRequestsReceived++;
		}
		AsyncValue<Handler> routeStaticRequest(String path) override;
		AsyncValue<Handler> routeDynamicRequest(String path) override;
	};
	typedef Host Server;
	class Request:public cppsp::CPollRequest
	{
	public:
		Request(CP::Socket& s, CP::StringPool* sp) :
			CPollRequest(s, sp) {
		}
		void* _handler;
	};
	//handles a single connection
	//just instantiate-and-forget; it will self-destruct when connection is closed
	struct handler:public RGC::Object {
		Host& thr;
		CP::Poll& p;
		Socket& s;
		StringPool sp;
		Request req;
		cppsp::Server* server;
		cppsp::Response* resp;
		//Page* p;
		//MemoryStream ms;
		uint8_t* buf;
		union {
			iovec iov[2];
			int64_t _sendFileOffset;
		};
		staticPage* _staticPage;
		bool readLoopRunning;
		bool shouldContinueReading;
		bool keepAlive;
		handler(Host& thr,CP::Poll& poll,Socket& s):thr(thr),
			p(poll),s(s),sp(2048),req(this->s,&sp) {
			//printf("handler()\n");
			req._handler=this;
			poll.add(this->s);
			s.retain();
			readLoop();
		}
		void readLoop() {
			readLoopRunning=true;
			shouldContinueReading=true;
			while(shouldContinueReading && req.readRequest({&handler::readCB, this})) readCB(true);
			readLoopRunning=false;
		}
		void readCB(bool success) {
			shouldContinueReading=false;
			if(unlikely(!success)) {
				destruct();
				return;
			}
			//if((sp=thr._stringPoolPool.tryGet())==nullptr) sp=new StringPool();
			if((resp=thr._responsePool.tryGet())) resp->init(this->s,&sp);
			else resp=new Response(this->s,&sp);
			
			thr._requestReceived();
			
			//keepAlive=true;
			auto it=req.headers.find("connection");
			if(it!=req.headers.end() && (*it).value=="close")keepAlive=false;
			else keepAlive=true;
			resp->headers.insert({"Connection", keepAlive?"keep-alive":"close"});
			
			/*char* date=sp.beginAdd(32);
			tm time;
			gmtime_r(&thr.curClockTime.tv_sec,&time);
			int l=rfctime(time,date);
			if(l>32)l=32;
			sp.endAdd(l);
			*/
			resp->headers.insert({"Date", sp.addString(thr.curRFCTime)});
			
			//perform vhost routing
			server=thr.preRouteRequest(req);
			server->performanceCounters.totalRequestsReceived++;
			try {
				server->handleRequest(req,*resp,{&handler::finalize,this});
			} catch(exception& ex) {
				server->handleError(req,*resp,ex,{&handler::finalize,this});
			}
		}
		static inline int itoa64(int64_t i, char* b) {
			static char const digit[] = "0123456789";
			char* p = b;
			int l;
			p += (l=((i==0?0:int(log10(i))) + 1));
			*p = '\0';
			do { //Move back, inserting digits as u go
				*--p = digit[i % 10];
				i = i / 10;
			} while (i);
			return l;
		}
		void handleStatic(staticPage* Sp) {
			Response& resp(*this->resp);
			(_staticPage=Sp)->retain();
			try {
				int bufferL = resp.buffer.length();
				if(Sp->mime.length()>0)resp.headers["Content-Type"]=Sp->mime;
				{
					char* tmps = sp.beginAdd(22);
					int l = itoa64(Sp->fileLen, tmps);
					sp.endAdd(l);
					resp.headers.insert({"Content-Length", { tmps, l }});
					StreamWriter sw(resp.buffer);
					resp.serializeHeaders(sw);
				}
				if(Sp->fileLen>=CPPSP_SENDFILE_MIN_SIZE) {
					_sendFileOffset=0;
					s.sendAll(resp.buffer.data()+bufferL,resp.buffer.length()-bufferL,
						MSG_MORE, { &handler::sendHeadersCB, this });
				} else {
					String data=Sp->data;
					iov[0]= {resp.buffer.data()+bufferL, (size_t)(resp.buffer.length()-bufferL)};
					iov[1]= {data.data(), (size_t)data.length()};
					resp.outputStream->writevAll(iov, data.length()<=0?1:2, { &handler::writevCB, this });
				}
			} catch(exception& ex) {
				Sp->release();
				server->handleError(req,resp,ex,{&handler::finalize,this});
			}
		}
		void sendHeadersCB(int r) {
			if(r<0) {
				_staticPage->release();
				end();
				return;
			}
			_beginSendFile();
		}
		void _beginSendFile() {
			s.sendFileFrom(_staticPage->fd,_sendFileOffset,CPPSP_SENDFILE_BUFSIZE,{&handler::sendFileCB,this});
		}
		void sendFileCB(int r) {
			if(r<0) {
				_staticPage->release();
				end();
			} else if(r==0) {
				_staticPage->release();
				finalize();
			} else {
				_sendFileOffset+=(int64_t)r;
				_beginSendFile();
			}
		}
		void handleDynamic(loadedPage* lp) {
			Response& resp(*this->resp);
			Page* p=lp->doCreate(&sp);
			//hold a strong reference to lp so that if cleanCache() etc is called by application code,
			//the application does not unload itself, causing a segfault
			p->lp=lp;
			p->sp=&sp;
			p->request=&req;
			p->response=&resp;
			p->poll=&this->p;
			p->server=server;
			p->handleRequest({&handler::handleRequestCB,this});
		}
		void sockReadCB(int r) {
			if(r<=0) {
				free(buf);
				destruct();
			}
		}
		void flushCB(Response& resp) {
			//s->shutdown(SHUT_WR);
			//release();
			finalize();
		}
		void writevCB(int i) {
			_staticPage->release();
			if(likely(i>=0)) finalize();
			else end();
		}
		void handleRequestCB() {
			//s->shutdown(SHUT_WR);
			//release();
			//s->repeatRead(buf,sizeof(buf),{&handler::sockReadCB,this});
			finalize();
		}
		void finalize() {
			if(resp->closed) {
				end(); return;
			}
			cleanup();
			if(keepAlive) {
				req.init(s,&sp);
				if(readLoopRunning) shouldContinueReading=true;
				else readLoop();
			} else {
				s.shutdown(SHUT_WR);
				buf=(uint8_t*)malloc(4096);
				s.repeatRead(buf,4096,{&handler::sockReadCB,this});
			}
		}
		//cleanup and terminate the connection
		void end() {
			cleanup();
			destruct();
		}
		//deallocate resources after a request has been completed
		void cleanup() {
			server->performanceCounters.totalRequestsFinished++;
			thr.performanceCounters.totalRequestsFinished++;
			req.reset();
			resp->reset();
			thr._responsePool.put(resp);
			resp=nullptr;
			sp.clear();
		}
		~handler() {
			//printf("~handler()\n");
			s.release();
		}
	};
	void staticHandler(staticPage* v,cppsp::Request& req, Response& resp, Delegate<void()> cb) {
		cppspServer::Request& r=static_cast<cppspServer::Request&>(req);
		(*(handler*)r._handler).handleStatic(v);
	}
	void dynamicHandler(loadedPage* v,cppsp::Request& req, Response& resp, Delegate<void()> cb) {
		cppspServer::Request& r=static_cast<cppspServer::Request&>(req);
		(*(handler*)r._handler).handleDynamic(v);
	}
	
	AsyncValue<Handler> Host::routeStaticRequest(String path) {
		struct stat st;
		string tmps=path.toSTDString();
		if(::stat(tmps.c_str(), &st)!=0) throwUNIXException(tmps);
		staticPage* sp;
		if(st.st_size>=CPPSP_SENDFILE_MIN_SIZE)
			sp=loadStaticPage(path,true,false);
		else
			sp=loadStaticPage(path);
		return Handler(&staticHandler,sp);
	}
	struct requestRouterState
	{
		Delegate<void(Handler,exception*)> cb;
		void operator()(loadedPage* lp, exception* ex) {
			if(lp==NULL)cb(nullptr,ex);
			else cb(Handler(&dynamicHandler,lp),nullptr);
			delete this;
		}
	};
	AsyncValue<Handler> Host::routeDynamicRequest(String path) {
		auto lp=loadPage(path);
		if(lp) {
			return Handler(&dynamicHandler,lp());
		}
		requestRouterState* st=new requestRouterState();
		lp.wait(st);
		return Future<Handler>(&st->cb);
	}
}
namespace cppsp {
	struct HandlerBase
	{
		Request* request; Response* response; Delegate<void()> cb;
	};
	template<class T>
	Handler makeHandler() {
		struct H: public RGC::Object
		{
			T* construct(RGC::Allocator& alloc) {
				return alloc.New<T>();
			}
			void operator()(Request& req, Response& resp, Delegate<void()> cb) {
				T* tmp=construct(*req.sp);
				tmp->request=&req;
				tmp->response=&resp;
				tmp->cb=cb;
				tmp->process();
			}
		};
		return newObj<H>();
	}
};
namespace cppspEmbedded
{
	class Server
	{
	public:
		cppspServer::Host host;
		Server(Poll* p, string root):host(p,root) {
		}
		void listen(Socket& s) {
			struct CB: public RGC::Object {
				Server* s;
				CB(Server* s):s(s){}
				void operator()(Socket* clientSock) {
					new cppspServer::handler(s->host,*s->host.poll,*clientSock);
					clientSock->release();
				}
			};
			s.repeatAccept(newObj<CB>(this));
		}
		HandleRequestChain::item* attachHandler(const Handler& h) {
			return host.defaultServer->handleRequest.attach(h);
		}
		void detachHandler(HandleRequestChain::item* it) {
			return host.defaultServer->handleRequest.detach(it);
		}
	};
};
