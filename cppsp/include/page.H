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
		Response(CP::Stream& out);
		RGC::Ref<CP::Stream> outputStream;
		CP::StreamWriter output;
		map<string, string> headers;
		const char* statusName;
		int statusCode;

		bool headersWritten;
		virtual void writeHeaders();

		template<class ... T>
		void write(T&&... a) {
			if(!headersWritten)writeHeaders();
			output.write(std::forward<T>(a)...);
		}
	};
	class Page: public RGC::Object
	{
	public:
		Page();
		Page(Request& req, Response& resp);
		RGC::Ref<Request> request;
		RGC::Ref<Response> response;
		string filePath;
		//pointer to mmap()ed address of original cppsp file
		const uint8_t* __stringTable;
		void __writeStringTable(int i, int len);
		virtual void handleRequest();
		virtual void render(CP::StreamWriter& out);
		virtual ~Page();
	};

} /* namespace cppsp */
#endif /* PAGE_H_ */
