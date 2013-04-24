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
			Request(s), s(s) {
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
						int nBegin, vBegin, lastLen;
						{
							nBegin = This->tmpbuffer.length();
							CP::StreamWriter sw(This->tmpbuffer);
							cppsp::urlDecode(name, nameLen, sw);
							sw.flush();
							vBegin = This->tmpbuffer.length();
							if (value != NULL) {
								cppsp::urlDecode(value, valueLen, sw);
							}
							lastLen = This->tmpbuffer.length();
						}
						This->queryString[ {(char*)This->tmpbuffer.data()+nBegin,vBegin-nBegin}]
						= {(char*)This->tmpbuffer.data()+vBegin,lastLen-vBegin};
					}
				} cb {this};
				cppsp::parseQueryString(q + 1, path + pathLen - q - 1, &cb, false);
				this->path = string(path, q - path);
			}
		} else {
			uint8_t* lineBuf = _lineBuffer.data();
			int lineBufLen = _lineBuffer.length();
			uint8_t* tmp = (uint8_t*) memchr(lineBuf, ':', lineBufLen);
			if (tmp == NULL || tmp == lineBuf) goto fail;
			uint8_t* tmp1 = tmp - 1;
			while (tmp1 > lineBuf && *tmp1 == ' ')
			tmp1--;
			int nBegin=tmpbuffer.length();
			tmpbuffer.write(lineBuf, tmp1 - lineBuf + 1);
			//string name((char*) lineBuf, tmp1 - lineBuf + 1);

			tmp1 = tmp + 1;
			while (tmp1 < (lineBuf + lineBufLen) && *tmp1 == ' ') tmp1++;
			int vBegin=tmpbuffer.length();
			tmpbuffer.write(tmp1, lineBuf + lineBufLen - tmp1);
			int lastLen=tmpbuffer.length();
			//string value((char*) tmp1, lineBuf + lineBufLen - tmp1);

			headers.insert(make_pair( String {(char*)tmpbuffer.data()+nBegin,vBegin-nBegin},
							String {(char*)tmpbuffer.data()+vBegin,lastLen-vBegin}));
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
