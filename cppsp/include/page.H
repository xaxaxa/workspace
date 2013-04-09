/*
 * page.H
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#ifndef PAGE_H_
#define PAGE_H_
#include <rgc.H>
#include <string>
#include <map>
#include <cpoll.H>
using namespace std;
namespace cppsp
{
	class Request: public RGC::Object
	{
	public:
		Request(CP::Stream& inp);
		virtual ~Request();
		RGC::Ref<CP::Stream> inputStream;
		map<string, string> queryString;
		map<string, string> headers;
		map<string, string> form;
		string method;
		string path;
		//string httpVersion;
		Delegate<void(Request*)> _readCB;

		//read POST data; headers are always read before creating the Request object
		//virtual void read(const Delegate<void(Request*)>& cb);
	};
	class Response: public RGC::Object
	{
	public:
		typedef Delegate<void(Response&)> Callback;
		Response(CP::Stream& out);
		RGC::Ref<CP::Stream> outputStream;
		CP::MemoryStream buffer;
		CP::StreamWriter output;
		Callback _cb;
		map<string, string> headers;
		const char* statusName;
		int statusCode;

		bool headersWritten;
		virtual void doWriteHeaders();
		void writeHeaders() {
			if (!headersWritten) {
				doWriteHeaders();
				headersWritten = true;
			}
		}
		virtual void flush(Callback cb);
		virtual void clear();
		template<class ... T>
		void write(T&&... a) {
			writeHeaders();
			output.write(std::forward<T>(a)...);
		}

		void _writeCB(int r);
	};
	class Page;
	class Server: public RGC::Object
	{
	public:
		virtual const char* rootDir()=0;
		//path is relative to rootDir
		virtual void loadPage(CP::Poll& p, string path, Delegate<void(Page*, exception* ex)> cb);
	};
	class Page: public RGC::Object
	{
	public:
		typedef Delegate<void(Page&)> Callback;
		Page();
		Page(Request& req, Response& resp);
		RGC::Ref<Request> request;
		RGC::Ref<Response> response;
		CP::Poll* poll;
		Server* server;
		Callback cb;
		string filePath;
		bool doRender;
		inline void cancelRender() {
			doRender = false;
		}
		//pointer to mmap()ed address of original cppsp file
		const uint8_t* __stringTable;
		void __writeStringTable(int i, int len);
		virtual void handleRequest(Callback cb = nullptr);
		virtual void processRequest();
		virtual void init(); //to be overridden by base class; called before load() is called, and
									//must call initCB() or cancelLoad() after it's done
		virtual void initCB();
		virtual void cancelLoad(exception* ex=NULL);
		virtual void load(); //to be overridden by base class; called before headers are written
		virtual void render(CP::StreamWriter& out);
		virtual ~Page();

		void _flushCB(Response& r);
	};

} /* namespace cppsp */
#endif /* PAGE_H_ */
