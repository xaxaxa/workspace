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
			flush();
		}
	}
	void Page::processRequest() {
		doInit();
	}
	void Page::render(CP::StreamWriter& out) {
		out.write("This is the default page of the cppsp C++ "
				"web application framework. If you see this, it means "
				"you haven't overridden the render() method derived from cppsp::Page.");
	}
	void Page::init() {

	}
	void Page::doInit() {
		doReadPost = false;
		init();
		if (doReadPost && request->method.compare("POST") == 0) request->readPost( {
				&Page::_readPOSTCB, this });
		else initCB();
	}
	void Page::initCB() {
		try {
			load();
			response->writeHeaders();
			if (doRender) render(response->output);
			flush();
		} catch (exception& ex) {
			handleError(&ex, *response, this->filePath);
			flush();
		}
	}
	void Page::cancelLoad(exception* ex) {
		if (ex == NULL) {
			runtime_error re("Web page execution cancelled");
			handleError(&re, *response, this->filePath);
		} else handleError(ex, *response, this->filePath);
		response->flush( { &Page::_flushCB, this });
	}
	void Page::_readPOSTCB(Request& r) {
		initCB();
	}
	void Page::flush() {
		if (response->closed) finalizeCB();
		else response->flush( { &Page::_flushCB, this });
	}
	void Page::_flushCB(Response& r) {
		flushCB();
	}
	void Page::flushCB() {
		finalize();
	}
	void Page::finalize() {
		finalizeCB();
	}
	void Page::finalizeCB() {
		if (this->cb != nullptr) cb(*this);
	}

	Request::Request(CP::Stream& inp) :
			inputStream(&inp), input(inp) {
	}
	Request::~Request() {
	}
	
	void Request::readPost(Delegate<void(Request&)> cb) {
		_readPOST.cb = cb;
		_readPOST.tmp_i = tmpbuffer.length();
		auto it = headers.find("Content-Length");
		if (it == headers.end()) {
			input.readToEnd(tmpbuffer, { &Request::_readCB, this });
		} else {
			input.readChunked(tmpbuffer, atoi((*it).second.c_str()), { &Request::_readCB, this });
		}
	}
	void Request::_readCB(int i) {
		tmpbuffer.flush();
		struct
		{
			Request* This;
			void operator()(const char* name, int nameLen, const char* value, int valueLen) {
				CP::StringStream n;
				CP::StringStream v;
				{
					CP::StreamWriter sw((CP::BufferedOutput&) n);
					cppsp::urlDecode(name, nameLen, sw);
				}
				if (value != NULL) {
					CP::StreamWriter sw((CP::BufferedOutput&) v);
					cppsp::urlDecode(value, valueLen, sw);
				}
				This->form[n.str()] = v.str();
			}
		} cb { this };
		cppsp::parseQueryString((const char*) tmpbuffer.data() + _readPOST.tmp_i, i, &cb, false);
		_readPOST.cb(*this);
	}

	Response::Response(CP::Stream& out) :
			outputStream(&out), output((CP::BufferedOutput&) buffer), closed(false) {
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
		if (closed) throw runtime_error("connection has already been closed by the client");
		output.flush();
		if (buffer.length() <= 0) {
			cb(*this);
			return;
		}
		this->_cb = cb;
		outputStream->write(buffer.data(), buffer.length(), { &Response::_writeCB, this });
	}
	void Response::clear() {
		output.flush();
		buffer.clear();
		headersWritten = false;
	}
	void Response::_writeCB(int r) {
		if (r <= 0) closed = true;
		buffer.clear();
		_cb(*this);
	}

} /* namespace cppsp */

void cppsp::Server::loadPage(CP::Poll& p, string path, Delegate<void(Page*, exception* ex)> cb) {
	cppsp::loadPage(p, rootDir(), mapPath(path), cb);
}

void cppsp::Server::loadPageFromFile(CP::Poll& p, string path,
		Delegate<void(Page*, exception* ex)> cb) {
	cppsp::loadPage(p, rootDir(), path, cb);
}

string cppsp::Server::mapPath(string path) {
	char tmp[path.length() + strlen(rootDir())];
	int l = cppsp::combinePathChroot(rootDir(), path.c_str(), tmp);
	return string(tmp, l);
}
