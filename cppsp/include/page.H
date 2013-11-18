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
/*
 * page.H
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#ifndef PAGE_H_
#define PAGE_H_
#include <rgc.H>
#include <map>
#include <set>
#include <cpoll/cpoll.H>
#include <string>
#include "stringutils.H"
#include "headercontainer.H"
#include "common.H"
#define CPPSP_VERSION 20130705
using namespace std;
using CP::String;
using CP::AsyncValue;
namespace cppsp
{
	class cppspManager;
	class loadedPage;
	class staticPage;
	class Host;
	class HTTPException: public std::exception
	{
	public:
		int code;
		char tmp[5];
		HTTPException();
		HTTPException(int32_t number);
		HTTPException(string message, int32_t number = 0);
		~HTTPException() throw ();
		const char* what() const throw ();
	};
	/**
	 Represents an HTTP request and contains data from the request such as HTTP headers, querystrings, and HTTP POST data.
	 */
	class Request: public RGC::Object
	{
	public:
		Request(CP::Stream& inp, CP::StringPool* sp);
		void init(CP::Stream& inp, CP::StringPool* sp);
		virtual ~Request() {
		}
		/**
		 internal field do not use.
		 */
		RGC::Ref<CP::Stream> inputStream;
		/**
		 The per-request StringPool instance. You can allocate memory from this if it only need to persist for the duration of this http request. The StringPool is cleared after the page has finished executing.
		 */
		CP::StringPool* sp; //may be used to store headers, querystrings, and POST data
		typedef map<String, String, less<String>, CP::PoolAllocator<std::pair<const String, String> > > StringMap;
		CP::PoolAllocator<std::pair<const String, String> > alloc;
		/**
		 HTTP request headers.
		 */
		headerContainer headers;
		/**
		 Query strings.
		 */
		StringMap queryString;
		/**
		 POST data.
		 */
		StringMap form;
		/**
		 can be used to pass custom parameters into a page (from a request router, etc)
		 */
		void* customParams;
		/**
		 HTTP method (GET, POST, etc).
		 */
		String method;
		/**
		 HTTP request path (directly from the HTTP header; not file path; does not include querystrings).
		 */
		String path;
		//string httpVersion;

		/**
		 You don't need to call this manually; POST data is automatically read for all HTTP POST requests.
		 */
		virtual void readPost(Delegate<void(Request&)> cb)=0;
		/**
		 internal method do not use.
		 */
		void parsePost(String buf);
		/**
		 internal method do not use.
		 See Response::reset().
		 */
		virtual void reset();
	};
	/**
	 Represents an HTTP response.
	 */
	class Response: public RGC::Object
	{
	public:
		typedef Delegate<void(Response&)> Callback;
		Response(CP::Stream& out, CP::StringPool* sp);
		Response(const Response& other) = delete;
		void init(CP::Stream& out, CP::StringPool* sp);
		/**
		 internal field do not use.
		 */
		RGC::Ref<CP::Stream> outputStream;
		/**
		 internal field do not use.
		 */
		CP::MemoryStream buffer;
		/**
		 You can write data to the HTTP response body by writing to "output". Content-Length will be automatically calculated.
		 */
		CP::StreamWriter output;
		/**
		 internal field do not use.
		 */
		Callback flushCB;
		/**
		 The per-request StringPool instance. You can allocate memory from this if it only need to persist for the duration of this http request. The StringPool is cleared after the page has finished executing.
		 */
		CP::StringPool* sp;
		//headerContainer2 headers;
		CP::PoolAllocator<std::pair<const String, String> > alloc;
		typedef map<String, String, less<String>, CP::PoolAllocator<std::pair<const String, String> > > StringMap;
		/**
		 HTTP response headers.
		 */
		StringMap headers;
		/**
		 HTTP response status (OK, Internal Server Error, etc).
		 */
		String statusName;
		/**
		 internal field do not use.
		 */
		iovec iov[3];
		/**
		 HTTP response status code (200, 500, etc).
		 */
		int statusCode;
		/**
		 internal field do not use.
		 */
		int _bufferPos;
		int _writeTo;

		/**
		 internal field do not use.
		 */
		bool headersWritten;
		/**
		 internal field do not use.
		 */
		bool closed;
		/**
		 TODO: chunked output not yet implemented.
		 */
		bool sendChunked;
		bool _writing;
		bool _doWrite;
		/*virtual void doWriteHeaders();
		 void writeHeaders() {
		 if (!headersWritten) {
		 doWriteHeaders();
		 headersWritten = true;
		 }
		 }*/
		/**
		 internal method do not use.
		 */
		virtual void flush(bool finalize = false);
		virtual void finalize(Callback cb);
		/**
		 clear buffered output
		 */
		virtual void clear();
		/**
		 internal method do not use.
		 */
		void addDefaultHeaders();
		/**
		 Serialize the HTTP response headers.
		 */
		void serializeHeaders(CP::StreamWriter& sw);
		/**
		 Write data to the HTTP response body.
		 */
		template<class ... T>
		void write(T&&... a) {
			output.write(std::forward<T>(a)...);
		}
		/**
		 internal method do not use.
		 */
		void _writeCB(int r);
		/**
		 internal method do not use.
		 Partially destructs the Response object. You can call init()
		 later on to reuse this Response object. This violates RAII
		 principles, but is necessary for good performance and low
		 memory usage.
		 */
		virtual void reset();
	};
	class Page;
	/**
	 Represents a request handler callback.
	 */
	typedef DelegateRef<void(Request&, Response&, Delegate<void()> cb)> Handler;
	typedef DelegateChain<void(Request&, Response&, Delegate<void()>)> HandleRequestChain;
	typedef DelegateChain<void(Request&, Response&, exception& ex, Delegate<void()>)> HandleErrorChain;
	struct ModuleInstance
	{
		loadedPage* origin;
		void* instance;
		bool operator<(const ModuleInstance& other) const {
			if (origin < other.origin) return true;
			if (origin > other.origin) return false;
			return instance < other.instance;
		}
		bool operator==(const ModuleInstance& other) const {
			return origin == other.origin && instance == other.instance;
		}
		ModuleInstance() {
		}
		ModuleInstance(loadedPage* origin, void* instance) :
				origin(origin), instance(instance) {
		}
		ModuleInstance(nullptr_t) :
				origin(nullptr), instance(nullptr) {
		}
	};
	class ModuleContainer
	{
	public:
		set<ModuleInstance> modules;
		struct moduleOrigin
		{
			loadedPage* origin;
			set<void*> modules;
		};
		map<string, set<ModuleInstance> > moduleMap_fileName;
		map<string, set<ModuleInstance> > moduleMap_path;
		map<string, set<ModuleInstance> > moduleMap_name;
		/**
		 Find all inserted modules with a file name of fn.
		 @param fn the file name (not path) e.g. module1.cppsm
		 */
		set<ModuleInstance>& findModulesByFileName(string fn);
		/**
		 Find all inserted modules with a path of p.
		 @param p the path.
		 */
		set<ModuleInstance>& findModulesByPath(string p);
		/**
		 Find all inserted modules with a module name (not filename) of n.
		 A module can declare its name by declaring a getModuleInfo(ModuleInfo&) function
		 and setting the "name" field in the ModuleInfo to the desired name in that function.
		 @param n the module name.
		 */
		set<ModuleInstance>& findModulesByName(string n);
		/**
		 see findModulesByFileName().
		 @returns the first item in the set. The ordering of the set is undefined, so only use this if you are sure the result set only contains one item.
		 */
		ModuleInstance findByFileName(string fn);
		/**
		 see findModulesByPath().
		 @returns the first item in the set. The ordering of the set is undefined, so only use this if you are sure the result set only contains one item.
		 */
		ModuleInstance findByPath(string p);
		/**
		 see findModulesByName().
		 @returns the first item in the set. The ordering of the set is undefined, so only use this if you are sure the result set only contains one item.
		 */
		ModuleInstance findByName(string n);
		/**
		 Same as findByName()
		 */
		ModuleInstance operator[](string name) {
			return findByName(name);
		}
		set<ModuleInstance>::iterator begin() {
			return modules.begin();
		}
		set<ModuleInstance>::iterator end() {
			return modules.end();
		}
		ModuleInstance insertModule(loadedPage* lp, ModuleParams& mp);
		void removeModule(ModuleInstance inst);
	};
	struct PerformanceCounters
	{ //to be incremented by the web server implementation
		int totalRequestsReceived;
		int totalRequestsFinished;
		PerformanceCounters() :
				totalRequestsReceived(0), totalRequestsFinished(0) {
		}
	};
	class Host: public RGC::Object
	{
	public:
		typedef DelegateChain<Server*(Request&)> PreRouteRequestChain;
		CP::Poll* poll;
		PerformanceCounters performanceCounters;
		set<Server*> servers;
		ModuleContainer modules;
		timespec curTime { 0, 0 }; //CLOCK_MONOTONIC
		timespec curClockTime { 0, 0 }; //CLOCK_REALTIME
		String curRFCTime;
		int threadID;
		//timer interval when the web server is active
		int timerShortInterval = 2;
		//timer interval when the web server is idle (no requests received since the last timer tick)
		int timerLongInterval = 120;
		Server* defaultServer;
		Host();
		~Host();
		void addServer(Server* srv);
		void removeServer(Server* srv);
		Server* defaultPreRouteRequest(Request& req);
		PreRouteRequestChain preRouteRequest;
		/**
		 should be called during every timer tick
		 @returns whether or not the timer still need to run
		 */
		virtual bool updateTime(bool noCleanCache = false);
		/**
		 Returns the cppspManager instance for this Host instance; may be NULL if the underlying server does not use cppspManager.
		 */
		virtual cppspManager* manager() {
			return NULL;
		}
		/**
		 Load static file from "path". path is absolute.
		 */
		virtual AsyncValue<Handler> routeStaticRequest(String path)=0;
		/**
		 Load dynamic page from "path". path is absolute.
		 */
		virtual AsyncValue<Handler> routeDynamicRequest(String path)=0;
		virtual AsyncValue<loadedPage*> loadPage(String path)=0;
		/**
		 @param fd whether to keep the file descriptor open
		 @param map whether to map the file into virtual memory
		 */
		virtual staticPage* loadStaticPage(String path, bool fd = false, bool map = true)=0;
		/**
		 Load cppsp module from "path". path is absolute.
		 */
		virtual AsyncValue<ModuleInstance> loadModule(String path);
		/**
		 Instantiate and insert a loaded module into this Host instance.
		 */
		virtual ModuleInstance insertModule(loadedPage* lp);
		/**
		 Remove an inserted module instance from this Host instance.
		 */
		virtual void removeModule(ModuleInstance inst);
	};
	class DefaultHost: public Host
	{
	public:
		cppspManager* mgr;
		string compilerWorkingDirectory;
		/**
		 Time (in seconds) before considering purging a cached cppspManager managed page.
		 For static pages, each cache entry contains an mmap()ed area backed by that file.
		 For dynamic pages, each cache entry contains an mmap()ed area backed by the page's string
		 table, and a dynamic linker handle from dlopen().
		 */
		int fileCacheCleanInterval = 600; //10 min.
		DefaultHost();
		~DefaultHost();
		bool updateTime(bool noCleanCache = false) override;
		bool cleanCache();
		cppspManager* manager() override;
		AsyncValue<loadedPage*> loadPage(String path) override;
		staticPage* loadStaticPage(String path, bool fd = false, bool map = true) override;
	};
	/**
	 Represents a cppsp web server instance.
	 */
	class Server: public RGC::Object
	{
	public:
		Host* host;
		string root;
		PerformanceCounters performanceCounters;
		ModuleContainer modules;

		CP::Poll* poll;
		int threadID;
		Server();
		~Server();
		inline timespec curTime() {
			return host->curTime;
		}
		inline timespec curClockTime() {
			return host->curClockTime;
		}
		inline String curRFCTime() {
			return host->curRFCTime;
		}

		/**
		 The handle request callback. It is called for every request right after reading in all the request data and instantiating the Request and Response objects.
		 You can override the behavior of the web server by attaching (from within a cppsp module). See the example .cppsm modules in /www (included with the cppsp source).
		 */
		HandleRequestChain handleRequest;
		/**
		 The error handler callback. Called when an exception occurs during the execution of a request handler, request router, or page.
		 */
		HandleErrorChain handleError;
		/**
		 Load static file from "path" and serve it to the specified response. path is relative to server root.
		 */
		void handleStaticRequest(String path, Request& req, Response& resp, Delegate<void()> cb) {
			return handleStaticRequestFromFile(mapPath(path, *req.sp), req, resp, cb);
		}
		/**
		 Load static file from "path" and serve it to the specified response. path is absolute.
		 */
		virtual void handleStaticRequestFromFile(String path, Request& req, Response& resp,
				Delegate<void()> cb);
		/**
		 Load dynamic page from "path" and serve it to the specified response. path is relative to server root.
		 */
		void handleDynamicRequest(String path, Request& req, Response& resp, Delegate<void()> cb) {
			return handleDynamicRequestFromFile(mapPath(path, *req.sp), req, resp, cb);
		}
		/**
		 Load dynamic page from "path" and serve it to the specified response. path is absolute.
		 */
		virtual void handleDynamicRequestFromFile(String path, Request& req, Response& resp,
				Delegate<void()> cb);
		/**
		 Calls routeRequest() to obtain the handler for the specific uri, and caches the
		 routing decision.
		 */
		void handleRoutedRequest(String path, Request& req, Response& resp, Delegate<void()> cb);
		/**
		 Default request handler. Calls handleRoutedRequest() with req.path as the path.
		 */
		void defaultHandleRequest(Request& req, Response& resp, Delegate<void()> cb);
		/**
		 Default error handler. Outputs the "Server Error" page similar to ASP.net
		 */
		void defaultHandleError(Request& req, Response& resp, exception& ex, Delegate<void()> cb);

		/*
		 Called by the default request handler to translate a request uri to a handler delegate.
		 The purpose of attaching a request router instead of a request handler directly to "handleRequest" is so that
		 the routing decision can be cached.
		 @returns a handler delegate that will be used to process requests destined for that uri. If the return value is undefined, it means the request is being completed asynchornously.
		 */
		DelegateChain<AsyncValue<Handler>(String path)> routeRequest;

		/**
		 Load static file from "path". path is relative to server root.
		 */
		AsyncValue<Handler> routeStaticRequest(String path) {
			return routeStaticRequestFromFile(mapPath(path.toSTDString()));
		}
		/**
		 Load static file from "path". path is absolute.
		 */
		AsyncValue<Handler> routeStaticRequestFromFile(String path) {
			return host->routeStaticRequest(path);
		}
		/**
		 Load dynamic page from "path". path is relative to server root.
		 */
		AsyncValue<Handler> routeDynamicRequest(String path) {
			return routeDynamicRequestFromFile(mapPath(path.toSTDString()));
		}
		/**
		 Load dynamic page from "path". path is absolute.
		 */
		AsyncValue<Handler> routeDynamicRequestFromFile(String path) {
			return host->routeDynamicRequest(path);
		}
		/**
		 Default request router. Calls either routeStaticRequest() or routeDynamicRequest() based on
		 the filename extension. Files ending with .cppsp are treated as dynamic pages, and everything else
		 are treated as static pages.
		 */
		AsyncValue<Handler> defaultRouteRequest(String path);
		struct RouteCacheEntry
		{
			Handler handler;
			string path;
			timespec lastUpdate; //CLOCK_MONOTONIC
		};
		/**
		 Internal field.
		 Holds route cache entries.
		 */
		map<String, RouteCacheEntry*> routeCache;
		/**
		 Time (in seconds) before considering a cached routing decision invalid and perform a re-route.
		 Increasing this may increase performance, but will increase the delay before a page is recompiled
		 after being edited.
		 */
		int routeCacheDuration = 2;
		/**
		 Time (in seconds) before considering purging a routing entry from the routeCache map.
		 Although routes expire after routeCacheDuration seconds, deleting and reinserting the same key
		 into a map is expensive, so we keep the entry itself around for longer by default.
		 Increasing this may increase performance, but may increase memory usage.
		 */
		int routeCacheCleanInterval = 120;

		/**
		 Returns absolute path of server root directory.
		 */
		String rootDir() {
			return root;
		}
		/**
		 Load and instantiate dynamic page from "path". path is relative to server root.
		 */
		virtual AsyncValue<Page*> loadPage(String path, RGC::Allocator& a);
		/**
		 Load and instantiate dynamic page from "path". path is absolute.
		 */
		virtual AsyncValue<Page*> loadPageFromFile(String path, RGC::Allocator& a);

		/**
		 Load cppsp module from "path". path is relative to server root.
		 */
		virtual AsyncValue<ModuleInstance> loadModule(String path);
		/**
		 Load cppsp module from "path". path is absolute.
		 */
		virtual AsyncValue<ModuleInstance> loadModuleFromFile(String path);
		virtual ModuleInstance insertModule(loadedPage* lp);
		virtual void removeModule(ModuleInstance inst);
		/**
		 Load static page from "path". path is relative to server root.
		 */
		virtual String loadStaticPage(String path);
		/**
		 Load static page from "path". path is absolute.
		 */
		virtual String loadStaticPageFromFile(String path);
		/**
		 Given a path relative to the server root, returns the absolute path.
		 */
		virtual string mapPath(string path);
		/**
		 Given a path relative to the server root, returns the absolute path. The returned string is allocated from "a".
		 */
		virtual String mapPath(String path, RGC::Allocator& a);
		/**
		 Returns the cppspManager instance for this Server instance; may be NULL if the underlying server does not use cppspManager.
		 */
		virtual cppspManager* manager();
		/**
		 should be called every time after you update Host::curTime or Host::curClockTime
		 @returns whether or not the timer still need to run
		 */
		virtual bool updateTime(bool noCleanCache = false);
		timespec _lastClean { 0, 0 };
		/**
		 Purges expired cache items.
		 @returns whether or not there are items left that could be purged in the future.
		 */
		virtual bool cleanCache(int minAge);
		virtual bool cleanCache() {
			return cleanCache(routeCacheCleanInterval);
		}
	};
	/**
	 Base class for all cppsp pages.
	 */
	class Page: public RGC::Object
	{
	public:
		typedef Delegate<void()> Callback;
		Page(Request& req, Response& resp, CP::StringPool* sp);
		/**
		 Object representing the HTTP request.
		 */
		RGC::Ref<Request> request;
		/**
		 Object representing the HTTP response.
		 */
		RGC::Ref<Response> response;
		RGC::Ref<cppsp::loadedPage> lp;
		/**
		 Poll instance for the current thread. Can be used for asynchronous I/O, timers, etc.
		 */
		CP::Poll* poll;
		/**
		 The current Server instance. Usually there is one Server instance for every thread.
		 */
		Server* server;
		/**
		 internal field do not use.
		 web server callback.
		 */
		Callback cb;
		/**
		 Absolute path of the current cppsp page.
		 */
		String filePath;
		/**
		 The per-request StringPool instance. You can allocate memory from this if it only need to persist for the duration of this http request. The StringPool is cleared after the page has finished executing.
		 */
		CP::StringPool* sp;
		bool doRender;
		bool doReadPost;
		Page() :
				doRender(true) {
		}
		inline void cancelRender() {
			doRender = false;
		}
		/**
		 internal field do not use. Pointer to mmap()ed address of the string table file.
		 */
		const uint8_t* __stringTable;
		/**
		 internal method do not use.
		 */
		void __writeStringTable(int i, int len);
		/**
		 Given a path relative to the current page, returns the absolute path. The returned string is allocated from the request-wide StringPool and is invalid after the page finishes executing.
		 */
		String mapPath(String path);
		/**
		 Given a path relative to the current page, returns the absolute path. The returned string is allocated from "a".
		 */
		String mapPath(String path, RGC::Allocator& a);
		/**
		 Given a path relative to the current page, returns the path relative to the server root. The returned string is allocated from the request-wide StringPool and is invalid after the page finishes executing.
		 */
		String mapRelativePath(String path);
		/**
		 Given a path relative to the current page, returns the path relative to the server root. The returned string is allocated from "a".
		 */
		String mapRelativePath(String path, RGC::Allocator& a);
		/**
		 Load and instantiate dynamic page from "path". path is relative to the current page.
		 */
		AsyncValue<Page*> loadNestedPage(String path);
		/**
		 Load and instantiate dynamic page from "path". path is relative to the current page. Returned Page object is allocated from "a".
		 */
		AsyncValue<Page*> loadNestedPage(String path, RGC::Allocator& a);
		/**
		 Load and instantiate dynamic page from "path". path is absolute.
		 */
		AsyncValue<Page*> loadNestedPageFromFile(String path);
		/**
		 Load and instantiate dynamic page from "path". path is absolute. Returned Page object is allocated from "a".
		 */
		AsyncValue<Page*> loadNestedPageFromFile(String path, RGC::Allocator& a);
		/**
		 Load static page from "path". path is relative to the current page.
		 */
		String loadNestedStaticPage(String path);
		/**
		 Load static page from "path". path is absolute.
		 */
		String loadNestedStaticPageFromFile(String path);
		//perform SSI from "path". path is relative to the current page.
		void staticInclude(String path);
		//perform SSI from "path". path is absolute.
		void staticIncludeFromFile(String path);
		/**
		 Called by the web server immediately after instantiating the current page. The page calls "cb" when it is done processing, to signal the web server that it can clean up the StringPool and all request-related data. The object pointed to by fields "request" and "response" may be recycled.
		 */
		virtual void handleRequest(Callback cb = nullptr);
		virtual void processRequest() {
			doInit();
		}
		/**
		 Each page goes through these stages (in order): init, [readPost], load, render, flush, finalize, [web server callback], ~Page

		 To be overridden by base class; called before load() is called, and must call initCB() or cancelLoad() after it's done
		 */
		virtual void init() {

		}
		virtual void doInit();
		virtual void initCB();
		virtual void cancelLoad(exception* ex = NULL);
		/**
		 To be overridden by derived class; called before headers are written. See init() for a description of the request life-cycle.
		 */
		virtual void load() {
		}
		virtual void render(CP::StreamWriter& out);
		/**
		 Called after rendering. See init() for a description of the request life-cycle.
		 */
		virtual void flush();
		/**
		 Called by flush() after it is done writing. See init() for a description of the request life-cycle.
		 */
		virtual void flushCB() {
			finalize();
		}
		/**
		 Called by flushCB. See init() for a description of the request life-cycle.
		 */
		virtual void finalize() {
			finalizeCB();
		}
		/**
		 Called by finalize() after it is done doing its work. This will call the web server's callback and cause it to clean-up and re-cycle the Request and Response objects, and destroy the Page object. See init() for a description of the request life-cycle.
		 */
		virtual void finalizeCB();
		virtual ~Page() {
		}

		void _readPOSTCB(Request& r);
		void _flushCB(Response& r);
	};
} /* namespace cppsp */
#endif /* PAGE_H_ */
