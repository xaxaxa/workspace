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
		RGC::Ref<CP::Stream> inputStream;
		map<string, string> queryString;
		map<string, string> headers;
		map<string, string> form;
		Delegate<void(Request*)> _readCB;

		//read POST data; headers are always read before creating the Request object
		virtual void read(const Delegate<void(Request*)>& cb);
		virtual ~Request();
	};
	class Response: public RGC::Object
	{
	public:
		RGC::Ref<CP::Stream> outputStream;
		map<string, string> headers;
		bool headersWritten;
		void writeHeaders();

		void write(string s);
	};
	class Page: public RGC::Object
	{
	public:
		RGC::Ref<Request> request;
		RGC::Ref<Response> response;
		virtual const char* getFilePath()=0;
		Page();
		void handleRequest(Request& req, Response& resp);
		void render(CP::Stream& s);
		virtual ~Page();
	};

} /* namespace cppsp */
#endif /* PAGE_H_ */
