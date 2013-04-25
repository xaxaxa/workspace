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
			Request(s, sp), s(s) {
	}
	
	void CPollRequest::readHeaders(const Delegate<void(bool)>& cb) {
		this->tmp_cb = cb;
		this->firstLine = true;
		_beginRead();
	}
	void CPollRequest::_beginRead() {
		//printf("\n_beginRead()\n");
		_lineBuffer.clear();
		input.readTo("\r\n", 2, _lineBuffer,
				CP::StreamReader::StreamCallback(&CPollRequest::_readCB, this));
	}
	void CPollRequest::_readCB(int i) {
		if (i <= 0) goto end;
		//write(2, ms.data(), ms.length());
		if (firstLine) {
			firstLine = false;
			uint8_t* lineBuf = _lineBuffer.data();
			int lineBufLen = _lineBuffer.length();
			uint8_t* tmp = (uint8_t*) memchr(lineBuf, ' ', lineBufLen);
			if (tmp == NULL) goto fail;
			method = sp->addString((char*) lineBuf, tmp - lineBuf);
			tmp++;
			if (lineBuf + lineBufLen - tmp <= 0) goto fail;
			uint8_t* tmp1 = (uint8_t*) memchr(tmp, ' ', lineBuf + lineBufLen - tmp);
			if (tmp1 == NULL) goto fail;
			const char* path = (const char*) tmp;
			int pathLen = tmp1 - tmp;
			if (pathLen <= 0) goto fail;

			const char* q = (const char*) memchr(path, '?', pathLen);
			if (q == NULL) this->path = sp->addString(path, pathLen);
			else {
				struct
				{
					CPollRequest* This;
					MemoryStream ms;
					void operator()(const char* name, int nameLen, const char* value, int valueLen) {
						String n, v;
						{
							CP::StreamWriter sw(ms);
							cppsp::urlDecode(name, nameLen, sw);
							sw.flush();
							n= {This->sp->add((const char*)ms.data(),ms.length()),ms.length()};
							ms.clear();
							if (value != NULL) {
								cppsp::urlDecode(value, valueLen, sw);
							}
						}
						v= {This->sp->add((const char*)ms.data(),ms.length()),ms.length()};
						This->queryString[n] = v;
					}
				} cb { this };
				cppsp::parseQueryString(q + 1, path + pathLen - q - 1, &cb, false);
				this->path = sp->addString(path, q - path);
			}
		} else {
			uint8_t* lineBuf = _lineBuffer.data();
			int lineBufLen = _lineBuffer.length();
			uint8_t* tmp = (uint8_t*) memchr(lineBuf, ':', lineBufLen);
			if (tmp == NULL || tmp == lineBuf) goto fail;
			uint8_t* tmp1 = tmp - 1;
			while (tmp1 > lineBuf && *tmp1 == ' ')
				tmp1--;
			String n { sp->add((const char*) lineBuf, tmp1 - lineBuf + 1), tmp1 - lineBuf + 1 };

			tmp1 = tmp + 1;
			while (tmp1 < (lineBuf + lineBufLen) && *tmp1 == ' ')
				tmp1++;
			String v { sp->add((const char*) tmp1, lineBuf + lineBufLen - tmp1), lineBuf + lineBufLen
					- tmp1 };

			headers.insert(make_pair(n, v));
		}
		if (input.eof) {
			end: _endRead(!firstLine);
			return;
		}
		_beginRead();
		return;
		fail: _endRead(false);
	}
	void CPollRequest::_endRead(bool success) {
		_lineBuffer.clear();
		tmp_cb(success);
	}
	CPollRequest::~CPollRequest() {
	}
}
