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
using namespace std;
using namespace CP;
using namespace cppsp;
using namespace RGC;
#define rmb() /**/
#define wmb() /**/

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
	class Server: public cppsp::DefaultServer {
	public:
		Poll* p;
		Server(Poll* p, string root):DefaultServer(root),p(p) {}
		AsyncValue<Handler> routeStaticRequestFromFile(String path) override;
		AsyncValue<Handler> routeDynamicRequestFromFile(String path) override;
	};
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
		Allocator* alloc;
		Server& thr;
		CP::Poll& p;
		Socket& s;
		StringPool sp;
		Request req;
		cppsp::Response resp;
		//Page* p;
		//MemoryStream ms;
		uint8_t* buf;
		iovec iov[2];
		bool keepAlive;
		handler(Server& thr,CP::Poll& poll,Socket& s):thr(thr),
			p(poll),s(s), req(this->s,&sp),resp(this->s,&sp) {
			//printf("handler()\n");
			req._handler=this;
			poll.add(this->s);
			s.retain();
			if(req.readRequest({&handler::readCB, this})) readCB(true);
		}
		void readCB(bool success) {
			if(!success) {
				destruct();
				return;
			}
			auto it=req.headers.find("connection");
			if(it!=req.headers.end() && (*it).value=="close")keepAlive=false;
			else keepAlive=true;
			resp.headers.insert({"Connection", keepAlive?"keep-alive":"close"});
			
			try {
				thr.handleRequest(req,resp,{&handler::finalize,this});
			} catch(exception& ex) {
				thr.handleError(req,resp,ex,{&handler::finalize,this});
			}
		}
		static inline int itoa(int i, char* b) {
			static char const digit[] = "0123456789";
			char* p = b;
			p += (i==0?0:int(log10f(i))) + 1;
			*p = '\0';
			int l = p - b;
			do { //Move back, inserting digits as u go
				*--p = digit[i % 10];
				i = i / 10;
			} while (i);
			return l;
		}
		void handleStatic(staticPage* Sp) {
			try {
				String data=Sp->data;
				int bufferL = resp.buffer.length();
				{
					char* tmps = sp.beginAdd(16);
					int l = itoa(data.length(), tmps);
					sp.endAdd(l);
					resp.headers.insert({"Content-Length", { tmps, l }});
					StreamWriter sw(resp.buffer);
					resp.serializeHeaders(sw);
				}
				iov[0]= {resp.buffer.data()+bufferL, (size_t)(resp.buffer.length()-bufferL)};
				iov[1]= {data.data(), (size_t)data.length()};
				resp.outputStream->writevAll(iov, 2, { &handler::writevCB, this });
			} catch(exception& ex) {
				thr.handleError(req,resp,ex,{&handler::finalize,this});
			}
		}
		void handleDynamic(loadedPage* lp) {
			Page* p=lp->doCreate(&sp);
			p->sp=&sp;
			p->request=&req;
			p->response=&resp;
			p->poll=&this->p;
			p->server=&thr;
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
			finalize();
		}
		void handleRequestCB() {
			//s->shutdown(SHUT_WR);
			//release();
			//s->repeatRead(buf,sizeof(buf),{&handler::sockReadCB,this});
			finalize();
		}
		void finalize() {
			if(resp.closed) {
				destruct(); return;
			}
			sp.clear();
			if(keepAlive) {
				req.reset();
				resp.reset();
				if(req.readRequest({&handler::readCB,this})) readCB(true);
			} else {
				s.shutdown(SHUT_WR);
				buf=(uint8_t*)malloc(4096);
				s.repeatRead(buf,4096,{&handler::sockReadCB,this});
			}
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
	
	AsyncValue<Handler> Server::routeStaticRequestFromFile(String path) {
		staticPage* sp=mgr->loadStaticPage(path);
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
	AsyncValue<Handler> Server::routeDynamicRequestFromFile(String path) {
		auto lp=mgr->loadPage(*p,root,path);
		if(lp) return Handler(&dynamicHandler,lp());
		requestRouterState* st=new requestRouterState();
		lp.wait(st);
		return Future<Handler>(&st->cb);
	}
}
