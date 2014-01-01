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
 * basictypes.H
 *
 *  Created on: Apr 26, 2013
 *      Author: xaxaxa
 */

#ifndef BASICTYPES_H_
#define BASICTYPES_H_
#include <string>
#include <stdexcept>
#include <rgc.H>
#include <delegate.H>
#include <string.h>

#ifndef DEPRECATED
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif
#endif

using namespace std;
namespace CP
{
	typedef int32_t HANDLE; //file descriptor or handle
	typedef Delegate<void(int32_t)> Callback;
	typedef uint8_t event_t;
	struct String;
	void * memrmem(void *s, size_t slen, void *t, size_t tlen);
	class MemoryBuffer: virtual public RGC::Object
	{
	public:
		virtual uint8_t* data() const=0;
		virtual int length() const=0;
		inline CP::String String();
	};
	struct String
	{
		char* d;
		int len;
		String() :
				d(nullptr), len(0) {
		}
		String(std::nullptr_t) :
				d(nullptr), len(0) {
		}
		String(char* data, int length) :
				d(data), len(length) {
		}
		String(char* data) :
				d(data), len(data == nullptr ? 0 : strlen(data)) {
		}
		String(const char* data, int length) :
				d(const_cast<char*>(data)), len(length) {
		}
		String(const void* data, int length) :
				d((char*) (data)), len(length) {
		}
		String(const char* data) :
				d(const_cast<char*>(data)), len(strlen(data)) {
		}
		String(std::string s) :
				d((char*) s.data()), len(s.length()) {
		}
		String(MemoryBuffer& mb) :
				d((char*) mb.data()), len(mb.length()) {
		}
		int compare(const String& other) const {
			if (len < other.len)
				return -1;
			else if (len > other.len)
				return 1;
			else if (len == 0)
				return 0;
			else return memcmp(d, other.d, len);
		}
		int alphabeticalCompare(const String& other) const {
			int l = len;
			if (other.len < l) l = other.len;
			for (int i = 0; i < l; i++) {
				char c1 = d[i], c2 = other.d[i];
				if (c1 < c2) return -1;
				if (c1 > c2) return 1;
			}
			if (len < other.len) return -1;
			else if (len > other.len) return 1;
			else return 0;
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
		inline String subString(int i, int l) {
			if (i < 0 || i + l > this->len || l < 0)
				throw range_error("String::subString() out of range");
			return String(d + i, l);
		}
		inline String subString(int i) {
			return subString(i, len - i);
		}
		inline void clip(int index, int length) {
			if (index < 0 || index + length > this->len || length < 0)
				throw range_error("String::clip() out of range");
			d += index;
			len = length;
		}
		inline void clip(int index) {
			if (index < 0 || index > this->len) throw range_error("String::clip() out of range");
			d += index;
			len -= index;
		}
		inline int indexOf(String s, int i = 0) {
			void* tmp = memmem(d + i, len - i, s.d, s.len);
			return tmp == NULL ? -1 : (((char*) tmp) - d);
		}
		inline int indexOf(char c, int i = 0) {
			void* tmp = memchr(d + i, c, len - i);
			return tmp == NULL ? -1 : (((char*) tmp) - d);
		}
		inline int lastIndexOf(String s, int i = -1) {
			if (i < 0) i = len - 1;
			void* tmp = memrmem(d, i + 1, s.d, s.len);
			return tmp == NULL ? -1 : (((char*) tmp) - d);
		}
		inline int lastIndexOf(char c, int i = -1) {
			if (i < 0) i = len - 1;
			void* tmp = memrchr(d, c, i + 1);
			return tmp == NULL ? -1 : (((char*) tmp) - d);
		}
		inline char& operator[](int i) {
			return d[i];
		}
		String copy() {
			String tmp;
			tmp.d = (char*) malloc((tmp.len = len));
			memcpy(tmp.d, d, len);
			return tmp;
		}
		void dealloc() {
			free(d);
			d = NULL;
			len = 0;
		}
	};
	template<class ... T>
	String concat(RGC::Allocator& alloc, T ... s...) {
		int l=sizeof...(s);
		String tmp[sizeof...(s)] {s...};
		int size=0;
		for(int i=0;i<l;i++) size+=tmp[i].len;
		String r {(char*)alloc.alloc(size),0};
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
	static double atof(String s) {
		__CPOLL_DO_ATOI(::atof, s);
	}

	/*
	 static long long atoq(String s) {
	 __CPOLL_DO_ATOI(::atoq, s);
	 }*/
	CP::String MemoryBuffer::String() {
		return {(char*)data(),length()};
	}
	struct Undefined
	{

	};
	static Undefined undefined;
	template<class T> class Optional
	{
	public:
		T value;
		bool exists;
		T& operator()() {
			return value;
		}
		T& operator*() {
			return value;
		}
		T* operator->() {
			return &value;
		}
		operator bool() {
			return exists;
		}
		bool operator==(Undefined) {
			return !exists;
		}
		Optional<T>& operator=(const T& other) {
			value = other;
			return *this;
		}
		Optional<T>& operator=(const Optional<T>& other) {
			if ((exists = other.exists)) value = other.value;
			return *this;
		}
		Optional<T>& operator=(Undefined) {
			exists = false;
			return *this;
		}
		Optional(const T& other) :
				value(other) {
			exists = true;
		}
		Optional(const Optional<T>& other) :
				value(other.value), exists(other.exists) {
		}
		struct _placeholder1
		{
		};
		template<class ... A> Optional(_placeholder1, A&&... a) :
				value(std::forward<A>(a)...), exists(true) {

		}
		template<class ... A>
		static Optional<T> create(A&&... a) {
			return Optional<T>(_placeholder1(), std::forward<A>(a)...);
		}
		Optional() :
				exists(false) {
		}
		Optional(const Undefined& u) :
				exists(false) {
		}
	};
	template<class T>
	struct Future
	{
		Delegate<void(T, exception*)>* cbAddr;
		Future(Delegate<void(T, exception*)>* cbAddr) :
				cbAddr(cbAddr) {
		}
	};
	template<class T> class AsyncValue
	{
	public:
		T value;
		Delegate<void(T, exception*)>* cbAddr;
		bool async;
		T& operator()() {
			return value;
		}
		T& operator*() {
			return value;
		}
		T* operator->() {
			return &value;
		}
		operator bool() {
			return !async;
		}
		bool operator==(Undefined) {
			return async;
		}
		AsyncValue<T>& operator=(const T& other) {
			value = other;
			return *this;
		}
		AsyncValue<T>& operator=(const AsyncValue<T>& other) {
			if (!(async = other.async))
				value = other.value;
			else cbAddr = other.cbAddr;
			return *this;
		}
		AsyncValue<T>& operator=(Undefined) {
			async = true;
			return *this;
		}
		AsyncValue(const T& other) :
				value(other) {
			async = false;
		}
		AsyncValue(const AsyncValue<T>& other) :
				value(other.value), cbAddr(other.cbAddr), async(other.async) {
		}
		AsyncValue(Future<T> f) :
				cbAddr(f.cbAddr), async(true) {
		}
		struct _placeholder1
		{
		};
		template<class ... A> AsyncValue(_placeholder1, A&&... a) :
				value(std::forward<A>(a)...), async(false) {

		}
		template<class ... A>
		static AsyncValue<T> create(A&&... a) {
			return AsyncValue<T>(_placeholder1(), std::forward<A>(a)...);
		}
		AsyncValue() :
				async(true) {
		}
		AsyncValue(const Undefined& u) :
				async(true) {
		}
		void wait(Delegate<void(T, exception*)> cb) {
			*cbAddr = cb;
		}
	};
}

#endif /* BASICTYPES_H_ */
