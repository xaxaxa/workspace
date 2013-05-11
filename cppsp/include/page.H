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
 * page.H
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#ifndef PAGE_H_
#define PAGE_H_
#include <rgc.H>
#include <map>
#include <cpoll/cpoll.H>
#include "stringutils.H"
#include "headercontainer.H"
using namespace std;
namespace cppsp
{
	typedef CP::String String;
	class Request: public RGC::Object
	{
	public:
		Request(CP::Stream& inp, CP::StringPool* sp);
		virtual ~Request() {
		}
		RGC::Ref<CP::Stream> inputStream;
		CP::StringPool* sp; //may be used to store headers, querystrings, and POST data
		typedef map<String, String, less<String>, CP::PoolAllocator<std::pair<const String, String> > > StringMap;
		CP::PoolAllocator<std::pair<const String, String> > alloc;
		headerContainer headers;
		StringMap queryString;
		StringMap form;
		String method;
		String path;
		//string httpVersion;

		//read POST data; headers are always read before creating the Request object
		virtual void readPost(Delegate<void(Request&)> cb)=0;
		void parsePost(String buf);
		virtual String getBufferedData() {
			return {(char*)nullptr,0};
		}

		virtual void reset();
	};
	class Response: public RGC::Object
	{
	public:
		typedef Delegate<void(Response&)> Callback;
		Response(CP::Stream& out, CP::StringPool* sp);
		RGC::Ref<CP::Stream> outputStream;
		CP::MemoryStream buffer;
		CP::StreamWriter output;
		Callback _cb;
		CP::StringPool* sp;
		headerContainer2 headers;
		String statusName;
		iovec iov[2];
		int statusCode;

		bool headersWritten;
		bool closed;
		bool sendChunked;
		/*virtual void doWriteHeaders();
		 void writeHeaders() {
		 if (!headersWritten) {
		 doWriteHeaders();
		 headersWritten = true;
		 }
		 }*/
		virtual void flush(Callback cb);
		virtual void clear();
		void addDefaultHeaders();
		void serializeHeaders(CP::StreamWriter& sw);
		template<class ... T>
		void write(T&&... a) {
			output.write(std::forward<T>(a)...);
		}
		void _writeCB(int r);
		virtual void reset();
	};
	class Page;
	class Server: public RGC::Object
	{
	public:
		Server();
		DelegateChain<void(Request&, Response&, Delegate<void()>)> handleRequest;
		virtual void handleStaticRequest(String path, Request& req, Response& resp,
				Delegate<void()> cb)=0;
		virtual void handleDynamicRequest(String path, Request& req, Response& resp,
				Delegate<void()> cb)=0;
		void defaultHandleRequest(Request& req, Response& resp, Delegate<void()> cb);
		virtual String rootDir()=0;
		//path is relative to rootDir
		virtual void loadPage(CP::Poll& p, String path, RGC::Allocator& a,
				Delegate<void(Page*, exception* ex)> cb)=0;
		//path is absolute
		virtual void loadPageFromFile(CP::Poll& p, String path, RGC::Allocator& a,
				Delegate<void(Page*, exception* ex)> cb)=0;

		virtual void loadModule(CP::Poll& p, String path, Delegate<void(void*, exception* ex)> cb)=0;
		virtual void loadModuleFromFile(CP::Poll& p, String path,
				Delegate<void(void*, exception* ex)> cb)=0;
		virtual String loadStaticPage(String path)=0;
		virtual String loadStaticPageFromFile(String path)=0;
		//path is relative to rootDir
		virtual string mapPath(string path);
		virtual String mapPath(String path, RGC::Allocator& a);
	};
	class Page: public RGC::Object
	{
	public:
		typedef Delegate<void()> Callback;
		Page(Request& req, Response& resp, CP::StringPool* sp);
		RGC::Ref<Request> request;
		RGC::Ref<Response> response;
		Delegate<void(Page*, exception* ex)> pageCB;
		CP::Poll* poll;
		Server* server;
		Callback cb;
		String filePath;
		CP::StringPool* sp;
		bool doRender;
		bool doReadPost;
		Page() :
				doRender(true) {
		}
		inline void cancelRender() {
			doRender = false;
		}
		//pointer to mmap()ed address of original cppsp file
		const uint8_t* __stringTable;
		void __writeStringTable(int i, int len);
		String mapPath(String path);
		String mapPath(String path, RGC::Allocator& a);
		String mapRelativePath(String path);
		String mapRelativePath(String path, RGC::Allocator& a);
		//path is relative to current page
		void loadNestedPage(String path, Delegate<void(Page*, exception* ex)> cb);
		void loadNestedPage(String path, Delegate<void(Page*, exception* ex)> cb, RGC::Allocator& a);
		//path is absolute
		void loadNestedPageFromFile(String path, Delegate<void(Page*, exception* ex)> cb);
		void loadNestedPageFromFile(String path, Delegate<void(Page*, exception* ex)> cb,
				RGC::Allocator& a);
		virtual void handleRequest(Callback cb = nullptr);
		virtual void processRequest() {
			doInit();
		}
		//each page goes through these operations (in order): init, [readPost], load, render, flush, finalize

		virtual void init() { //to be overridden by base class; called before load() is called, and
									 //must call initCB() or cancelLoad() after it's done
		}
		virtual void doInit();
		virtual void initCB();
		virtual void cancelLoad(exception* ex = NULL);
		virtual void load() { //to be overridden by derived class; called before headers are written
		}
		virtual void render(CP::StreamWriter& out);
		virtual void flush();
		virtual void flushCB() {
			finalize();
		}
		virtual void finalize() {
			finalizeCB();
		}
		virtual void finalizeCB();
		virtual ~Page() {
		}

		void _readPOSTCB(Request& r);
		void _flushCB(Response& r);
		void _pageCB(Page* p, exception* ex);
	};

} /* namespace cppsp */
#endif /* PAGE_H_ */
