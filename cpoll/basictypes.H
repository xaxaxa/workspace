/*
 * basictypes.H
 *
 *  Created on: Apr 26, 2013
 *      Author: xaxaxa
 */

#ifndef BASICTYPES_H_
#define BASICTYPES_H_
#include <string>
#include <rgc.H>
namespace CP
{
	typedef int32_t HANDLE; //file descriptor or handle
	typedef Delegate<void(int32_t)> Callback;
	typedef uint8_t event_t;
	struct String
	{
		char* d;
		int len;
		String() :
				len(0) {
		}
		String(char* data, int length) :
				d(data), len(length) {
		}
		String(char* data) :
				d(data), len(strlen(data)) {
		}
		String(const char* data, int length) :
				d(const_cast<char*>(data)), len(length) {
		}
		String(const char* data) :
				d(const_cast<char*>(data)), len(strlen(data)) {
		}
		int compare(const String& other) const {
			if (len < other.len) return -1;
			else if (len > other.len) return 1;
			else if (len == 0) return 0;
			else return memcmp(d, other.d, len);
		}
		inline bool operator<(const String& other) const {
			return compare(other) < 0;
		}
		inline bool operator>(const String& other) const {
			return compare(other) > 0;
		}
		inline bool operator<=(const String& other) const {
			return compare(other) <= 0;
		}
		inline bool operator>=(const String& other) const {
			return compare(other) >= 0;
		}
		inline bool operator==(const String& other) const {
			return compare(other) == 0;
		}
		inline bool operator==(std::nullptr_t other) const {
			return len == 0;
		}
		inline char* data() const {
			return d;
		}
		inline int length() const {
			return len;
		}
		inline std::string toSTDString() const {
			return std::string(d, len);
		}
	};
	template<class ... T>
	String concat(RGC::Allocator& alloc, T ... s...) {
		int l=sizeof...(s);
		String tmp[l];
		tmp= {s...};
		int size=0;
		for(int i=0;i<l;i++) size+=tmp[i].len;
		String r {alloc.alloc(size),0};
		for(int i=0;i<l;i++) {
			memcpy(r.d+r.len,tmp[i].d,tmp[i].len);
			r.len+=tmp[i].len;
		}
		return r;
	}
#define __CPOLL_DO_ATOI(func,str) char tmp[str.len + 1];\
														memcpy(tmp, str.d, str.len);\
														tmp[str.len] = 0;\
														return func(tmp);
	static int atoi(String s) {
		__CPOLL_DO_ATOI(::atoi, s);
	}
	static long atol(String s) {
		__CPOLL_DO_ATOI(::atol, s);
	}
	static long long atoll(String s) {
		__CPOLL_DO_ATOI(::atoll, s);
	}
	/*
	 static long long atoq(String s) {
	 __CPOLL_DO_ATOI(::atoq, s);
	 }*/
	class MemoryBuffer: virtual public RGC::Object
	{
	public:
		virtual uint8_t* data() const=0;
		virtual int length() const=0;
		CP::String String() {
			return {(char*)data(),length()};
		}
	};
}

#endif /* BASICTYPES_H_ */
