/*
 * page.H
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#ifndef PAGE_H_
#define PAGE_H_
#include <rgc.H>
#include <unordered_map>
#include <cpoll.H>
#include "stringutils.H"
using namespace std;
namespace cppsp
{
	typedef CP::String String;
	class Request: public RGC::Object
	{
	public:
		Request(CP::Stream& inp, CP::StringPool* sp);
		virtual ~Request();
		RGC::Ref<CP::Stream> inputStream;
		CP::StreamReader input;
		CP::StringPool* sp; //may be used to store headers, querystrings, and POST data
		typedef unordered_map<String, String, std::hash<String>, equal_to<String>,
				CP::PoolAllocator<std::pair<String, String> > > StringMap;
		CP::PoolAllocator<std::pair<const String, String> > alloc;
		StringMap queryString;
		StringMap headers;
		StringMap form;
		String method;
		String path;
		//string httpVersion;
		union
		{
			struct
			{
				DelegateBase<void(Request&)> cb;
				CP::MemoryStream* ms;
				char ms1[sizeof(CP::MemoryStream)];
				int tmp_i;
			} _readPOST;
		};

		//read POST data; headers are always read before creating the Request object
		virtual void readPost(Delegate<void(Request&)> cb);

		void _readCB(int i);
		virtual void reset();

		//prepare for StringPool reset
		virtual void partialDestruct();
		virtual void partialConstruct();
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
		typedef unordered_map<String, String, std::hash<String>, equal_to<String>,
				CP::PoolAllocator<std::pair<String, String> > > StringMap;
		CP::PoolAllocator<std::pair<String, String> > alloc;
		StringMap headers;
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
		template<class ... T>
		void write(T&&... a) {
			output.write(std::forward<T>(a)...);
		}
		void _writeCB(int r);
		virtual void reset();
		virtual void partialDestruct();
		virtual void partialConstruct();
	};
	class Page;
	class Server: public RGC::Object
	{
	public:
		virtual const char* rootDir()=0;
		//path is relative to rootDir
		virtual void loadPage(CP::Poll& p, String path, Delegate<void(Page*, exception* ex)> cb);
		virtual void loadPageFromFile(CP::Poll& p, String path,
				Delegate<void(Page*, exception* ex)> cb);
		//path is relative to rootDir
		virtual string mapPath(string path);
	};
	class Page: public RGC::Object
	{
	public:
		typedef Delegate<void(Page&)> Callback;
		Page();
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
		inline void cancelRender() {
			doRender = false;
		}
		//pointer to mmap()ed address of original cppsp file
		const uint8_t* __stringTable;
		void __writeStringTable(int i, int len);
		string mapPath(string path);
		string mapRelativePath(string path);
		//path is relative to current page
		void loadNestedPage(string path, Delegate<void(Page*, exception* ex)> cb);
		//path is absolute
		void loadNestedPageFromFile(string path, Delegate<void(Page*, exception* ex)> cb);
		virtual void handleRequest(Callback cb = nullptr);
		virtual void processRequest();
		//each page goes through these operations (in order): init, [readPost], load, render, flush, finalize

		virtual void init(); //to be overridden by base class; called before load() is called, and
									//must call initCB() or cancelLoad() after it's done
		virtual void doInit();
		virtual void initCB();
		virtual void cancelLoad(exception* ex = NULL);
		virtual void load(); //to be overridden by derived class; called before headers are written
		virtual void render(CP::StreamWriter& out);
		virtual void flush();
		virtual void flushCB();
		virtual void finalize();
		virtual void finalizeCB();
		virtual ~Page();

		void _readPOSTCB(Request& r);
		void _flushCB(Response& r);
		void _pageCB(Page* p, exception* ex);
	};

} /* namespace cppsp */
#endif /* PAGE_H_ */
