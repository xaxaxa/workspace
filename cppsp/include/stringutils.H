/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
/*
 * stringutils.H
 *
 *  Created on: Apr 9, 2013
 *      Author: xaxaxa
 */

#ifndef URLPARSER_H_
#define URLPARSER_H_
#include <cpoll/cpoll.H>
#include <string>
namespace cppsp
{
	typedef CP::String String;
#ifndef __CPPSP_TOLOWER
#define __CPPSP_TOLOWER
	static inline char tolower(char c) {
		if (c <= 'Z' && c >= 'A') c = c - 'A' + 'a';
		return c;
	}
#endif

	int urlDecode(const char* in, int inLen, CP::StreamWriter& sw);
	static inline int urlDecode(CP::String in, CP::StreamWriter& sw) {
		return urlDecode(in.data(), in.length(), sw);
	}
	CP::String urlDecode(const char* in, int inLen, CP::StringPool& sp);
	int urlEncode(const char* in, int inLen, CP::StreamWriter& sw);
	static inline int urlEncode(CP::String in, CP::StreamWriter& sw) {
		urlEncode(in.data(), in.length(), sw);
	}
	std::string urlDecode(const char* in, int inLen);
	std::string urlEncode(const char* in, int inLen);
	static inline std::string urlDecode(const char* in) {
		return urlDecode(in, strlen(in));
	}
	static inline std::string urlEncode(const char* in) {
		return urlEncode(in, strlen(in));
	}
	static inline std::string urlDecode(std::string in) {
		return urlDecode(in.data(), in.length());
	}
	static inline std::string urlEncode(std::string in) {
		return urlEncode(in.data(), in.length());
	}

	typedef Delegate<void(const char* name, int nameLen, const char* value, int valueLen)> queryStringCallback;
	void parseQueryString(const char* in, int inLen, queryStringCallback cb, bool decode = true);

	void htmlEscape(const char* in, int inLen, CP::StreamWriter& sw);
	static inline void htmlEscape(CP::String in, CP::StreamWriter& sw) {
		htmlEscape(in.data(), in.length(), sw);
	}
	std::string htmlEscape(const char* in, int inLen);
	static inline std::string htmlEscape(const char* in) {
		return htmlEscape(in, strlen(in));
	}
	static inline std::string htmlEscape(std::string in) {
		return htmlEscape(in.data(), in.length());
	}

	void htmlAttributeEscape(const char* in, int inLen, CP::StreamWriter& sw);
	static inline void htmlAttributeEscape(CP::String in, CP::StreamWriter& sw) {
		htmlAttributeEscape(in.data(), in.length(), sw);
	}
	std::string htmlAttributeEscape(const char* in, int inLen);
	static inline std::string htmlAttributeEscape(const char* in) {
		return htmlAttributeEscape(in, strlen(in));
	}
	static inline std::string htmlAttributeEscape(std::string in) {
		return htmlAttributeEscape(in.data(), in.length());
	}

	int ci_compare(CP::String s1, CP::String s2);
	struct ci_less: std::binary_function<CP::String, CP::String, bool>
	{
		bool operator()(const CP::String & s1, const CP::String & s2) const {
			return ci_compare(s1, s2) < 0;
		}
	};

	/**
	 Combine two paths. Result is stored in buf. Does not write a null byte.
	 The memory block pointed to by buf must be at least l1 + l2 bytes.
	 @return the length of the string written to buf
	 */
	int combinePath(const char* p1, int l1, const char* p2, int l2, char* buf);
	/**
	 Combine two paths. Result is stored in buf. Does not write a null byte.
	 The memory block pointed to by buf must be at least strlen(p1) + strlen(p2) bytes.
	 @return the length of the string written to buf
	 */
	int combinePath(const char* p1, const char* p2, char* buf);
	/**
	 Combine two paths securely. The resulting path is guaranteed to be under p1. (The user can not use ".." to leave the root directory)
	 The memory block pointed to by buf must be at least l1 + l2 bytes.
	 @return the length of the string written to buf
	 */
	int combinePathChroot(const char* p1, int l1, const char* p2, int l2, char* buf);
	/**
	 Combine two paths securely. The resulting path is guaranteed to be under p1. (The user can not use ".." to leave the root directory)
	 The memory block pointed to by buf must be at least strlen(p1) + strlen(p2) bytes.
	 @return the length of the string written to buf
	 */
	int combinePathChroot(const char* p1, const char* p2, char* buf);
	/**
	 Combine two paths. Result is allocated from sp.
	 @return the resulting path
	 */
	String combinePath(String p1, String p2, CP::StringPool& sp);
	/**
	 Combine two paths securely. The resulting path is guaranteed to be under p1. (The user can not use ".." to leave the root directory)
	 @return the resulting path
	 */
	String combinePathChroot(String p1, String p2, CP::StringPool& sp);
	/**
	 c should be of at least 32 chars
	 */
	int rfctime(const tm& time, char* c);
}

#endif /* URLPARSER_H_ */
