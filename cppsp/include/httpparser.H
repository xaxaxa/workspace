/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
/*
 * httpparser.H
 *
 *  Created on: Apr 28, 2013
 *      Author: xaxaxa
 */

#ifndef HTTPPARSER_H_
#define HTTPPARSER_H_

#include <cpoll/cpoll.H>
#include "headercontainer.H"
namespace cppsp
{
	using namespace CP;
	struct HTTPParser
	{
		static inline char tolower(char c) {
			if (c <= 'Z' && c >= 'A') c = c - 'A' + 'a';
			return c;
		}
		//s2 must be all lowercase!!!!!
		static bool ci_equals(String s1, String s2) {
			if (s1.length() != s2.length()) return false;
			if (s1.length() == 0) return true;
			for (int i = 0; i < s1.length(); i++) {
				if (tolower(s1.data()[i]) != s2.data()[i]) return false;
			}
			return true;
		}
		headerContainer* hc;
		MemoryStream ms;
		String content;
		String reqLine;
		enum
		{
			s_readingHeaders = 1, s_readingContent
		} state = s_readingHeaders;
		int pos;
		int rpos;
		int _ctLen;
		int reqLine_i;
		bool firstLine = true;
		HTTPParser(headerContainer* hc) :
				hc(hc), ms(8192), pos(0), rpos(0), _ctLen(0) {
			hc->buffer = (char**) &ms.buffer;
		}
		String beginPutData(int len) {
			if (ms.bufferSize - ms.bufferPos < len) ms.flushBuffer(len);
			return {(char*)ms.buffer + ms.bufferPos,ms.bufferSize-ms.bufferPos};
		}
		void endPutData(int len) {
			ms.bufferPos += len;
			ms.flush();
		}
		//returns whether or not a complete http request was found
		//headers will be added to *hc, and content will be set to point
		//to any content received
		//note: *hc may be modified even when it returns false
		bool process() {
			if (pos >= ms.length()) return false;
			uint8_t* buf = ms.data();
			switch (state) {
				case s_readingHeaders:
				{
					aaaaa: void* tmp = memmem(buf + rpos, ms.length() - rpos, "\r\n", 2);
					if (tmp == NULL) {
						//minus one to catch any delimiters that might be cut off
						//in the middle
						if (rpos < ms.length() - 1) rpos = ms.length() - 1;
						return false;
					}
					int newpos = ((uint8_t*) tmp) - buf;
					//line: (buf+pos, newpos-pos)
					uint8_t* lineBuf = buf + pos;
					int lineBufLen = newpos - pos;

					if (firstLine) {
						reqLine.len = newpos - pos;
						reqLine_i = pos;
						firstLine = false;
					} else {
						if (lineBufLen == 0) {
							rpos = pos = newpos + 2;
							state = s_readingContent;
							goto asdfg;
						}
						tmp = memchr(buf + pos, ':', newpos - pos);
						if (tmp == NULL) {
							hc->add( { pos, newpos - pos, -1, 0 });
						} else {
							headerContainer::item it;
							uint8_t* tmp1 = (uint8_t*) tmp - 1;
							while (tmp1 >= lineBuf && *tmp1 == ' ')
								tmp1--;
							it.nameStart = pos;
							it.nameLength = (int) (tmp1 - lineBuf + 1);
							tmp1 = (uint8_t*) tmp + 1;
							while (tmp1 < (buf + newpos) && *tmp1 == ' ')
								tmp1++;
							it.valueStart = (int) (tmp1 - buf);
							it.valueLength = (int) (lineBuf + lineBufLen - tmp1);

							if (_ctLen == 0
									&& ci_equals( { (char*) buf + pos, it.nameLength }, "content-length")) {
								_ctLen = atoi( { (char*) tmp1, it.valueLength });
							}
							hc->add(it);
						}
					}
					rpos = pos = newpos + 2;
					goto aaaaa;
				}
				case s_readingContent:
				{
					asdfg: if (ms.length() - pos < _ctLen) return false;
					content= {(char*)buf+pos,_ctLen};
					pos += _ctLen;
					rpos = pos;
					_ctLen = 0;
					state = s_readingHeaders;
					firstLine = true;
					reqLine.d = (char*) buf + reqLine_i;
					return true;
				}
				default:
					return false;
			}
		}
		inline String getBufferData() {
			return {(char*)ms.buffer + pos, ms.length() - pos};
		}
		inline String getHistory(bool includeUnprocessed = true) {
			return {(char*)ms.buffer, includeUnprocessed ? ms.length() : pos};
		}
		//free up buffer space
		void reset() {
			if (pos > 0) {
				if (ms.length() - pos > 0) memmove(ms.buffer, ms.buffer + pos, ms.length() - pos);
				ms.len -= pos;
				rpos -= pos;
				pos = 0;
				ms.bufferPos = ms.len;
			}
		}
	};
}

#endif /* HTTPPARSER_H_ */
