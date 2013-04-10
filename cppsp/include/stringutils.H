/*
 * urlparser.H
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
	void urlDecode(const char* in, int inLen, CP::StreamWriter& sw);
	void urlEncode(const char* in, int inLen, CP::StreamWriter& sw);
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
	std::string htmlEscape(const char* in, int inLen);
	static inline std::string htmlEscape(const char* in) {
		return htmlEscape(in, strlen(in));
	}
	static inline std::string htmlEscape(std::string in) {
		return htmlEscape(in.data(), in.length());
	}
}

#endif /* URLPARSER_H_ */
