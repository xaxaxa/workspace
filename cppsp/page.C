/*
 * page.C
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#include "include/page.H"

namespace cppsp
{
	
	Page::Page() {
		
	}
	Page::Page(Request& req, Response& resp) :
			request(&req), response(&resp) {
	}
	
	void Page::__writeStringTable(int i, int len) {
		response->output.write(__stringTable + i, len);
	}
	
	Page::~Page() {
	}
	void Page::handleRequest() {
		response->writeHeaders();
		render(response->output);
	}
	void Page::render(CP::StreamWriter& out) {
		out.write("This is the default page of the cppsp C++ "
				"web application framework. If you see this, it means "
				"you haven't overridden the render() method derived from cppsp::Page.");
	}
	Request::Request(CP::Stream& inp) :
			inputStream(&inp) {
	}
	Request::~Request() {
	}
	Response::Response(CP::Stream& out) :
			outputStream(&out), output(out) {
		statusCode = 200;
		statusName = "OK";
		headers.insert( { "Connection", "close" });
		headers.insert( { "Content-Type", "text/html" });
		headersWritten = false;
	}
	
	void Response::writeHeaders() {
		if (headersWritten) return;
		headersWritten = true;
		output.writeF("HTTP/1.1 %i %s\r\n", statusCode, statusName);
		for (auto it = headers.begin(); it != headers.end(); it++) {
			output.writeF("%s: %s\r\n", (*it).first.c_str(), (*it).second.c_str());
		}
		output.write("\r\n");
	}
} /* namespace cppsp */

