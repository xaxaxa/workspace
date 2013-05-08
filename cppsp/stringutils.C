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
 * stringutils.C
 *
 *  Created on: Apr 9, 2013
 *      Author: xaxaxa
 */
#include <cpoll/cpoll.H>
#include "include/stringutils.H"
#include "include/split.H"
using namespace CP;
namespace cppsp
{
	inline char hexCharToInt(char ch) {
		if (ch <= '9') return ch - '0';
		else if (ch <= 'Z') return ch - 'A' + 10;
		else return ch - 'a' + 10;
	}
	inline char intToHexChar(char i) {
		if (i < 10) return i + '0';
		else return i - 10 + 'A';
	}
	void urlDecode(const char* in, int inLen, StreamWriter& sw) {
		const char* end = in + inLen;
		const char* ptr = in;
		while (true) {
			if (ptr >= end) goto E;
			const char* next = (const char*) memchr(ptr, '%', end - ptr);
			if (next == NULL) break;
			sw.write(ptr, next - ptr);
			if (next + 2 >= end) {
				sw.write(next, end - next);
				goto E;
			}
			char tmp = hexCharToInt(next[1]) << 4 | hexCharToInt(next[2]);
			sw.write(tmp);
			ptr = next + 3;
		}
		if (ptr < end) sw.write(ptr, end - ptr);
		E: ;
	}
	String urlDecode(const char* in, int inLen, StringPool& sp) {
		char* ch = sp.beginAdd(inLen); //output size will never exceed input size
		char* c = ch;
		const char* end = in + inLen;
		const char* ptr = in;
		while (true) {
			if (ptr >= end) goto E;
			const char* next = (const char*) memchr(ptr, '%', end - ptr);
			if (next == NULL) break;
			memcpy(c, ptr, next - ptr);
			c += (next - ptr);
			if (next + 2 >= end) {
				memcpy(c, next, end - next);
				c += (end - next);
				goto E;
			}
			*c = hexCharToInt(next[1]) << 4 | hexCharToInt(next[2]);
			c++;
			ptr = next + 3;
		}
		if (ptr < end) {
			memcpy(c, ptr, end - ptr);
			c += (end - ptr);
		}
		sp.endAdd(c - ch);
		return {ch,c-ch};
		E: ;
		return {(char*)nullptr,0};
	}
	void urlEncode(const char* in, int inLen, CP::StreamWriter& sw) {
		int last_i = 0;
		const char* c = in;
		char ch[3];
		ch[0] = '%';
		for (int i = 0; i < inLen; i++) {
			if ((48 <= c[i] && c[i] <= 57) || //0-9
					(65 <= c[i] && c[i] <= 90) || //abc...xyz
					(97 <= c[i] && c[i] <= 122) || //ABC...XYZ
					(c[i] == '~' || c[i] == '!' || c[i] == '*' || c[i] == '(' || c[i] == ')'
							|| c[i] == '\'')) continue;
			if (i > last_i) sw.write(in + last_i, i - last_i);
			last_i = i + 1;
			ch[1] = intToHexChar(c[i] >> 4);
			ch[2] = intToHexChar(c[i] & (char) 0xF);
			sw.write(ch, 3);
		}
		if (inLen > last_i) sw.write(in + last_i, inLen - last_i);
	}
	std::string urlDecode(const char* in, int inLen) {
		StringStream ss;
		{
			StreamWriter sw(ss);
			urlDecode(in, inLen, sw);
		}
		return ss.str();
	}
	std::string urlEncode(const char* in, int inLen) {
		StringStream ss;
		{
			StreamWriter sw(ss);
			urlEncode(in, inLen, sw);
		}
		return ss.str();
	}
	std::string htmlEscape(const char* in, int inLen) {
		StringStream ss;
		{
			StreamWriter sw(ss);
			htmlEscape(in, inLen, sw);
		}
		return ss.str();
	}
	std::string htmlAttributeEscape(const char* in, int inLen) {
		StringStream ss;
		{
			StreamWriter sw(ss);
			htmlAttributeEscape(in, inLen, sw);
		}
		return ss.str();
	}
	void parseQueryString(const char* in, int inLen, queryStringCallback cb, bool decode) {
		if (decode) {
			MemoryStream ms;
			StreamWriter sw(ms);
			split spl(in, inLen, '&');
			while (spl.read()) {
				const char* s = spl.value.d;
				int l = spl.value.len;
				const char* _end = s + l;
				const char* tmp = (const char*) memchr(s, '=', l);
				if (tmp == NULL) {
					urlDecode(s, l, sw);
					sw.flush();
					cb((const char*) ms.data(), ms.length(), nullptr, 0);
					ms.clear();
				} else {
					urlDecode(s, tmp - s, sw);
					sw.flush();
					int i = ms.length();
					urlDecode(tmp + 1, _end - tmp - 1, sw);
					sw.flush();
					cb((const char*) ms.data(), i, (const char*) (ms.data() + i), ms.length() - i);
					ms.clear();
				}
			}
		} else {
			split spl(in, inLen, '&');
			while (spl.read()) {
				const char* s = spl.value.d;
				int l = spl.value.len;
				const char* _end = s + l;
				const char* tmp = (const char*) memchr(s, '=', l);
				if (tmp == NULL) cb(s, l, nullptr, 0);
				else cb(s, tmp - s, tmp + 1, _end - tmp - 1);
			}
		}
	}
	void htmlEscape(const char* in, int inLen, CP::StreamWriter& sw) {
		int last_i = 0;
		const char* tmp;
		for (int i = 0; i < inLen; i++) {
			switch (in[i]) {
				case '&':
					tmp = "&amp;";
					break;
				case '<':
					tmp = "&lt;";
					break;
				case '>':
					tmp = "&gt;";
					break;
				default:
					continue;
			}
			if (i > last_i) sw.write(in + last_i, i - last_i);
			last_i = i + 1;
			sw.write(tmp);
		}
		if (inLen > last_i) sw.write(in + last_i, inLen - last_i);
	}
	void htmlAttributeEscape(const char* in, int inLen, CP::StreamWriter& sw) {
		int last_i = 0;
		const char* tmp;
		for (int i = 0; i < inLen; i++) {
			switch (in[i]) {
				case '&':
					tmp = "&amp;";
					break;
				case '<':
					tmp = "&lt;";
					break;
				case '>':
					tmp = "&gt;";
					break;
				case '"':
					tmp = "&quot;";
					break;
				case '\'':
					tmp = "&apos;";
					break;
				default:
					continue;
			}
			if (i > last_i) sw.write(in + last_i, i - last_i);
			last_i = i + 1;
			sw.write(tmp);
		}
		if (inLen > last_i) sw.write(in + last_i, inLen - last_i);
	}

}

