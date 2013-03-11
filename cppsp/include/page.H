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
	class Request
	{
		RGC::Ref<CP::Stream> inputStream;
		map<string, string> queryString;
		map<string, string> headers;
		map<string, string> form;

		virtual void read()=0;
		virtual ~Request();
	};
	class Response
	{
		RGC::Ref<CP::Stream> outputStream;
		map<string, string> headers;
		bool headersWritten;
		virtual void writeHeaders()=0;
		virtual void write(string s)=0;
	};
	class Page
	{
	public:
		virtual const char* getFilePath()=0;
		Page();
		void handleRequest(Request& req, Response& resp);
		void render(CP::Stream& s);
		virtual ~Page();
	};

} /* namespace cppsp */
#endif /* PAGE_H_ */
