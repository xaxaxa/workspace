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
namespace cppsp
{
	//static int CPollRequest::bufSize=4096;
	CPollRequest::CPollRequest(CP::Socket& s) :
			Request(s), s(s), sr(s) {

	}
	
	void CPollRequest::readHeaders(const Delegate<void()>& cb) {
		this->tmp_cb = cb;
		this->firstLine = true;
		_beginRead();
	}
	void CPollRequest::_beginRead() {
		ms.clear();
		//printf("\n_beginRead()\n");
		sr.readTo("\r\n", 2, ms, CP::StreamReader::StreamCallback(&CPollRequest::_readCB, this));
	}
	void CPollRequest::_readCB(int i) {
		if (i <= 0) goto fail;
		//write(2, ms.data(), ms.length());
		if (firstLine) {
			firstLine = false;
			uint8_t* lineBuf = ms.data();
			int lineBufLen = ms.length();
			uint8_t* tmp = (uint8_t*) memchr(lineBuf, ' ', lineBufLen);
			if (tmp == NULL) goto fail;
			method = string((char*) lineBuf, tmp - lineBuf);
			tmp++;
			if (lineBuf + lineBufLen - tmp <= 0) goto fail;
			uint8_t* tmp1 = (uint8_t*) memchr(tmp, ' ', lineBuf + lineBufLen - tmp);
			if (tmp1 == NULL) goto fail;
			const char* path = (const char*) tmp;
			int pathLen = tmp1 - tmp;
			if (pathLen <= 0) goto fail;

			const char* q = (const char*) memchr(path, '?', pathLen);
			if (q == NULL) this->path = string(path, pathLen);
			else {
				struct
				{
					CPollRequest* This;
					void operator()(const char* name, int nameLen, const char* value, int valueLen) {
						CP::StringStream n;
						CP::StringStream v;
						{
							CP::StreamWriter sw((CP::BufferedOutput&)n);
							cppsp::urlDecode(name, nameLen, sw);
						}
						if (value != NULL) {
							CP::StreamWriter sw((CP::BufferedOutput&)v);
							cppsp::urlDecode(value, valueLen, sw);
						}
						This->queryString[n.str()] = v.str();
					}
				} cb { this };
				cppsp::parseQueryString(q + 1, path + pathLen - q - 1, &cb, false);
				this->path = string(path, q - path);
			}
		} else {
			uint8_t* lineBuf = ms.data();
			int lineBufLen = ms.length();
			uint8_t* tmp = (uint8_t*) memchr(lineBuf, ':', lineBufLen);
			if (tmp == NULL || tmp == lineBuf) goto fail;
			uint8_t* tmp1 = tmp - 1;
			while (tmp1 > lineBuf && *tmp1 == ' ')
				tmp1--;
			string name((char*) lineBuf, tmp1 - lineBuf + 1);

			tmp1 = tmp + 1;
			while (tmp1 < (lineBuf + lineBufLen) && *tmp1 == ' ')
				tmp1++;
			string value((char*) tmp1, lineBuf + lineBufLen - tmp1);

			headers.insert(make_pair(name, value));
		}
		if (sr.eof) {
			fail: _endRead();
			return;
		}
		_beginRead();
	}
	void CPollRequest::_endRead() {
		tmp_cb();
	}
	CPollRequest::~CPollRequest() {
	}
}
