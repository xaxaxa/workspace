/*
 * cppsp_socketd.C
 *
 *  Created on: Mar 8, 2013
 *      Author: xaxaxa
 */
#include "include/cppsp_cpoll.H"
#include <stdio.h>
#include <unistd.h>
#include "include/stringutils.H"
using namespace CP;
namespace cppsp
{
	//static int CPollRequest::bufSize=4096;
	CPollRequest::CPollRequest(CP::Socket& s, CP::StringPool* sp) :
			Request(s, sp), _parser(&headers), s(s) {
	}
	bool CPollRequest_parseReqLine(CPollRequest* This) {
		uint8_t* lineBuf = (uint8_t*) This->_parser.reqLine.data();
		int lineBufLen = This->_parser.reqLine.length();
		uint8_t* tmp = (uint8_t*) memchr(lineBuf, ' ', lineBufLen);
		if (tmp == NULL) return false;
		This->method = {(char*) lineBuf, int(tmp - lineBuf)};
		tmp++;
		if (lineBuf + lineBufLen - tmp <= 0) return false;
		uint8_t* tmp1 = (uint8_t*) memchr(tmp, ' ', lineBuf + lineBufLen - tmp);
		if (tmp1 == NULL) return false;
		const char* path = (const char*) tmp;
		int pathLen = tmp1 - tmp;
		if (pathLen <= 0) return false;
		This->path = {path, pathLen};
		return true;
	}
	bool CPollRequest::readRequest(const Delegate<void(bool)>& cb) {
		if (_parser.process()) {
			if (CPollRequest_parseReqLine(this)) return true;
			else {
				cb(false);
				return false;
			}
		} else {
			this->tmp_cb = cb;
			//_parser.reset();
			_beginRead();
			return false;
		}
	}
	void CPollRequest::_beginRead() {
		String b = _parser.beginPutData(4096);
		inputStream->read(b.data(), b.length(), { &CPollRequest::_readCB, this });
	}
	void CPollRequest::_readCB(int i) {
		if (i <= 0) {
			tmp_cb(false);
			return;
		}
		_parser.endPutData(i);
		if (_parser.process()) {
			tmp_cb(CPollRequest_parseReqLine(this));
		} else {
			_beginRead();
		}
	}
	CPollRequest::~CPollRequest() {
	}
}
