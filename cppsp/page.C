/*
 * page.C
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#include "include/page.H"
#include "include/common.H"
#include <stdexcept>
namespace cppsp
{
	
	Page::Page() :
			doRender(true) {
		
	}
	Page::Page(Request& req, Response& resp) :
			request(&req), response(&resp), doRender(true) {
	}
	void Page::__writeStringTable(int i, int len) {
		response->output.write(__stringTable + i, len);
	}
	void Page::load() {

	}
	Page::~Page() {
	}
	void Page::handleRequest(Callback cb) {
		this->cb = cb;
		try {
			processRequest();
		} catch (exception& ex) {
			handleError(&ex, *response, this->filePath);
			response->flush( { &Page::_flushCB, this });
		}
	}
	void Page::processRequest() {
		init();
	}
	void Page::render(CP::StreamWriter& out) {
		out.write("This is the default page of the cppsp C++ "
				"web application framework. If you see this, it means "
				"you haven't overridden the render() method derived from cppsp::Page.");
	}
	void Page::init() {
		initCB();
	}
	void Page::initCB() {
		load();
		response->writeHeaders();
		if (doRender) render(response->output);
		response->flush( { &Page::_flushCB, this });
	}
	void Page::cancelLoad(exception* ex) {
		if (ex == NULL) {
			runtime_error re("Web page execution cancelled");
			handleError(&re, *response, this->filePath);
		} else handleError(ex, *response, this->filePath);
		response->flush( { &Page::_flushCB, this });
	}
	
	void Page::_flushCB(Response& r) {
		if (this->cb != nullptr) cb(*this);
	}
	Request::Request(CP::Stream& inp) :
			inputStream(&inp) {
	}
	Request::~Request() {
	}
	Response::Response(CP::Stream& out) :
			outputStream(&out), output(buffer) {
		statusCode = 200;
		statusName = "OK";
		headers.insert( { "Connection", "close" });
		headers.insert( { "Content-Type", "text/html; charset=UTF-8" });
		headersWritten = false;
	}
	
	void Response::doWriteHeaders() {
		output.writeF("HTTP/1.1 %i %s\r\n", statusCode, statusName);
		for (auto it = headers.begin(); it != headers.end(); it++) {
			output.writeF("%s: %s\r\n", (*it).first.c_str(), (*it).second.c_str());
		}
		output.write("\r\n");
	}
	void Response::flush(Callback cb) {
		this->_cb = cb;
		output.flush();
		outputStream->write(buffer.data(), buffer.length(), { &Response::_writeCB, this });
	}
	void Response::clear() {
		output.flush();
		buffer.clear();
		headersWritten = false;
	}
	void Response::_writeCB(int r) {
		buffer.clear();
		_cb(*this);
	}

} /* namespace cppsp */

void cppsp::Server::loadPage(CP::Poll& p, string path, Delegate<void(Page*, exception* ex)> cb) {
	char tmp[path.length() + strlen(rootDir())];
	int l = cppsp::combinePathChroot(rootDir(), path.c_str(), tmp);
	string path1(tmp, l);
	cppsp::loadPage(p, rootDir(), path1, cb);
}
