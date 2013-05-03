/*
 * split.H
 *
 *  Created on: May 1, 2013
 *      Author: xaxaxa
 */

#ifndef SPLIT_H_
#define SPLIT_H_
#include <cpoll/basictypes.H>
namespace cppsp
{
	struct split
	{
		const char* s;
		const char* end;
		const char* s1;
		CP::String value;
		char delim;
		split(const char* s, int len, char delim) {
			if (len == -1) len = strlen(s);
			this->s = s;
			this->s1 = s;
			this->end = s + len;
			this->delim = delim;
		}
		bool read() {
			if (s == nullptr) return false;
			s = (const char*) memchr(s, delim, end - s);
			if (s == nullptr) {
				value= {s1, int(end - s1)};
				return true;
			}
			value= {s1, int(s - s1)};
			s1 = ++s;
			return true;
		}
	};
}


#endif /* SPLIT_H_ */
