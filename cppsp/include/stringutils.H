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
	struct ci_less: std::binary_function<std::string, std::string, bool>
	{
		// case-independent (ci) compare_less binary function
		struct nocase_compare: public std::binary_function<unsigned char, unsigned char, bool>
		{
			bool operator()(const unsigned char& c1, const unsigned char& c2) const {
				return tolower(c1) < tolower(c2);
			}
		};
		bool operator()(const std::string & s1, const std::string & s2) const {
			return std::lexicographical_compare(s1.begin(), s1.end(), // source range
					s2.begin(), s2.end(), // dest range
					nocase_compare()); // comparison
		}
	};

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

	void htmlAttributeEscape(const char* in, int inLen, CP::StreamWriter& sw);
	std::string htmlAttributeEscape(const char* in, int inLen);
	static inline std::string htmlAttributeEscape(const char* in) {
		return htmlAttributeEscape(in, strlen(in));
	}
	static inline std::string htmlAttributeEscape(std::string in) {
		return htmlAttributeEscape(in.data(), in.length());
	}
}

#endif /* URLPARSER_H_ */
