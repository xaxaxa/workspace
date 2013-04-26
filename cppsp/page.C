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
using namespace CP;
namespace cppsp
{
	
	Page::Page() :
			doRender(true) {
		
	}
	Page::Page(Request& req, Response& resp, CP::StringPool* sp) :
			request(&req), response(&resp), sp(sp), doRender(true) {
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
			//response->writeHeaders();
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
	string Page::mapPath(string path) {
		return server->mapPath(mapRelativePath(path));
	}
	string Page::mapRelativePath(string path) {
		char tmp[request->path.length() + path.length()];
		int l = combinePath(request->path.data(), request->path.length(), path.data(), path.length(),
				tmp);
		return string(tmp, l);
	}
	void Page::loadNestedPage(string path, Delegate<void(Page*, exception* ex)> cb) {
		this->pageCB = cb;
		string tmp = mapRelativePath(path);
		server->loadPage(*poll, { tmp.data(), (int) tmp.length() }, { &Page::_pageCB, this });
	}
	void Page::loadNestedPageFromFile(string path, Delegate<void(Page*, exception* ex)> cb) {
		this->pageCB = cb;
		server->loadPageFromFile(*poll, { path.data(), (int) path.length() },
				{ &Page::_pageCB, this });
	}
	
	void Page::_pageCB(Page* p, exception* ex) {
		if (p != NULL) {
			p->request = request;
			p->response = response;
			p->poll = poll;
			p->server = server;
		}
		pageCB(p, ex);
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

	Request::Request(CP::Stream& inp, CP::StringPool* sp) :
			inputStream(&inp), input(inp), sp(sp) {
	}
	Request::~Request() {
	}
	
	void Request::readPost(Delegate<void(Request&)> cb) {
		_readPOST.cb = cb;
		_readPOST.ms = new (_readPOST.ms1) MemoryStream();
		_readPOST.tmp_i = _readPOST.ms->length();
		auto it = headers.find("Content-Length");
		if (it == headers.end()) {
			input.readToEnd(*_readPOST.ms, { &Request::_readCB, this });
		} else {
			input.readChunked(*_readPOST.ms, atoi((*it).second), { &Request::_readCB, this });
		}
	}
	void Request::_readCB(int i) {
		_readPOST.ms->flush();
		struct
		{
			Request* This;
			void operator()(const char* name, int nameLen, const char* value, int valueLen) {
				int nBegin, vBegin, lastLen;
				{
					nBegin = This->_readPOST.ms->length();
					CP::StreamWriter sw(*This->_readPOST.ms);
					cppsp::urlDecode(name, nameLen, sw);
					sw.flush();
					vBegin = This->_readPOST.ms->length();
					if (value != NULL) {
						cppsp::urlDecode(value, valueLen, sw);
					}
				}
				lastLen = This->_readPOST.ms->length();
				String n { This->sp->add((char*) This->_readPOST.ms->data() + nBegin, vBegin - nBegin),
						vBegin - nBegin };
				String v { This->sp->add((char*) This->_readPOST.ms->data() + vBegin, lastLen - vBegin),
						lastLen - vBegin };
				This->form[n] = v;
			}
		} cb { this };
		cppsp::parseQueryString((const char*) _readPOST.ms->data() + _readPOST.tmp_i, i, &cb, false);
		_readPOST.ms->~MemoryStream();
		_readPOST.cb(*this);
	}

	Response::Response(CP::Stream& out, CP::StringPool* sp) :
			outputStream(&out), output((CP::BufferedOutput&) buffer), sp(sp), headersWritten(false),
					closed(false), sendChunked(false) {
		statusCode = 200;
		statusName = "OK";
		addDefaultHeaders();
	}
	void Response::addDefaultHeaders() {
		headers.insert( { "Content-Type", "text/html; charset=UTF-8" });
	}
	void Response_doWriteHeaders(Response* This, CP::StreamWriter& sw) {
		sw.writeF("HTTP/1.1 %i %s\r\n", This->statusCode, This->statusName);
		for (auto it = This->headers.begin(); it != This->headers.end(); it++) {
			int l1 = (*it).first.length();
			int l2 = (*it).second.length();
			char* tmp = sw.beginWrite(l1 + 4 + l2);
			memcpy(tmp, (*it).first.data(), l1);
			tmp[l1] = ':';
			tmp[l1 + 1] = ' ';
			memcpy(tmp + l1 + 2, (*it).second.data(), l2);
			tmp[l1 + 2 + l2] = '\r';
			tmp[l1 + 2 + l2 + 1] = '\n';
			sw.endWrite(l1 + 4 + l2);
			//sw.writeF("%s: %s\r\n", (*it).first.c_str(), (*it).second.c_str());
		}
		sw.write("\r\n", 2);
	}
	void Response::flush(Callback cb) {
		//printf("flush\n");
		if (closed) throw runtime_error("connection has already been closed by the client");
		output.flush();
		this->_cb = cb;
		if (!headersWritten) {
			headersWritten = true;
			int bufferL = buffer.length();
			{
				CP::StreamWriter sw(buffer);
				auto it = this->headers.find("Content-Length");
				if (it == this->headers.end()) {
					char tmps[32];
					snprintf(tmps, 32, "%i", buffer.length());
					this->headers.insert(make_pair("Content-Length", tmps));
				}
				Response_doWriteHeaders(this, sw);
			}
			iov[0]= {buffer.data()+bufferL, (size_t)(buffer.length()-bufferL)};
			iov[1]= {buffer.data(), (size_t)bufferL};
			outputStream->writevAll(iov, 2, { &Response::_writeCB, this });
			return;
		} else {
			if (buffer.length() <= 0) {
				cb(*this);
				return;
			}
			outputStream->write(buffer.data(), buffer.length(), { &Response::_writeCB, this });
		}
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
	
	void Request::reset() {
		headers.clear();
		queryString.clear();
		form.clear();
	}
	void Response::reset() {
		headers.clear();
		addDefaultHeaders();
		output.flush();
		buffer.clear();
		headersWritten = false;
		closed = false;
		sendChunked = false;
	}

} /* namespace cppsp */

void cppsp::Server::loadPage(CP::Poll& p, String path, Delegate<void(Page*, exception* ex)> cb) {
	string tmp = mapPath(path.toSTDString());
	cppsp::loadPage(p, rootDir(), { tmp.data(), (int) tmp.length() }, cb);
}

void cppsp::Server::loadPageFromFile(CP::Poll& p, String path,
		Delegate<void(Page*, exception* ex)> cb) {
	cppsp::loadPage(p, rootDir(), path, cb);
}

string cppsp::Server::mapPath(string path) {
	char tmp[path.length() + strlen(rootDir())];
	int l = cppsp::combinePathChroot(rootDir(), path.c_str(), tmp);
	return string(tmp, l);
}
