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
/*
 * page.C
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#include "include/page.H"
#include "include/common.H"
#include <stdexcept>
#include <stdlib.h>
#include <math.h>

using namespace CP;
using namespace std;
namespace cppsp
{
	static inline int itoa(int i, char* b) {
		static char const digit[] = "0123456789";
		char* p = b;
		//negative detection is not needed for this specific use-case
		//(writing the content-length header)
		/*if (i < 0) {
		 *p++ = '-';
		 i = -i;
		 }*/
		p += (i == 0 ? 0 : int(log10f(i))) + 1;
		*p = '\0';
		int l = p - b;
		do { //Move back, inserting digits as u go
			*--p = digit[i % 10];
			i = i / 10;
		} while (i);
		return l;
	}
	//inline-able memcpy() for copying SHORT STRINGS ONLY
	static inline void memcpy2_1(void* dst, const void* src, int len) {
		for (int i = 0; i < len; i++)
			((char*) dst)[i] = ((const char*) src)[i];
	}
	Page::Page(Request& req, Response& resp, CP::StringPool* sp) :
			request(&req), response(&resp), sp(sp), doRender(true) {
	}
	void Page::__writeStringTable(int i, int len) {
		response->output.write(__stringTable + i, len);
	}
	void Page::handleRequest(Callback cb) {
		this->cb = cb;
		try {
			processRequest();
		} catch (exception& ex) {
			server->handleError(*request, *response, ex, cb);
			destruct();
		}
	}
	void Page::render(CP::StreamWriter& out) {
		out.write("This is the default page of the cppsp C++ "
				"web application framework. If you see this, it means "
				"you haven't overridden the render() method derived from cppsp::Page.");
	}
	void Page::doInit() {
		doReadPost = false;
		init();
		if (doReadPost && request->method.compare("POST") == 0)
			request->readPost( { &Page::_readPOSTCB, this });
		else initCB();
	}
	void Page::initCB() {
		try {
			load();
			//response->writeHeaders();
			if (doRender) render(response->output);
			flush();
		} catch (exception& ex) {
			server->handleError(*request, *response, ex, cb);
			flush();
		}
	}
	void Page::cancelLoad(exception* ex) {
		if (ex == NULL) {
			runtime_error re("Web page execution cancelled");
			server->handleError(*request, *response, re, cb);
		} else server->handleError(*request, *response, *ex, cb);
		destruct();
	}
	void Page::_readPOSTCB(Request& r) {
		initCB();
	}
	void Page::flush() {
		if (response->closed)
			finalizeCB();
		else response->finalize( { &Page::_flushCB, this });
	}
	String Page::mapPath(String path) {
		return server->mapPath(mapRelativePath(path), *sp);
	}
	String Page::mapPath(String path, RGC::Allocator& a) {
		return server->mapPath(mapRelativePath(path, a), a);
	}
	String Page::mapRelativePath(String path) {
		return mapRelativePath(path, *sp);
	}
	String Page::mapRelativePath(String path, RGC::Allocator& a) {
		char *tmp = (char*) a.alloc(request->path.length() + path.length());
		int l = combinePath(request->path.data(), request->path.length(), path.data(), path.length(),
				tmp);
		return {tmp,l};
	}
	static void _setupPage(Page* t, Page* p) {
		p->request = t->request;
		p->response = t->response;
		p->poll = t->poll;
		p->server = t->server;
	}
	struct lpState
	{
		Page* t;
		Delegate<void(Page*, exception*)> cb;
		void operator()(Page* p, exception* ex) {
			if (p != NULL) _setupPage(t, p);
			delete this;
			cb(p, ex);
		}
	};
	AsyncValue<Page*> Page::loadNestedPage(String path) {
		return loadNestedPage(path, *sp);
	}
	AsyncValue<Page*> Page::loadNestedPage(String path, RGC::Allocator& a) {
		String tmp = mapRelativePath(path, a);
		auto tmpv = server->loadPage(*poll, tmp, a);
		if (tmpv) {
			_setupPage(this, tmpv());
			return tmpv();
		}
		auto* st = new lpState { this };
		tmpv.wait(st);
		return Future<Page*>(&st->cb);
	}
	AsyncValue<Page*> Page::loadNestedPageFromFile(String path) {
		return loadNestedPageFromFile(path, *sp);
	}
	AsyncValue<Page*> Page::loadNestedPageFromFile(String path, RGC::Allocator& a) {
		auto tmpv = server->loadPageFromFile(*poll, path, a);
		if (tmpv) {
			_setupPage(this, tmpv());
			return tmpv();
		}
		auto* st = new lpState { this };
		tmpv.wait(st);
		return Future<Page*>(&st->cb);
	}
	void Page::_flushCB(Response& r) {
		flushCB();
	}
	void Page::finalizeCB() {
		auto cb1 = this->cb;
		destruct();
		if (cb1 != nullptr) cb1();
	}
	Request::Request(CP::Stream& inp, CP::StringPool* sp) :
			inputStream(&inp), sp(sp), alloc(sp), headers(sp), queryString(less<String>(), alloc),
					form(less<String>(), alloc) {
	}
	void Request::init(CP::Stream& inp, CP::StringPool* sp) {
		new (&queryString) StringMap(less<String>(), alloc);
		new (&form) StringMap(less<String>(), alloc);
	}
	void Request::parsePost(String buf) {
		struct
		{
			Request* This;
			void operator()(const char* name, int nameLen, const char* value, int valueLen) {
				String n, v;
				n = cppsp::urlDecode(name, nameLen, *This->sp);
				if (value != NULL) {
					v = cppsp::urlDecode(value, valueLen, *This->sp);
				} else v= {(char*)nullptr,0};
				This->form[n] = v;
			}
		} cb { this };
		cppsp::parseQueryString(buf.data(), buf.length(), &cb, false);
	}

	Response::Response(CP::Stream& out, CP::StringPool* sp) :
			outputStream(&out), buffer(), output((CP::BufferedOutput&) buffer), sp(sp), alloc(sp),
					headers(less<String>(), alloc), _bufferPos(0), headersWritten(false), closed(false),
					sendChunked(false), _writing(false) {
		addDefaultHeaders();
	}
	void Response::init(CP::Stream& out, CP::StringPool* sp) {
		outputStream = &out;
		this->alloc.sp = sp;
		this->sp = sp;
		new (&headers) StringMap(less<String>(), alloc);
		addDefaultHeaders();
	}
	void Response::addDefaultHeaders() {
		statusCode = 200;
		statusName = "OK";
		headers.insert( { "Content-Type", "text/html; charset=UTF-8" });
	}
	void Response_doWriteHeaders(Response* This, CP::StreamWriter& sw) {
		//sw.writeF("HTTP/1.1 %i %s\r\n", This->statusCode, This->statusName);
		{
			char* s1 = sw.beginWrite(32);
			memcpy(s1, "HTTP/1.1 ", 9);
			int x = 9 + itoa(This->statusCode, s1 + 9);
			s1[x] = ' ';
			x++;
			memcpy(s1 + x, This->statusName.data(), This->statusName.length());
			x += This->statusName.length();
			s1[x] = '\r';
			s1[x + 1] = '\n';
			x += 2;
			sw.endWrite(x);
		}
		if (This->sendChunked) This->headers["Transfer-Encoding"] = "chunked";
		for (auto it = This->headers.begin(); it != This->headers.end(); it++) {
			int l1 = (*it).first.length();
			int l2 = (*it).second.length();
			char* tmp = sw.beginWrite(l1 + 4 + l2);
			memcpy(tmp, (*it).first.data(), l1);
			tmp[l1] = ':';
			tmp[l1 + 1] = ' ';
			memcpy(tmp + l1 + 2, (*it).second.data(), l2);
			tmp[l1 + 2 + l2] = '\r';
			tmp[l1 + 2 + l2 + 1] = '\n';
			sw.endWrite(l1 + 4 + l2);
			//sw.writeF("%s: %s\r\n", (*it).first.c_str(), (*it).second.c_str());
		}
		sw.write("\r\n", 2);
	}
	void Response::serializeHeaders(CP::StreamWriter& sw) {
		Response_doWriteHeaders(this, sw);
	}
	void Response::flush(bool finalize) {
		//printf("flush\n");
		if (closed) throw runtime_error("connection has already been closed by the client");
		if (_writing) {
			_doWrite = true;
			return;
		}
		_doWrite = false;
		_writing = true;
		if (finalize && sendChunked) output.write("\r\n0\r\n");
		output.flush();
		if (!headersWritten) {
			headersWritten = true;
			int bufferL = buffer.length();
			if (!sendChunked) {
				char* tmps = sp->beginAdd(16);
				int l = itoa(buffer.length(), tmps);
				sp->endAdd(l);
				this->headers.insert( { "Content-Length", { tmps, l } });
			} else {
				output.write("\r\n");
				output.flush();
			}
			{
				CP::StreamWriter sw(buffer);
				Response_doWriteHeaders(this, sw);
			}
			iov[0]= {buffer.data()+bufferL, (size_t)(buffer.length()-bufferL)};
			if (sendChunked) {
				int l2 = buffer.length();
				output.writeF("%x\r\n", finalize ? bufferL - 5 : bufferL);
				output.flush();
				iov[1]= {buffer.data()+l2,size_t(buffer.length()-l2)};
				iov[2]= {buffer.data(),(size_t)(bufferL+2)};
				outputStream->writevAll(iov, 3, { &Response::_writeCB, this });
			} else {
				iov[1]= {buffer.data(), (size_t)bufferL};
				outputStream->writevAll(iov, 2, {&Response::_writeCB, this});
			}
			_bufferPos = buffer.length();
			return;
		} else {
			if (_bufferPos >= buffer.length()) {
				_writing = false;
				if (flushCB) flushCB(*this);
				return;
			}
			if (sendChunked) {
				int _len = buffer.length();
				int l = _len - _bufferPos;
				output.writeF("\r\n%x\r\n", finalize ? l - 5 : l);
				output.flush();
				iov[0]= {buffer.data()+_len+2,size_t(buffer.length()-_len-2)};
				iov[1]= {buffer.data() + _bufferPos,(size_t)(l+2)};
				_bufferPos = buffer.length();
				outputStream->writevAll(iov, 2, { &Response::_writeCB, this });
			}
		}
	}
	void Response::finalize(Callback cb) {
		flushCB = cb;
		flush(true);
	}
	void Response::clear() {
		output.flush();
		buffer.clear();
		headersWritten = false;
	}
	void Response::_writeCB(int r) {
		if (r <= 0) closed = true;
		_writing = false;
		if (_doWrite)
			flush();
		else if (flushCB) flushCB(*this);
	}
	
	void Request::reset() {
		headers.clear();
		queryString.~map();
		form.~map();
		this->inputStream = nullptr;
	}
	void Response::reset() {
		headers.~map();
		output.flush();
		buffer.clear();
		headersWritten = false;
		closed = false;
		sendChunked = false;
		flushCB = nullptr;
		outputStream = nullptr;
	}

	Server::Server() :
			threadID(0) {
		handleRequest.attach( { &Server::defaultHandleRequest, this });
		handleError.attach( { &Server::defaultHandleError, this });
		routeRequest.attach( { &Server::defaultRouteRequest, this });
		curRFCTime.d = (char*) malloc(32);
		curRFCTime.len = 0;
	}
	Server::~Server() {
		free(curRFCTime.d);
	}
	struct requestHandlerState
	{
		Server* s;
		Request* req;
		Response* resp;
		Delegate<void()> cb;
		String path;
		void operator()(Handler h, exception* ex) {
			if (h != nullptr) {
				try {
					if (unlikely(s!=nullptr)) {
						auto it = s->routeCache.find(path);
						if (it == s->routeCache.end()) {
							Server::RouteCacheEntry* ce = new Server::RouteCacheEntry { h,
									path.toSTDString(), s->curTime };
							s->routeCache.insert( { ce->path, ce });
						} else {
							(*it).second->handler = h;
							(*it).second->lastUpdate = s->curTime;
						}
					}
					h(*req, *resp, cb);
				} catch (exception& ex1) {
					s->handleError(*req, *resp, ex1, cb);
				}
			} else {
				s->handleError(*req, *resp, *ex, cb);
			}
			resp->sp->del(this);
		}
		void flushCB(Response&) {
			cb();
			resp->sp->dealloc(this);
		}
	};
	void Server::handleStaticRequestFromFile(String path, Request& req, Response& resp,
			Delegate<void()> cb) {
		auto h = routeStaticRequestFromFile(path);
		if (h)
			h()(req, resp, cb);
		else {
			h.wait(
					resp.sp->New<requestHandlerState>(requestHandlerState { nullptr, &req, &resp, cb }));
		}
	}
	void Server::handleDynamicRequestFromFile(String path, Request& req, Response& resp,
			Delegate<void()> cb) {
		auto h = routeDynamicRequestFromFile(path);
		if (h)
			h()(req, resp, cb);
		else {
			h.wait(
					resp.sp->New<requestHandlerState>(requestHandlerState { nullptr, &req, &resp, cb }));
		}
	}
	void Server::handleRoutedRequest(String path, Request& req, Response& resp,
			Delegate<void()> cb) {
		auto it = routeCache.find(path);
		timespec tmp1 = curTime;
		tmp1.tv_sec -= routeCacheDuration;
		if (likely(it != routeCache.end() && tsCompare((*it).second->lastUpdate, tmp1) > 0)) {
			(*it).second->handler(req, resp, cb);
			return;
		}
		//printf("re-routing %s\n", req.path.toSTDString().c_str());
		auto* st = resp.sp->New<requestHandlerState>(
				requestHandlerState { this, &req, &resp, cb, path });
		auto h = routeRequest(path);
		if (h)
			(*st)(h(), nullptr);
		else h.wait(st);
	}
	void Server::defaultHandleRequest(Request& req, Response& resp, Delegate<void()> cb) {
		handleRoutedRequest(req.path, req, resp, cb);
	}
	struct flusher: public RGC::Object
	{
		Delegate<void()> cb;
		flusher(const Delegate<void()>& cb) :
				cb(cb) {
		}
		void operator()(int i) {
			cb();
			destruct();
		}
	};
	void Server::defaultHandleError(Request& req, Response& resp, exception& ex,
			Delegate<void()> cb) {
		cppsp::handleError(&ex, resp, req.path);
		resp.finalize(resp.sp->New<flusher>(cb));
	}
	AsyncValue<Handler> Server::defaultRouteRequest(String path) {
		if (path.length() > 6 && memcmp(path.data() + (path.length() - 6), ".cppsp", 6) == 0)
			return routeDynamicRequest(path);
		else return routeStaticRequest(path);
	}
	String Server::mapPath(String path, RGC::Allocator& a) {
		String r = rootDir();
		char* tmp = (char*) a.alloc(path.length() + r.length());
		int l = cppsp::combinePathChroot(r.data(), r.length(), path.data(), path.length(), tmp);
		return String(tmp, l);
	}
	
	void Server::updateTime(bool noCleanCache) {
		clock_gettime(CLOCK_MONOTONIC, &curTime);
		clock_gettime(CLOCK_REALTIME, &curClockTime);
		tm time;
		gmtime_r(&curClockTime.tv_sec, &time);
		curRFCTime.len = rfctime(time, curRFCTime.d);
		if (noCleanCache) return;
		timespec tmp1 = curTime;
		tmp1.tv_sec -= routeCacheCleanInterval;
		//printf("%i\n",tsCompare(_lastClean, tmp1));
		if (tsCompare(_lastClean, tmp1) < 0) {
			_lastClean = curTime;
			cleanCache();
		}
	}
	void Server::cleanCache() {
		timespec tmp1 = curTime;
		tmp1.tv_sec -= routeCacheCleanInterval;
		auto it = routeCache.begin();
		int del = 0;
		while (it != routeCache.end()) {
			if (tsCompare((*it).second->lastUpdate, tmp1) < 0) {
				delete (*it).second;
				auto tmp = it;
				it++;
				routeCache.erase(tmp);
				del++;
			} else it++;
		}
		if (del > 0) printf("%i route cache entries purged\n", del);
	}

	DefaultServer::DefaultServer(string root) :
			mgr(new cppspManager()), root(root) {
		mgr->srv = this;
	}
	DefaultServer::~DefaultServer() {
		delete mgr;
	}
	struct pageLoaderState
	{
		RGC::Allocator* a;
		Delegate<void(Page*, exception* ex)> cb;
		void operator()(loadedPage* lp, exception* ex) {
			if (lp == nullptr) {
				cb(nullptr, ex);
			} else {
				Page* p;
				try {
					p = lp->doCreate(a);
				} catch (exception& ex) {
					cb(nullptr, &ex);
					goto ret;
				}
				cb(p, nullptr);
			}
			ret: a->del(this);
		}
	};
	AsyncValue<Page*> DefaultServer::loadPage(CP::Poll& p, String path, RGC::Allocator& a) {
		auto tmp = mgr->loadPage(p, rootDir(), mapPath(path.toSTDString()));
		if (tmp) return tmp()->doCreate(&a);
		auto* st = a.New<pageLoaderState>(pageLoaderState { &a });
		tmp.wait(st);
		return Future<Page*>(&st->cb);
	}
	AsyncValue<Page*> DefaultServer::loadPageFromFile(CP::Poll& p, String path, RGC::Allocator& a) {
		auto tmp = mgr->loadPage(p, rootDir(), path);
		if (tmp) return tmp()->doCreate(&a);
		auto* st = a.New<pageLoaderState>(pageLoaderState { &a });
		tmp.wait(st);
		return Future<Page*>(&st->cb);
	}
	struct moduleLoaderState
	{
		Delegate<void(void*, exception* ex)> cb;
		void operator()(loadedPage* lp, exception* ex) {
			cb(lp == nullptr ? nullptr : lp->dlHandle, ex);
			delete this;
		}
	};
	AsyncValue<void*> DefaultServer::loadModule(CP::Poll& p, String path) {
		auto tmp = mgr->loadPage(p, rootDir(), mapPath(path.toSTDString()));
		if (tmp) return tmp()->dlHandle;
		auto* st = new moduleLoaderState();
		tmp.wait(st);
		return Future<void*>(&st->cb);
	}
	AsyncValue<void*> DefaultServer::loadModuleFromFile(CP::Poll& p, String path) {
		auto tmp = mgr->loadPage(p, rootDir(), path);
		if (tmp) return tmp()->dlHandle;
		auto* st = new moduleLoaderState();
		tmp.wait(st);
		return Future<void*>(&st->cb);
	}
	String DefaultServer::loadStaticPage(String path) {
		return mgr->loadStaticPage(mapPath(path.toSTDString()))->data;
	}
	String DefaultServer::loadStaticPageFromFile(String path) {
		return mgr->loadStaticPage(path)->data;
	}
	void DefaultServer::updateTime(bool noCleanCache) {
		Server::updateTime(noCleanCache);
		mgr->curTime = curTime;
	}
	void DefaultServer::cleanCache() {
		Server::cleanCache();
		mgr->cleanCache(this->fileCacheCleanInterval);
	}

} /* namespace cppsp */

string cppsp::Server::mapPath(string path) {
	String r = rootDir();
	char tmp[path.length() + r.length()];
	int l = cppsp::combinePathChroot(r.data(), r.length(), path.data(), path.length(), tmp);
	return string(tmp, l);
}
