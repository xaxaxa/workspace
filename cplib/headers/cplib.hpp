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
#ifdef __debug_print123
#define dbgprint(msg) cout << msg << endl
#else
#define dbgprint(msg)
#endif

#ifndef __CPLIB_H
#define __CPLIB_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
//#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <execinfo.h>
#include <typeinfo>
#include <boost/shared_array.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <map>
#include <list>
#include <set>
#include <errno.h>
#include <queue>
#include <sys/eventfd.h>
#include "lock.hpp"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <ucontext.h>
#include <cxxabi.h>
#include <functional>
#include <utility>
#ifdef cplib_glib_wrappers
#include <glibmm.h>
#include <giomm.h>
#endif
//#define FUNCTION(RETVAL,...) struct{RETVAL(*f)(void*,__VA_ARGS__);void* obj;}
//RETVAL(*)(__VA_ARGS__)

#define FUNCTION_ISNULL(fn) ((fn).f==NULL)
/*#define FUNCTION_DECLARE(NAME,RETVAL,...) \
	struct NAME;struct NAME{\
		RETVAL(*f)(void*,__VA_ARGS__);\
		void* obj;\
		inline NAME(RETVAL(*f)(void*,__VA_ARGS__),void* obj)\
		{this->f=f;\
		 this->obj=obj;}\
		template<class FGSAFJGFJSA>inline NAME(RETVAL(FGSAFJGFJSA::*f)(__VA_ARGS__),void* obj)\
		{this->f=reinterpret_cast<RETVAL(*)(void*,__VA_ARGS__)>((void*)f);\
		this->obj=obj;}\
		inline NAME(RETVAL(*f)(void*,__VA_ARGS__))\
		{this->f=reinterpret_cast<RETVAL(*)(void*,__VA_ARGS__)>((void*)f);\
		this->obj=NULL;}\
		inline NAME(){f=NULL;}\
		static NAME null;\
		bool operator==(const NAME& x) const\
		{return (x.f==this->f) && ((x.f==NULL) || (x.obj==this->obj));}\
		bool operator!=(const NAME& x) const\
		{return !((*this)==x);}\
		bool operator<(const NAME& X) const\
		{if(f==X.f)return (f!=NULL)&&(obj<X.obj);return f>X.f;}\
		bool operator>(const NAME& X) const\
		{if(f==X.f)return (f!=NULL)&&(obj>X.obj);return f>X.f;}\
		bool operator<=(const NAME& X) const\
		{return !(*this>X);}\
		bool operator>=(const NAME& X) const\
		{return !(*this<X);}\
	}*/
#define FUNCTION_DECLARE(NAME,RETVAL,...) \
	struct NAME;struct NAME{\
		std::function<RETVAL(void*,__VA_ARGS__)> f;\
		void* obj;int __id;\
		template<class FGSAFJGFJSA>inline NAME(RETVAL(FGSAFJGFJSA::*f)(__VA_ARGS__),void* obj)\
		{__id=rand();this->f=reinterpret_cast<RETVAL(*)(void*,__VA_ARGS__)>((void*)f);\
		this->obj=obj;}\
		inline NAME(RETVAL(*f)(void*,__VA_ARGS__))\
		{__id=rand();this->f=reinterpret_cast<RETVAL(*)(void*,__VA_ARGS__)>((void*)f);\
		this->obj=NULL;}\
		inline NAME(const std::function<RETVAL(void*,__VA_ARGS__)>& func)\
		{__id=rand();f=func;\
		this->obj=NULL;}\
		inline NAME(const std::function<RETVAL(void*,__VA_ARGS__)>& func, void* obj)\
		{__id=rand();f=func;\
		this->obj=obj;}\
		inline NAME(){}\
		static NAME null;\
		bool operator<(const NAME& X) const\
		{return __id>X.__id;}\
	}
/*#define FUNCTION_DECLARE(NAME,RETVAL,...) \
	struct NAME;struct NAME{\
		std::function<RETVAL(__VA_ARGS__)> f;\
		inline NAME(RETVAL(*f)(void*,__VA_ARGS__),void* obj)\
		{this->f=std::bind(std::function<RETVAL(void*,__VA_ARGS__)>(f), obj);}\
		template<class FGSAFJGFJSA>inline NAME(RETVAL(FGSAFJGFJSA::*f)(__VA_ARGS__),void* obj)\
		{this->f=std::bind(std::function<RETVAL(void*,__VA_ARGS__)>(reinterpret_cast<RETVAL(*)(void*,__VA_ARGS__)>((void*)f)), obj);\
		}\
		inline NAME(RETVAL(*f)(void*,__VA_ARGS__))\
		{this->f=std::bind(std::function<RETVAL(void*,__VA_ARGS__)>(reinterpret_cast<RETVAL(*)(void*,__VA_ARGS__)>((void*)f)), (void*)NULL);\
		}\
		inline NAME(){}\
	}*/
//#define FUNCTION_EXPORT(CLASS,NAME,FNAME,RETVAL,...) static RETVAL NAME(void* obj,__VA_ARGS__){return ((CLASS*)obj)->FNAME(__VA_ARGS__);}
#define FUNCTION_DECLWRAPPER(NAME,RETVAL,...) static inline RETVAL NAME(void* obj,__VA_ARGS__)
//#define FUNCTION_GET(TYPE,WRAPPER_NAME) {TYPE __func;__func.f=WRAPPER_NAME;__func.obj=this;}
//#define FUNCTION_GETSTATIC(TYPE,WRAPPER_NAME) (TYPE __func,__func.f=WRAPPER_NAME,__func.obj=NULL)
//#define FUNCTION_GET1(TYPE,OBJ,WRAPPER_NAME) {TYPE __func;__func.f=OBJ->WRAPPER_NAME;__func.obj=OBJ;}
//#define FUNCTION_CALL(FUNC,...) FUNCTION_ISNULL(FUNC)?throw Exception("attempting to call null function"):(FUNC).f((FUNC).obj,__VA_ARGS__)
#define FUNCTION_CALL(FUNC,...) (FUNC).f((FUNC).obj,__VA_ARGS__)
#define DELEGATE(RETVAL,NAME,...) FUNCTION_DECLARE(NAME,RETVAL,__VA_ARGS__)
#define CALL FUNCTION_CALL
/*#define FUNCTION_ISNULL(FUNC) (FUNC==NULL)
 #define FUNCTION_DECLARE(NAME,RETVAL,...) typedef boost::function<RETVAL (__VA_ARGS__)> NAME;
 #define FUNCTION_CALL(FUNC,...) FUNC(__VA_ARGS__)
 #define FUNCTION_DECLWRAPPER(NAME,RETVAL,...) static inline void NAME(void* obj,__VA_ARGS__)
 #define FUNCTION_BIND(FUNC,THIS) bind(FUNC,THIS,_1)
 */

#ifndef WARNLEVEL
#define WARNLEVEL 10
#endif
#define WARN(LEVEL,MSG) if(LEVEL<=WARNLEVEL){if(LEVEL<=1)cerr << "\x1B[41;1;33m"; else if(LEVEL<=2)cerr << "\x1B[1;1;1m"; cerr << MSG << "\x1B[0;0;0m" << endl;}

using namespace std;
namespace xaxaxa
{
//typedef void* Function;
	typedef volatile int atomic_t;
#define atomic_read(v)                  v
#define atomic_set(v,i)                 v = i
#define atomic_add(i,v)                 v += i
#define atomic_sub(i,v)                 v -= i
#define atomic_sub_and_test(i,v)        (v -= i) == 0
#define atomic_inc(v)                   v++
#define atomic_dec(v)                   v--
#define atomic_dec_and_test(v)          --v == 0
#define atomic_inc_and_test(v)          ++v == 0
#define atomic_add_negative(i,v)        (v += i) < 0
	template<class SIGNATURE> struct Delegate;
	template<class RET, class ... ARGS> struct Delegate<RET(ARGS...)>
	{
		virtual RET operator()(ARGS...)=0;
	};

//for the following 3 function wrapper classes, if they are initialized
//with the default constructor(without any parameters), calling the object
//results in undefined behavior
	template<class SIGNATURE> struct StaticFunction;
	template<class RET, class ... ARGS> struct StaticFunction<RET(ARGS...)> : public Delegate<
			RET(ARGS...)>
	{
		RET (*func)(ARGS...);
		virtual RET operator()(ARGS&& ... a)
		{
			return func(std::forward<ARGS...>(a...));
		}
		StaticFunction(RET (*func)(ARGS...)) :
		func(func)
		{
		}
		StaticFunction()
		{
		}
	};

	template<class SIGNATURE> struct MemberFunction;
	template<class RET, class ... ARGS> struct MemberFunction<RET(ARGS...)> : public Delegate<
			RET(ARGS...)>
	{
		RET (*func)(void*, ARGS...);
		void* this_ptr;
		virtual RET operator()(ARGS&& ... a)
		{
			return func(this_ptr, std::forward<ARGS...>(a...));
		}
		template<class c> MemberFunction(RET (c::*func)(ARGS...), c* obj)
		//:func((RET (*)(void*, ARGS...))func), this_ptr(obj)
				{
					this->func = (RET (*)(void*, ARGS...))func;this->this_ptr=obj;
				}
				MemberFunction()
				{}
			};

		//uncomment to allow hybrid functions to distinguish between (a member
		//function pointer with a NULL thisptr) and (a static function).
		//this adds overhead.
		//#define XAXAXA_FUNCTION_ALLOW_NULL_THISPTR

	template<class SIGNATURE> struct Function;
	template<class RET, class ... ARGS> struct Function<RET(ARGS...)> : public Delegate<RET(ARGS...)>
	{
		//RET (*func)(void*, ARGS...);
		void* func;
		void* this_ptr;
#ifdef XAXAXA_FUNCTION_ALLOW_NULL_THISPTR
		bool is_mem_func;
#endif
		virtual RET operator()(ARGS&& ... a)
		{
#ifdef XAXAXA_FUNCTION_ALLOW_NULL_THISPTR
				if(is_mem_func)
				return ((RET (*)(void*, ARGS...))func)(this_ptr, a...);
				else
				return ((RET (*)(ARGS...))func)(a...);
#endif
				if (this_ptr != NULL)
				return ((RET (*)(void*, ARGS...)) func)(this_ptr, std::forward<ARGS...>(a...));
			else
				return ((RET (*)(ARGS...)) func)(std::forward<ARGS...>(a...));
		}
		template<class c> Function(RET (c::*func)(ARGS...), c* obj) :
		func((void*) func), this_ptr(obj)
		{
#ifdef XAXAXA_FUNCTION_ALLOW_NULL_THISPTR
			this->is_mem_func = true;
#endif
		}
		Function(RET (*func)(ARGS...)) :
		func((void*) func)
		{
#ifdef XAXAXA_FUNCTION_ALLOW_NULL_THISPTR
			this->is_mem_func = false;
#else
			this->this_ptr = NULL;
#endif
		}
		Function()
		{
		}
	};
	template<class SIGNATURE> struct FunctionWrapper
	;
	template<class RET, class ... ARGS> struct FunctionWrapper<RET(ARGS...)>
	{
		function<RET(ARGS...)> func;
		typedef RET (*func_t)(void*, ARGS...);
		FunctionWrapper(const function<RET(ARGS...)>& f) :
				func(f)
		{
		}
		RET operator()(ARGS&& ... a)
		{
			return func(std::forward<ARGS...>(a...));
		}
		func_t Get()
		{
			return (func_t) &FunctionWrapper<RET(ARGS...)>::operator();
		}
	};
	extern int objs;
	class Object
	{
	public:

		atomic_t RefCount; //reference counter
		Object()
		{
			RefCount = 1;
#ifdef __debug_obj123
			objs++;
			cout<<"construct<"<<typeid(*this).name()<<">; "<<objs<<" objects total"<<endl;
#endif
		}
		Object(const Object& other)
		{
			RefCount = 1;
		}
		virtual ~Object()
		{
#ifdef __debug_obj123
			objs--;
			cout<<"destruct<"<<typeid(*this).name()<<">; "<<objs<<" objects total"<<endl;
#endif
		}
		inline void RefCount_inc()
		{
			atomic_inc(RefCount);
#ifdef __debug_obj123
			cout<<"retain<"<<typeid(this).name()<<"> = "<<RefCount << "; "<<objs<<" objects total"<<endl;
#endif
		}
		inline void RefCount_dec()
		{
			atomic_dec(RefCount);
#ifdef __debug_obj123
			cout<<"release<"<<typeid(this).name()<<"> = "<<RefCount << "; "<<objs<<" objects total"<<endl;
#endif
			dbgprint("refcount=" << RefCount);
			if (RefCount <= 0) {
				delete this;
				//cout << o;
			}
		}
		inline void Release()
		{
			RefCount_dec();
		}
		virtual string ToStr() const
		{
			return string(typeid(*this).name());
		}
		Object& operator=(const Object& other)
		{
			//RefCount = 1;
			return *this;
		}
	}
	;
	template<class T> struct objref
	;
	template<class T> struct Property
	{
		T* obj;
		inline Property() :
				obj(NULL)
		{
		}
		inline Property(const objref<T>& other);
		inline Property(T* obj) :
				obj(obj)
		{
			if (obj != NULL) obj->RefCount_inc();
		}
		inline Property(T& obj) :
				obj(&obj)
		{
			if (&obj != NULL) obj.RefCount_inc();
		}
		inline ~Property()
		{
			if (this->obj != NULL) this->obj->RefCount_dec();
		}
		Property(const Property<T>& other) :
				obj(other.obj)
		{
			if (obj != NULL) obj->RefCount_inc();
		}
		T* operator=(T* obj)
		{
			if (this->obj != NULL) this->obj->RefCount_dec();
			this->obj = obj;
			if (obj != NULL) obj->RefCount_inc();
			return obj;
		}
		T* operator=(T& obj)
		{
			return operator=(*obj);
		}
		Property& operator=(const Property& other)
		{
			if (this->obj != NULL) this->obj->RefCount_dec();
			this->obj = other.obj;
			if (obj != NULL) obj->RefCount_inc();
			return *this;
		}
		inline T* operator()() const
		{
			return obj;
		}
		inline T& operator*() const
		{
			return *obj;
		}
		inline T* operator->() const
		{
			return obj;
		}
		inline T* get() const
		{
			return obj;
		}
	}
	;
	template<class T> struct objref
	{
		T* obj;
		template<class ... ARGS> inline objref(ARGS&& ... args)
		:obj(new T(std::forward<ARGS>(args)...))
		{
		}
		objref(const Property<T>& prop):obj(prop.obj)
		{
			obj->RefCount_inc();
		}
		~objref()
		{
			obj->RefCount_dec();
		}
		inline T* operator()() const
		{
			return obj;
		}
		inline T* operator->() const
		{
			return obj;
		}
		inline T& operator*() const
		{
			return *obj;
		}
		inline T* get() const
		{
			return obj;
		}
	};
	template<class T> Property<T>::Property(const objref<T>& other) :
			obj(other.obj)
	{
		if (obj != NULL) obj->RefCount_inc();
	}

//creates a new instance, but disowns it
	template<class T, class ... ARGS> T* newobj(ARGS&&... args)
	{
		T* tmp = new T(std::forward<ARGS>(args)...);
		tmp->RefCount = 0;
		return tmp;
	}
//*/
	/*template<class T> T* operator=(T*& a, const Property<T>& b)
	 {
	 a = b.obj;
	 return a;
	 }*/
	class Exception: public Object, public exception
	{
	public:
		int Code;
		std::string Message;
		virtual const char* what() const throw ()
		{
			return Message.c_str();
		}
		virtual void init(int code, std::string message)
		{
			this->Code = code;
			this->Message = message;
			//#if DEBUG
			//if(message==NULL)
			//	cout << "Exception: code=" << code;
			//else
			cout << "Exception: " << message << "; code=" << code << endl;
			void *array[50];
			size_t size;

			// get void*'s for all entries on the stack
			size = backtrace(array, 50);

			// print out all the frames to stderr
			//fprintf(stderr, "Error: signal %d:\n", sig);
			backtrace_symbols_fd(array, size, 2);
			//#endif
		}
		inline Exception(int code)
		{
			init(code, strerror(code));
			//delete err;
		}
		inline Exception(std::string message)
		{
			init(-1, message);
		}
		inline Exception(int code, std::string message)
		{
			init(code, message);
		}
		inline Exception()
		{
			Code = 0;
		}
		virtual ~Exception() throw ()
		{
		}
	}
	;
	class OutOfRangeException: public Exception
	{
	public:
		inline OutOfRangeException() :
				Exception()
		{
			//this();
		}
		inline OutOfRangeException(std::string message) :
				Exception(message)
		{

		}
	}
	;
	class NotSupportedException: public Exception
	{
	public:
		inline NotSupportedException() :
				Exception()
		{
			//this();
		}
		inline NotSupportedException(std::string message) :
				Exception(message)
		{

		}
	}
	;
	struct STRING
	{
		char* c;
		int length;
		STRING(char* c, int length)
		{
			this->c = c;
			this->length = length;
		}
		STRING(char* c)
		{
			//STRING(c,strlen(c));
			this->c = c;
			length = strlen(c);
		}
		STRING(const char* c)
		{
			this->c = (char*) c;
			length = strlen(c);
		}
		STRING()
		{
		}
	}
	;
#ifdef __debug_print123
	void __Buffer_bytes_inc(int i);
	void __Buffer_bytes_dec(int i);
	int __Buffer_bytes_get();
#endif
	/*struct Buffer
	 {
	 public:
	 void* Data;
	 private:
	 void* __location;
	 public:
	 int Length;
	 //bool destruct;
	 struct BufferInfo
	 {
	 int RetainCount;
	 };
	 inline Buffer()
	 {
	 Data = NULL;
	 }
	 inline Buffer(void* buf, int length, Buffer* src = NULL)
	 {
	 this->Data = buf;
	 this->Length = length;
	 this->__location = (src == NULL ? NULL : src->__location);
	 //this->destruct=destruct;
	 //Retain();
	 }
	 inline Buffer(int length)
	 {
	 if (length <= 0)
	 {
	 this->__location = NULL;
	 return;
	 }
	 this->__location = malloc(sizeof(BufferInfo) + length);
	 BufferInfo *inf = (BufferInfo*) __location;
	 inf->RetainCount = 1;
	 this->Data = (void*) (((__uint8_t *) this->__location) + sizeof(BufferInfo));
	 this->Length = length;
	 //this->destruct=destruct;
	 #ifdef __debug_print123
	 __Buffer_bytes_inc(length);
	 dbgprint(length << " bytes allocated(total " << __Buffer_bytes_get() << ")");
	 #endif
	 }
	 inline void Free()
	 {
	 if (__location == NULL)
	 {
	 if (Data != NULL)
	 {
	 free(Data);
	 Data = NULL;
	 #ifdef __debug_print123
	 __Buffer_bytes_dec(length);
	 dbgprint(length << " bytes freed(total " << __Buffer_bytes_get() << ")");
	 #endif
	 }
	 }
	 else
	 {
	 free(__location);
	 __location = NULL;
	 Data = NULL;
	 #ifdef __debug_print123
	 __Buffer_bytes_dec(length);
	 dbgprint(length << " bytes freed(total " << __Buffer_bytes_get() << ")");
	 #endif
	 }
	 }

	 inline Buffer& Retain()
	 {
	 if (__location != NULL && Data != NULL)
	 {
	 BufferInfo *inf = (BufferInfo*) __location;
	 inf->RetainCount++;
	 dbgprint("Buffer::Retain()");
	 }
	 return *this;
	 }
	 inline Buffer SubBuffer(int index, int length, bool retain = false)
	 {
	 if (index < 0 || length > this->Length)
	 throw Exception("SubBuffer() out of range");
	 //Buffer b((void*)((char*)buf+index),length,this,false,retain);
	 Buffer b;
	 b.Data = (void*) ((char*) Data + index);
	 b.Length = length;
	 b.__location = this->__location;
	 if (retain)
	 this->Retain();
	 return b;
	 }
	 inline Buffer SubBuffer(int index)
	 {
	 return SubBuffer(index, Length - index);
	 }
	 inline void Release()
	 {
	 if (Length <= 0 || Data == NULL
	 )
	 return;
	 if (__location != NULL)
	 {
	 BufferInfo *inf = (BufferInfo*) __location;
	 inf->RetainCount--;
	 dbgprint("Buffer::Release()");
	 if (inf->RetainCount == 0)
	 Free();
	 }
	 }
	 };*/

	typedef __uint8_t Byte;
	typedef __int8_t SByte;
	typedef __int16_t Short;
	typedef __uint16_t UShort;
	typedef int Int;
	typedef unsigned int UInt;
	typedef long long int Long;
	typedef unsigned long long int ULong;
	typedef float Float;
	typedef double Double;
	typedef long double Decimal;
	template<class T> inline T modulus(T number, T modulus)
	{
		T result = number % modulus;
		if (result < 0) result += modulus;
		return result;
	}
//typedef boost::shared_ptr shared_ptr;
//does NOT do reference counting; this is a simple struct to pass around buffers
//to functions that do not store them; this avoids the overhead of Buffer
	class Buffer
	;
	struct BufferRef
	{
		Byte* Data;
		Int Length;
		inline BufferRef() :
				Length(0)
		{
		}
		inline BufferRef(const Buffer& b);
		inline BufferRef(Byte* Data, Int Length) :
				Data(Data), Length(Length)
		{
		}
		inline BufferRef(void* Data, Int Length) :
				Data((Byte*) Data), Length(Length)
		{
		}
		inline BufferRef(char* buf)
		{
			this->Data = (Byte*) buf;
			this->Length = strlen(buf);
		}
		inline BufferRef(const char* buf)
		{
			this->Data = (Byte*) buf;
			this->Length = strlen(buf);
		}
		/*BufferRef(const Buffer& b) :
		 Data(b.Data), Length(b.Length)
		 {
		 }*/
		inline BufferRef SubBuffer(int index, int length) const
		{
			if (index < 0 || index + length > this->Length) throw Exception(
					"SubBuffer() out of range");
			return BufferRef
			{ Data + index, length };
		}
		inline BufferRef SubBuffer(int index) const
		{
			return SubBuffer(index, Length - index);
		}
		inline void Clip(int index, int length)
		{
			if (index < 0 || index + length > this->Length) throw Exception(
					"BufferRef::Clip() out of range");
			Data += index;
			Length = length;
		}
		inline void Clip(int index)
		{
			//Clip(index, Length - index);
			if (index < 0 || index > this->Length) throw Exception(
					"BufferRef::Clip() out of range");
			Data += index;
			Length -= index;
		}
	}
	;
	class Buffer: public BufferRef
	{
	public:
		//boost::shared_array<Byte> pbuf;
		typedef int refc_t;
		refc_t* pbuf;
		//Byte* Data;
		//bool IsRaw;
		//Int Length;
		inline void __inc()
		{
			++*pbuf;
		}
		inline void __dec_autofree()
		{
			if (--*pbuf <= 0) delete[] (Byte*) pbuf;
		}
		inline Buffer() :
				pbuf(NULL)
		{
		}
		Buffer(void* buf, int length) :
				pbuf(NULL)
		{
			this->Data = (Byte*) buf;
			this->Length = length;
		}
		Buffer(const char* buf, int length) :
				pbuf(NULL)
		{
			this->Data = (Byte*) buf;
			this->Length = length;
		}
		Buffer(char* buf) :
				pbuf(NULL)
		{
			this->Data = (Byte*) buf;
			this->Length = strlen(buf);
		}
		Buffer(const char* buf) :
				pbuf(NULL)
		{
			this->Data = (Byte*) buf;
			this->Length = strlen(buf);
		}
		/*Buffer(Byte* buf, int length)
		 {
		 this->Data = (Byte*) buf;
		 this->Length = length;
		 this->IsRaw = true;
		 }*/
		Buffer(void* buf, int length, refc_t* orig) :
				pbuf(orig)
		{
			this->Data = (Byte*) buf;
			this->Length = length;
			__inc();
		}
		inline void Release()
		{
			if (pbuf == NULL) throw Exception(
					"Attempting to release a buffer initialized with a raw pointer. This can be dangerous.");
			__dec_autofree();
			Data = NULL;
			Length = 0;
			pbuf = NULL;
		}
		Buffer(int length)
		{
			this->Length = length;
			if (length <= 0) {
				this->Data = NULL;
				this->pbuf = NULL;
				return;
			}
			this->pbuf = (refc_t*) new Byte[length + sizeof(refc_t)];
			*this->pbuf = 1;
			this->Data = (Byte*) (this->pbuf + 1);
			//this->Data = new Byte[length];
			//this->pbuf = boost::shared_array<Byte>(this->Data);
		}
		Buffer(const Buffer& b) :
				BufferRef(b), pbuf(b.pbuf)
		{
			if (b.pbuf != NULL) __inc();
		}
		Buffer& operator=(const Buffer& b)
		{
			BufferRef::operator=(b);
			if (pbuf != NULL) __dec_autofree();
			if ((pbuf = b.pbuf) != NULL) __inc();
			return *this;
		}
		inline ~Buffer()
		{
			if (pbuf != NULL) __dec_autofree();
		}
		inline Buffer SubBuffer(int index, int length) const
		{
			if (index < 0 || index + length > this->Length) throw Exception(
					"SubBuffer() out of range");
			if (pbuf == NULL) return Buffer(Data + index, length);
			else return Buffer(Data + index, length, pbuf);
		}
		inline Buffer SubBuffer(int index) const
		{
			return SubBuffer(index, Length - index);
		}
		inline void Clip(int index, int length)
		{
			if (index < 0 || index + length > this->Length) throw Exception(
					"Buffer::Clip() out of range");
			Data += index;
			Length = length;
		}
		inline void Clip(int index)
		{
			//Clip(index, Length - index);
			if (index < 0 || index > this->Length) throw Exception("Buffer::Clip() out of range");
			Data += index;
			Length -= index;
		}

	};
	BufferRef::BufferRef(const Buffer& b) :
			Data(b.Data), Length(b.Length)
	{

	}
	template<class T> class __event: public Object
	{
	public:
		set<T> handlers;
		typedef typename set<T>::iterator iter;
		/*inline void* newiter()
		 {
		 return new iter(handlers.begin());
		 }
		 inline void deleteiter(void* it)
		 {
		 iter* tmp=(iter*)it;
		 delete tmp;
		 }
		 inline bool isiterend(void* it)
		 {
		 iter& tmp=*((iter*)it);
		 return tmp==handlers.end();
		 }
		 inline void incrementiter(void* it)
		 {
		 iter& tmp=*((iter*)it);
		 tmp++;
		 }
		 inline T getitervalue(void* it)
		 {
		 iter& tmp=*((iter*)it);
		 return (*tmp).first;
		 }*/
		inline void addhandler(T delegate)
		{
			handlers.insert(delegate);
		}
		inline void removehandler(T delegate)
		{
			handlers.erase(delegate);
		}
		inline void operator+=(T delegate)
		{
			addhandler(delegate);
		}
		inline void operator-=(T delegate)
		{
			removehandler(delegate);
		}
	};
#define EVENT(DEL) __event<DEL>

#define RAISEEVENT(NAME,...) \
		{\
		for(auto gjfdjdsghddjh=(NAME).handlers.begin();gjfdjdsghddjh!=(NAME).handlers.end();gjfdjdsghddjh++)\
		{CALL((*gjfdjdsghddjh),__VA_ARGS__);}}
#define ADDHANDLER(NAME,DEL) (NAME).addhandler(DEL)
#define REMOVEHANDLER(NAME,DEL) (NAME).removehandler(DEL)
	class Stream: public Object
	{
	public:
		enum class Cap
:		Byte
		{
			None=0,
			Read=1, Write=2, ReadWrite=Read|Write, Flush=4, Close=8,
			Seek=16, Length=32,
			All=0xFF
		};

		enum class SeekFrom
		{
			Nop=0,
			Begin=SEEK_SET,
			End=SEEK_END,
			Current=SEEK_CUR
		};
		virtual Int Read(const BufferRef& buf)=0;
		virtual void Write(const BufferRef& buf)=0;
		Int Fill(BufferRef buf)
		{
			Int br,br1=0;
			while((br=Read(buf))>0)
			{
				buf.Clip(br);
				br1+=br;
			}
			return br1;
		}
		virtual void Flush()=0;
		virtual void Close()=0;
		virtual void Seek(Long n, SeekFrom from=SeekFrom::Begin)
		{}
		virtual Long Position()
		{	return 0;}
		virtual Long Length(Long newlen=-1)
		{	return 0;} //set or get length
		virtual Cap Capabilities()
		{	return Cap::None;}
		virtual Int Skip(Long n)
		{
			if((Byte)Capabilities() & (Byte)Cap::Seek)
			{
				Seek(n,SeekFrom::Current);
				return n;
			}
			Buffer b(n>8192?8192:n);
			Int br;
			Int n2(n);
			while(n2>0 && (br=Read(b))>0)
			{
				n2-=br;
				if(b.Length>n2)b.Clip(0,n2);
			}
			return n-n2;
		}
		//bool AutoClose;
		//typedef boost::function<void (void*,Stream*)> Callback;
		FUNCTION_DECLARE(Callback,void,Stream*);
		int __tmp;
		virtual void BeginRead(const Buffer& buf, Callback cb)
		{
			__tmp = Read(buf);
			FUNCTION_CALL(cb, this);
		}
		virtual void BeginWrite(const Buffer& buf, Callback cb)
		{
			Write(buf);
			FUNCTION_CALL(cb, this);
		}
		virtual Int EndRead()
		{
			return __tmp;
		}
		virtual void EndWrite()
		{

		}
		struct
		{
			Buffer buf;
			Callback cb;
			Int br;
			bool in_progress;
		}r_state,w_state;

		void cb1(Stream* s)
		{
			try
			{
				Int tmp=EndRead();
				if(tmp>0)r_state.br+=tmp;
				if(tmp<=0 || tmp>=r_state.buf.Length)
				{
					r_state.in_progress=false;
					FUNCTION_CALL(r_state.cb, this);
					return;
				}
				r_state.buf.Clip(tmp);
				BeginRead(r_state.buf,Callback(&Stream::cb1,this));
			}
			catch(Exception& ex)
			{
				r_state.in_progress=false;
				FUNCTION_CALL(r_state.cb, this);
				return;
			}
		}
		virtual void BeginFill(const Buffer& buf, Callback cb)
		{
			if(r_state.in_progress)throw Exception("read operation already in progress");
			r_state.in_progress=true;
			r_state.buf=buf;
			r_state.br=0;
			r_state.cb=cb;
			BeginRead(buf,Callback(&Stream::cb1,this));
		}
		virtual Int EndFill()
		{
			return r_state.br;
		}
		Stream()
		{
			r_state.in_progress=w_state.in_progress=false;
		}
		virtual ~Stream()
		{
			//if(AutoClose)Close();
		}
	};
	inline Stream::Cap operator|(Stream::Cap c1, Stream::Cap c2)
	{
		return (Stream::Cap) ((Byte) c1 | (Byte) c2);
	}
	inline Stream::Cap operator&(Stream::Cap c1, Stream::Cap c2)
	{
		return (Stream::Cap) ((Byte) c1 & (Byte) c2);
	}
	inline Stream::Cap operator~(Stream::Cap c)
	{
		return (Stream::Cap) (~(Byte) c);
	}

	Long Splice(Stream& from, Stream& to, Long c);

	class StreamSource: public Object
	{
	public:
		virtual Stream& CreateStream()=0;
		virtual ~StreamSource()
		{
		}
	};
	class NullStream: public Stream
	{
		virtual Cap Capabilities()
		{
			return Cap::Read | Cap::Write | Cap::Close;
		}
		virtual Int Read(const BufferRef& buf)
		{
			return 0;
		}
		virtual void Write(const BufferRef& buf)
		{
		}
		virtual void Flush()
		{
		}
		virtual void Close()
		{
		}
	};
	class Waitable
	{ //represents something that needs to wait for events
	public:
		typedef uint32_t event_t;
		virtual ~Waitable()
		{
		}
		virtual int GetFileDesc()=0;
		virtual void Trigger(event_t events)=0;
	};
	class TextWriter: public Stream
	{

	};
	class StringBuilder: public Stream
	{
	public:
		StringBuilder(int initsize = 4096);
		StringBuilder(const Buffer& b);
		virtual ~StringBuilder();
		virtual Cap Capabilities()
		{
			return Cap::ReadWrite | Cap::Length | Cap::Seek;
		}
		//void *buf;
		Buffer buf;
		int Capacity;
		int length;
		int position;
		void Clear()
		{
			length = 0;
			position = 0;
		}
		inline void EnsureCapacity(int c)
		{
			if (Capacity >= c) return;
			int tmp = this->Capacity;
			while (tmp < c) {
				tmp *= 2;
			}
			/*Byte* new_arr = new Byte[tmp];
			 memcpy(new_arr, buf.Data, length);
			 buf.Data = new_arr;
			 buf.pbuf.reset(new_arr);
			 buf.Length = this->Capacity = tmp;*/
			Buffer tmpb(tmp);
			memcpy(tmpb.Data, buf.Data, length);
			buf = tmpb;
			this->Capacity = tmp;
		}
		void Append(const BufferRef& buf);
		void Append(STRING buf);
		inline void Append(const char* buf, int length)
		{
			this->EnsureCapacity(this->position + length);
			memcpy((char*) this->buf.Data + this->position, buf, length);
			this->position += length;
			if (this->position > this->length) this->length = this->position;
		}
		void Append(const char* buf);
		void Append(const StringBuilder* s);
		inline void Append(char c)
		{
			Append(&c, 1);
		}
		inline void Append(Int n)
		{
			//char c[log10(0xFFFFFFFF)+2];
			char c[12];
			int i = snprintf(c, sizeof(c), "%i", n);
			Append(c, i);
		}
		inline void Append(UInt n)
		{
			//char c[log10(0xFFFFFFFF)+2];
			char c[12];
			int i = snprintf(c, sizeof(c), "%i", n);
			Append(c, i);
		}
		inline void Append(Long n)
		{
			//char c[log10(0xFFFFFFFF)+2];
			char c[22];
			int i = snprintf(c, sizeof(c), "%lli", n);
			Append(c, i);
		}
		inline void Append(ULong n)
		{
			//char c[log10(0xFFFFFFFF)+2];
			char c[22];
			int i = snprintf(c, sizeof(c), "%lli", n);
			Append(c, i);
		}
		inline void Append(const string s)
		{
			Append(s.data(), s.length());
		}
		int Append(Stream& s, Int n)
		{
			if (n <= 0) return 0;
			EnsureCapacity(position + n);
			Int tmp = s.Read(buf.SubBuffer(position, n));
			position += tmp;
			if (position > length) length = position;
			return tmp;
		}
		/*inline void Append(Float n)
		 {
		 //char c[log10(0xFFFFFFFF)+2];
		 char c[22];
		 int i = snprintf(c, sizeof(c), "%f", n);
		 Append(c, i);
		 }*/
		int CompareTo(const BufferRef& buf);
		int CompareTo(const StringBuilder& sb);
		STRING ToString();
		Buffer ToBuffer();
		char* ToCString()
		{
			EnsureCapacity(length + 1);
			((char*) buf.Data)[length] = '\x00';
			return ((char*) buf.Data);
		}
		string ToSTDString()
		{
			return string(ToCString(), length);
		}
		virtual int Read(const BufferRef& buf);
		virtual void Write(const BufferRef& buf);
		virtual void Flush();
		virtual void Close();
		virtual void Seek(Long n, SeekFrom from = SeekFrom::Begin)
		{
			switch (from)
			{
				case SeekFrom::Begin:
					break;
				case SeekFrom::Current:
					n += position;
					break;
				case SeekFrom::End:
					n = length - n;
					break;
				default:
					return;
			}
			if (n < 0) n = 0;
			position = n;
		}
		virtual Long Position()
		{
			return position;
		}
		virtual Long Length(Long newlen = -1)
		{
			if (newlen < 0) return length;
			else return (length = newlen);
		}
	};
	typedef int FILEDES;
#define CreateFile open
	struct File
	{
		FILEDES _f;
		inline File() :
				_f(0)
		{
		}
		inline ~File()
		{
		}
		inline File(const char *path, int flags)
		{
			_f = CreateFile(path, flags);
			if (_f < 0) throw Exception(errno);
			//int set = 1;
			//setsockopt(_s, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
			dbgprint("file " << _f << " created");
			//this->autoClose=autoClose;
		}
		inline File(const char *path, int flags, mode_t mode)
		{
			_f = CreateFile(path, flags, mode);
			if (_f < 0) throw Exception(errno);
			//int set = 1;
			//setsockopt(_s, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
			dbgprint("file " << _f << " created");
			//this->autoClose=autoClose;
		}
		inline File(FILEDES f)
		{
			_f = f;
			//int set = 1;
			//setsockopt(_s, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
			//this->autoClose=autoClose;
		}
		inline void Close()
		{
			//throw Exception();
			dbgprint("file " << _f << " closed");
			if (_f != -1) close(_f);
			_f = -1;
		}
		inline Int Write(const BufferRef& buf)
		{
			Int tmp = write(_f, buf.Data, buf.Length);
			if (tmp < 0) throw Exception(errno);
			return tmp;
		}
		inline Int Read(const BufferRef& buf)
		{
			Int tmp = read(_f, buf.Data, buf.Length);
			if (tmp < 0) throw Exception(errno);
			return tmp;
		}
		inline off_t Seek(off_t offset, int whence)
		{
			off_t tmp = lseek(_f, offset, whence);
			if (tmp == (off_t) -1) throw Exception(errno);
			return tmp;
		}
		void Flush()
		{
		}
		inline Int GetFlags()
		{
			return fcntl(_f, F_GETFL, 0);
		}
		inline void SetFlags(Int f)
		{
			if (fcntl(_f, F_SETFL, f) < 0) throw Exception(errno, "could not set file flags");
		}
	};
	class FileStream: public Stream
	{
	public:
		File f;
		bool closed;
		FileStream(File f) :
				f(f), closed(false)
		{
		}
		template<class ... ARGS> FileStream(ARGS&& ... a)
		:f(std::forward<ARGS>(a)...), closed(false)
		{
		}
		virtual int Read(const BufferRef& buf)
		{
			return f.Read(buf);
		}
		virtual void Write(const BufferRef& buf)
		{
			int bw = 0;
			int off = 0;
			while (off < buf.Length && (bw = f.Write(buf.SubBuffer(off))) > 0)
			off += bw;
		}
		virtual void Flush()
		{
			//::fflush(f);
			}
			virtual void Close()
			{
				if (closed)
				return;
				closed = true;
				f.Close();
			}
			virtual ~FileStream()
			{
				Close();
			}
			virtual Long Position()
			{
				return f.Seek(0, SEEK_CUR);
			}
			virtual void Seek(Long n, SeekFrom from)
			{
				f.Seek(n, (int) from);
			}
			virtual Cap Capabilities()
			{
				return Cap::All;
			}
		};
	class StandardStream: public Stream
	{
	public:
		StandardStream()
		{
		}
		virtual int Read(const BufferRef& buf)
		{
			return File(0).Read(buf);
		}
		virtual void Write(const BufferRef& buf)
		{
			File(1).Write(buf);
		}
		virtual void Flush()
		{
		}
		virtual void Close()
		{
		}
		virtual Cap Capabilities()
		{
			return Cap::ReadWrite;
		}
	};
	class FileStreamSource: public StreamSource
	{
	public:
		string filename;
		int flags;
		mode_t mode;
		virtual Stream& CreateStream()
		{
			FileStream* fs = new FileStream(File(filename.c_str(), flags, mode));
			return *fs;
		}
		virtual ~FileStreamSource()
		{
		}
	};
#ifdef cplib_glib_wrappers
	class GIOGenericStream: public Stream
	{
	public:
		Glib::RefPtr<Glib::Object> f;
		//Glib::RefPtr<Glib::Interface> i;
		/*enum class T:Byte
		 {
		 I=1,O=2,IO=I|O
		 };
		 T t;*/
		Cap c;
		void CheckCaps()
		{
			Glib::RefPtr<Gio::Seekable> s=Glib::RefPtr<Gio::Seekable>::cast_dynamic(f);
			if(s)
			c=c|Cap::Seek;
			else c=c&(~Cap::Seek);
		}
		GIOGenericStream(Glib::RefPtr<Gio::IOStream> f):f(f),c(Cap::ReadWrite)
		{	CheckCaps();}
		GIOGenericStream(Glib::RefPtr<Gio::InputStream> f):f(f),c(Cap::Read)
		{	CheckCaps();}
		GIOGenericStream(Glib::RefPtr<Gio::OutputStream> f):f(f),c(Cap::Write)
		{	CheckCaps();}
		virtual ~GIOGenericStream()
		{}
		virtual int Read(const BufferRef& buf)
		{
			if((c&Cap::Read)==Cap::None)throw NotSupportedException();
			Glib::RefPtr<Gio::InputStream> s;
			if((c&Cap::Write)!=Cap::None)
			{
				Glib::RefPtr<Gio::IOStream> f=Glib::RefPtr<Gio::IOStream>::cast_static(this->f);
				s=f->get_input_stream();
			}
			else s=Glib::RefPtr<Gio::InputStream>::cast_static(this->f);
			return s->read(buf.Data,buf.Length);
		}
		virtual void Write(const BufferRef& buf)
		{
			if((c&Cap::Write)==Cap::None)throw NotSupportedException();
			Glib::RefPtr<Gio::OutputStream> s;
			if((c&Cap::Read)!=Cap::None)
			{
				Glib::RefPtr<Gio::IOStream> f=Glib::RefPtr<Gio::IOStream>::cast_static(this->f);
				s=f->get_output_stream();
			}
			else s=Glib::RefPtr<Gio::OutputStream>::cast_static(this->f);
			gsize tmp;
			if(!s->write_all(buf.Data,buf.Length,tmp))
			{
				throw Exception("GIO error");
			}
			//return s.write(buf.Data,buf.Length);
		}
		virtual void Flush()
		{
			if((c&Cap::Write)==Cap::None)throw NotSupportedException();
			Glib::RefPtr<Gio::OutputStream> s;
			if((c&Cap::Read)!=Cap::None)
			{
				Glib::RefPtr<Gio::IOStream> f=Glib::RefPtr<Gio::IOStream>::cast_static(this->f);
				s=f->get_output_stream();
			}
			else s=Glib::RefPtr<Gio::OutputStream>::cast_static(this->f);
			s->flush();
		}
		virtual void Close()
		{
			if((c&Cap::ReadWrite)==Cap::ReadWrite)
			{
				Glib::RefPtr<Gio::IOStream> f=Glib::RefPtr<Gio::IOStream>::cast_static(this->f);
				f->close();
				//WARN(2,"sssssss");
			}
			else if((c&Cap::Read)!=Cap::None)
			{
				Glib::RefPtr<Gio::InputStream> f=Glib::RefPtr<Gio::InputStream>::cast_static(this->f);
				f->close();
			}
			else if((c&Cap::Write)!=Cap::None)
			{
				Glib::RefPtr<Gio::OutputStream> f=Glib::RefPtr<Gio::OutputStream>::cast_static(this->f);
				f->close();
			}
		}
		virtual Long Position()
		{
			if((c&Cap::Seek)==Cap::None)throw NotSupportedException();
			Glib::RefPtr<Gio::Seekable> s=Glib::RefPtr<Gio::Seekable>::cast_dynamic(f);
			return s->tell();
		}
		virtual void Seek(Long n, SeekFrom from)
		{
			if((c&Cap::Seek)==Cap::None)throw NotSupportedException();
			Glib::RefPtr<Gio::Seekable> s=Glib::RefPtr<Gio::Seekable>::cast_dynamic(f);
			Glib::SeekType t;
			switch(from)
			{
				case SeekFrom::Begin:
				t=Glib::SEEK_TYPE_SET; break;
				case SeekFrom::End:
				t=Glib::SEEK_TYPE_END; break;
				case SeekFrom::Current:
				t=Glib::SEEK_TYPE_CUR; break;
				default: return;
			}
			s->seek(n, t);
		}
		virtual Long Length(Long newlen=-1)
		{
			if(newlen>=0) //truncate
			{
				if((c&Cap::Seek)==Cap::None)throw NotSupportedException();
				Glib::RefPtr<Gio::Seekable> s=Glib::RefPtr<Gio::Seekable>::cast_dynamic(f);
				s->truncate(newlen);
				return newlen;
			}
			else throw NotSupportedException();
			//return 0;
		}
		virtual Cap Capabilities()
		{	return c;}
	};
#endif

	class StreamReaderWriter: public Stream
	{
	public:
		Property<Stream> s;
		void* buf;
		StringBuilder wbuf;
		int wbuffersize;
		int max_wbuffer_copy;
		int buf_size;
		int buf_index;
		int buf_length;
		StreamReaderWriter(Stream& s, int rbuffersize = 4096, int wbuffersize = 4096);
		virtual ~StreamReaderWriter();
		virtual int Read(const BufferRef& buf);
		int Read(StringBuilder& buf, int length);
		int Read(StringBuilder& buf, const char* delimitors, int delimitor_count);

		//virtual int read(Stream *buf,int length);

		//skips over any extra delimitors
		int Read(Stream& out, const char* delimitors, int delimitor_count);
		/*{
		 return Read([&out](const BufferRef& data)
		 {
		 out.Write(data);
		 }, delimitors, delimitor_count);
		 }*/
		void BeginRead(const function<void(const BufferRef& data, bool isLast)>& cb,
				const char* delimitors, int delimitor_count)
		{

		}
		inline Short ReadByte()
		{
			if (buf_length <= 0) {
				Buffer tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0) return -1;
				buf_index = 0;
				buf_length = tmp;
			}
			return ((char*) (this->buf))[buf_index++];
		}
		//
		//buf: destination stream(where data read will be put).
		//delimitors: array of delimitors
		//delimitor_count: self explanatory
		//delim_index: (out) the index (in the array 'delimitors') of the delimitor
		//				that the read operation stopped at
		//NOTES:
		//looks for delimitors in the order they are specified.
		//delimitors should NOT be null terminated.
		//the delimitor will be consumed.
		//RETURN VALUE: -1 if no data was read; 0 if delimitor was read but there
		//was no data in between;
		int Read(Stream& out, const STRING* delimitors, int delimitor_count,
				int* delim_index = NULL);
		/*{
		 return Read([&out](const BufferRef& data)
		 {
		 out.Write(data);
		 }, delimitors, delimitor_count, delim_index);
		 }*/

		//delimitors have to be 1, 2, 4, or 8 bytes
		//int fast_readline(const function<void(const BufferRef& data)>& cb);
		int fast_readline(Stream& out);
		/*{
		 return fast_readline([&out](const BufferRef& data)
		 {
		 out.Write(data);
		 });
		 }*/
		int ReadLine(Stream& buf)
		{
			STRING delim[2]
			{ "\r\n", "\n" };
			return Read(buf, delim, 2);
			//return Read(buf, "\r\n", 2);
		}
		int ReadLine(string& s)
		{
			struct asdf: public Stream
			{
				string& s;
				asdf(string& s) :
						s(s)
				{
				}
				virtual int Read(const BufferRef& buf)
				{
					return 0;
				}
				virtual void Write(const BufferRef& buf)
				{
					s.append((char*) buf.Data, buf.Length);
				}
				virtual void Flush()
				{
				}
				virtual void Close()
				{
				}
			} tmp(s);
			return ReadLine(tmp);
		}
		virtual void Write(const BufferRef& buf);
		virtual void Flush();
		virtual void Close();
		void do_flush()
		{
			if (wbuf.length <= 0) return;
			s->Write(wbuf.ToBuffer());
			wbuf.Clear();
		}
		inline void flush_if_full(int space = 0)
		{
			if (wbuf.length > (wbuffersize - space)) do_flush();
		}
		void Write(void* buf, int len)
		{
			if (len > max_wbuffer_copy) {
				s->Write(Buffer(buf, len));
				return;
			}
			//s->Write(buf);
			//flush_if_full(len);
			wbuf.Append((char*) buf, len);
			flush_if_full();
		}
		inline void write_fast(void* buf, int len)
		{
			//flush_if_full(len);
			wbuf.Append((char*) buf, len);
			flush_if_full();
		}
		inline void Write(char* s)
		{
			Write(s, strlen(s));
		}
		template<class T> inline void Write(T x)
		{
			//flush_if_full(len);
			wbuf.Append(x);
			flush_if_full();
		}
		template<class T> inline void WriteLine(T x)
		{
			wbuf.Append(x);
			wbuf.Append('\n');
			flush_if_full();
		}
		inline void WriteLine()
		{
			wbuf.Append('\n');
			flush_if_full();
		}
	};
	struct StreamReader_async
	{
		Property<StreamReaderWriter> sr;
		Property<Stream> out;
		const STRING* delimitors;
		int delimitor_count;
		int* delim_index;
		Int br;
		StreamReader_async() :
				delim_index(NULL)
		{
		}
		void begin();
		void check_buffer(); //make sure the buffer is not empty. if it is, begin a read operation
		void check_buffer_cb();
		void do_process(); //called after check_buffer() or check_buffer_cb
		void shift_buffer(); //if end-of-buffer is found while comparing against a delimitor, flush existing data
							 //before the start of the delimitor, shift everything after the delimitor forward, and
							 //read more data into the buffer
		void found_char(); //after a matching substring is found
		void completed(Int ret); //calls the callback function

	};
	typedef StreamReaderWriter StreamWriter;
	typedef StreamReaderWriter StreamReader;
	template<class X> inline StringBuilder& operator<<(StringBuilder& sb, const X& x)
	{
		sb.Append(x);
		return sb;
	}
	/*inline StringBuilder& operator<<(StringBuilder& sb, const char* x)
	 {
	 sb.Append(x);
	 return sb;
	 }*/
	template<class X> inline Stream& operator<<(Stream& s, const X& x)
	{
		s.Write(x);
		return s;
	}
	inline Int operator>>(Stream& s, const Buffer& b)
	{
		return s.Read(b);
	}
	inline Int operator>>(StreamReaderWriter& s, Stream& o)
	{
		return s.ReadLine(o);
	}
	template<class X> inline StreamWriter& operator<<(StreamWriter& s, const X& x)
	{
		s.Write(x);
		return s;
	}
	template<class S, class X> inline S& operator<<(S&& os, const vector<X>& dt)
	{
		os << "vector[";
		bool notfirst = false;
		for (auto it = dt.begin(); it != dt.end(); it++)
		(notfirst || !(notfirst = true)) ? os << ", " << (*it) : os << (*it);
		os << "]";
		return os;
	}
	template<class S, class X> inline S& operator<<(S&& os, const list<X>& dt)
	{
		os << "list[";
		bool notfirst = false;
		for (auto it = dt.begin(); it != dt.end(); it++)
		(notfirst || !(notfirst = true)) ? os << ", " << (*it) : os << (*it);
		os << "]";
		return os;
	}
	template<class S, class X> inline S& operator<<(S&& os, const set<X>& dt)
	{
		os << "set[";
		bool notfirst = false;
		for (auto it = dt.begin(); it != dt.end(); it++)
		(notfirst || !(notfirst = true)) ? os << ", " << (*it) : os << (*it);
		os << "]";
		return os;
	}

	template<class T> inline T& operator<<(T& out, void* obj)
	{
		out << "&{...}";
		return out;
	}
	/*template<class T, class X> T& operator<<(T& out, X* obj)
	{
		out << "&(";
		out << *obj;
		out << ')';
		return out;
	}*/
	class BitArray64
	{
	public:
		__uint64_t* data;
		int size;
		BitArray64(int length)
		{
			this->size=ceil(((double)length)/64);
			this->data=new __uint64_t[size];
		}
		~BitArray64()
		{
			delete[] data;
		}
		inline bool Get(int i)
		{ //WARNING: no runtime check
			__uint64_t tmp=data[i/64];
			return (tmp&((long)1<<(i%64))) != 0;
		}
		inline void Set(int i, bool b)
		{
			int tmp1=i/64;
			__uint64_t tmp=data[tmp1];
			if(b) data[tmp1]=tmp|((long)1<<(i%64));
			else data[tmp1]=tmp&(~((long)1<<(i%64)));
		}
	};
	class BitArray32
	{
	public:
		__uint32_t* data;
		int size;
		BitArray32(int length)
		{
			this->size=ceil(((double)length)/32);
			this->data=new __uint32_t[size];
		}
		~BitArray32()
		{
			delete[] data;
		}
		inline bool Get(int i)
		{ //WARNING: no runtime check
			__uint32_t tmp=data[i/32];
			return (tmp&((int)1<<(i%32))) != 0;
		}
		inline void Set(int i, bool b)
		{
			int tmp1=i/32;
			__uint32_t tmp=data[tmp1];
			if(b) data[tmp1]=tmp|((int)1<<(i%32));
			else data[tmp1]=tmp&(~((int)1<<(i%32)));
		}
	};
#ifdef __32_bit
	typedef BitArray32 BitArray; // 32 bit
#else
	typedef BitArray64 BitArray; // 64 bit
#endif
	template<class T> class ArrayList: public Object
	{
	public:
		ArrayList(int initsize = 32);
		virtual ~ArrayList();
		T* array;
		int Length;
		int Capacity;
		void EnsureCapacity(int c);
		void Append(T& item);
		void Append(const T* items, int length);
	};
	template<class T> ArrayList<T>::ArrayList(int initsize)
	{
		array = (T*) malloc(initsize * sizeof(T));
		Capacity = initsize;
		Length = 0;
	}
	template<class T> ArrayList<T>::~ArrayList()
	{
		free(array);
	}
	template<class T> void ArrayList<T>::EnsureCapacity(int c)
	{
		if (Capacity >= c) return;
		int tmp = this->Capacity;
		while (tmp < c) {
			tmp *= 2;
		}
		this->array = (T*) realloc(this->array, tmp * sizeof(T));
		this->Capacity = tmp;
	}
	template<class T> void ArrayList<T>::Append(T& item)
	{
		EnsureCapacity(Length + 1);
		array[Length] = item;
		Length++;
	}
	template<class T> void ArrayList<T>::Append(const T* items, int length)
	{
		EnsureCapacity(this->Length + length);
		memcpy(array + this->Length, items, length * sizeof(T));
	}
	class BufferManager;
	static BufferManager* __def_BufferManager;
	class BufferManager: public Object
	{
	public:

		inline static BufferManager* GetDefault()
		{
			if (__def_BufferManager == NULL) __def_BufferManager = new BufferManager();
			return __def_BufferManager;
		}
		vector<Buffer> l;
		int bufferSize, maxSpare;
		BufferManager(int bufferSize = 65536, int maxSpare = 10) :
				bufferSize(bufferSize), maxSpare(maxSpare)
		{
		}
		~BufferManager()
		{
			/*int i;
			 for(i=0;i<l.Length;i++)
			 {
			 l.array[i].Release();
			 }*/
		}
		Buffer Get()
		{
			if (l.size() <= 0) return Buffer(bufferSize);
			else {
				auto tmp = l[l.size() - 1];
				l.pop_back();
				return tmp;
			}
		}
		void Return(Buffer& b)
		{
			if ((int) l.size() < maxSpare) l.push_back(b);
			//else b->Release();
		}
	};
	template<class T, class LockObj = MutexLock> class EventQueue: public Object
	{
	public:
		int efd;
		queue<T> q;
		LockObj l;
		typedef xaxaxa::Lock<LockObj> Lock;
		EventQueue()
		{
			efd = eventfd(0, EFD_SEMAPHORE);
		}
		~EventQueue()
		{
			close(efd);
		}
		void Append(const T& value)
		{
			{
				Lock lck(l);
				q.push(value);
			}
			eventfd_write(efd, 1);
		}
		T Dequeue()
		{
			eventfd_t val;
			eventfd_read(efd, &val);
			{
				Lock lck(l);
				T tmp = q.front();
				q.pop();
				return tmp;
			}
		}
	};
	class EventHandle: public Object
	{
	public:
		int efd;
		EventHandle()
		{
			efd = eventfd(0, 0);
		}
		~EventHandle()
		{
			close(efd);
		}
		void Set()
		{
			eventfd_write(efd, 1);
		}
		eventfd_t Wait()
		{
			eventfd_t val;
			eventfd_read(efd, &val);
			return val;
		}
	};
	class ConditionHandle: public Object
	{
	public:
		int efd;
		bool value;
		ConditionHandle(bool value = false) :
				value(value)
		{
			efd = eventfd(0, 0);
		}
		~ConditionHandle()
		{
			close(efd);
		}
		void Set()
		{
			value = true;
			eventfd_write(efd, 1);
		}
		void Reset()
		{
			value = false;
		}
		void Wait() //wait for value to become true
		{
			eventfd_t val;
			while (!value)
				eventfd_read(efd, &val);
		}
	};
#define __intwrap1(var,max) var%=(max);
#define __intwrap(var,max) ((var)%(max))
	/*template<class T> class CircularQueue: public Object
	 {
	 public:
	 T* array;
	 int size;
	 int __wrap;
	 int s1, s2, e1, e2;
	 BitArray b;
	 CircularQueue(int size) :
	 b(size)
	 {
	 s1 = 0;
	 s2 = 0;
	 e1 = 0;
	 e2 = 0;
	 this->size = size;
	 this->__wrap = size * 2;
	 array = new T[size];
	 }
	 ~CircularQueue()
	 {
	 delete[] array;
	 }
	 inline int __getlength(int i1, int i2, int wrap)
	 {
	 return (i2 < i1 ? i2 + wrap : i2) - i1;
	 }
	 inline T& GetPointer(int i)
	 {
	 __intwrap1(i, size);
	 if (i >= size || i < 0)
	 throw new OutOfRangeException("CircularQueue::GetPointer() out of range");
	 return array[i]; //__intwrap(i,size);
	 }
	 inline bool CanAppend()
	 {
	 return __getlength(s1, e2, __wrap) < size;
	 }
	 int BeginAppend()
	 {
	 if (__getlength(s1, e2, __wrap) >= size) return -1;
	 int tmp = e2++;
	 __intwrap1(e2, __wrap);
	 b.Set(__intwrap(tmp,size), true);
	 return tmp;
	 }
	 void EndAppend(int i)
	 {
	 if (i == e1)
	 {
	 do
	 {
	 e1++;
	 __intwrap1(e1, __wrap);
	 }
	 while (__getlength(e1, e2, __wrap) > 0 && !(b.Get(__intwrap(e1,size))));
	 }
	 else b.Set(__intwrap(i,size), false);
	 }
	 inline bool CanDequeue()
	 {
	 return __getlength(s2, e1, __wrap) > 0;
	 }
	 int BeginDequeue()
	 {
	 if (__getlength(s2, e1, __wrap) <= 0) return -1;
	 int tmp = s2++;
	 __intwrap1(s2, __wrap);
	 b.Set(__intwrap(tmp,size), true);
	 return tmp;
	 }
	 void EndDequeue(int i)
	 {
	 if (i == s1)
	 {
	 do
	 {
	 s1++;
	 __intwrap1(s1, __wrap);
	 }
	 while (__getlength(s1, s2, __wrap) > 0 && !(b.Get(__intwrap(s1,size))));
	 }
	 else b.Set(__intwrap(i,size), false);
	 }
	 };*/
	template<class T> class CircularQueue: public Object
	{
	public:
		T* array;
		BitArray b;
		Int size;
		Int objsize;
		Int __wrap;
		Int s1, s2, e1, e2;
		CircularQueue(Int size, Int objsize = 1) :
				b(size), size(size), objsize(objsize), __wrap(size * 2), s1(0), s2(0), e1(0), e2(0)
		{
			array = new T[size * objsize];
		}
		~CircularQueue()
		{
			delete[] array;
		}
		inline int __getlength(Int i1, Int i2, Int wrap)
		{
			return (i2 < i1 ? i2 + wrap : i2) - i1;
		}
		inline T& GetPointer(Int i)
		{
			__intwrap1(i, size);
			if (i >= size || i < 0) throw OutOfRangeException(
					"CircularQueue::GetPointer() out of range");
			return array[i * objsize]; //__intwrap(i,size);
		}
		inline bool CanAppend()
		{
			return __getlength(s1, e2, __wrap) < size;
		}
		int BeginAppend()
		{
			if (__getlength(s1, e2, __wrap) >= size) return -1;
			int tmp = e2++;
			__intwrap1(e2, __wrap);
			b.Set(__intwrap(tmp,size), true);
			return tmp;
		}
		void EndAppend(Int i)
		{
			if (i == e1) {
				do {
					e1++;
					__intwrap1(e1, __wrap);
				} while (__getlength(e1, e2, __wrap) > 0 && !(b.Get(__intwrap(e1,size))));
			} else b.Set(__intwrap(i,size), false);
		}
		inline Int Length()
		{
			return __getlength(s2, e1, __wrap);
		}
		inline bool CanDequeue()
		{
			return __getlength(s2, e1, __wrap) > 0;
		}
		Int BeginDequeue()
		{
			if (__getlength(s2, e1, __wrap) <= 0) return -1;
			Int tmp = s2++;
			__intwrap1(s2, __wrap);
			b.Set(__intwrap(tmp,size), true);
			return tmp;
		}
		void EndDequeue(Int i)
		{
			if (i == s1) {
				do {
					s1++;
					__intwrap1(s1, __wrap);
				} while (__getlength(s1, s2, __wrap) > 0 && !(b.Get(__intwrap(s1,size))));
			} else b.Set(__intwrap(i,size), false);
		}
	};
	class CircularBuffer: public Object
	{
	public:
		Byte* array;
		Int size;
		Int __wrap;
		Int s1, s2, e1, e2;
		CircularBuffer(Int size) :
				size(size), __wrap(size * 2), s1(0), s2(0), e1(0), e2(0)
		{
			array = new Byte[size];
		}
		~CircularBuffer()
		{
			delete[] array;
		}
		inline int __getlength(Int i1, Int i2, Int wrap)
		{
			return (i2 < i1 ? i2 + wrap : i2) - i1;
		}
		inline Byte* GetPointer(Int i)
		{
			__intwrap1(i, size);
			if (i >= size || i < 0) throw OutOfRangeException(
					"CircularBuffer::GetPointer() out of range");
			return array + i; //__intwrap(i,size);
		}
		inline bool BufferOperations(const function<bool(BufferRef b)>& func, Int i, Int c)
		{ //perform an arbitrary operation on a buffer segment;
		  //for example writing a section of the buffer to disk
		  //this function mainly handles buffer fragmentation
			Int tmp(size - __intwrap(i, size));
			if (tmp > c) tmp = c;
			if (!func(BufferRef(GetPointer(i), tmp))) return false;
			if (tmp < c) return func(BufferRef(GetPointer(0), c - tmp));
			return true;
		}
		inline bool CanAppend(Int c)
		{
			return __getlength(s1, e2, __wrap) + c <= size;
		}
		Int BeginAppend(Int c)
		{
			if (__getlength(s1, e2, __wrap) + c > size) return -1;
			Int tmp(e2);
			//e2 += c;
			e2 = __intwrap(e2 + c, __wrap);
			//b.Set(__intwrap(tmp,size), true);
			return tmp;
		}
		void EndAppend(Int i, Int c)
		{
			e2 = e1 = __intwrap(e1 + c, __wrap);
		}
		inline Int Length()
		{
			return __getlength(s2, e1, __wrap);
		}
		inline bool CanDequeue()
		{
			return __getlength(s2, e1, __wrap) > 0;
		}
		Int BeginDequeue(Int& c, bool allow_short_read = true)
		{
			if (allow_short_read) {
				Int data_avail(__getlength(s2, e1, __wrap));
				if (data_avail <= 0) return -1;
				if (data_avail < c) c = data_avail;
			} else {
				if (__getlength(s2, e1, __wrap) < c) return -1;
			}
			Int tmp(s2);
			s2 = __intwrap(s2 + c, __wrap);
			return tmp;
		}
		void EndDequeue(Int i, Int c)
		{
			s2 = s1 = __intwrap(s1 + c, __wrap);
		}
	};
	class CircularStream: public Stream
	{
	public:
		Buffer b;
		Int rpos, len; //0 <= len <= b.Length; 0 <= rpos < b.Length
		CircularStream(Int size = 4096) :
				b(size), rpos(0), len(0)
		{

		}
		virtual Int Read(const BufferRef& buf)
		{
			Int l(buf.Length);
			Int l2;
			if (l > len) l = len;
			if (l <= 0) return 0;
			l2 = b.Length - rpos;
			if (l2 > l) l2 = l;
			memcpy(buf.Data, b.Data + rpos, l2);
			rpos += l2;
			if (rpos >= b.Length) {
				memcpy(buf.Data + l2, b.Data, l - l2);
				rpos = l - l2;
			}
			len -= l;
			return l;
		}
		void Clear()
		{
			len = 0;
		}
		virtual Int Read(Stream& s, Int length)
		{
			Int l(length);
			Int l2;
			if (l > len) l = len;
			if (l <= 0) return 0;
			l2 = b.Length - rpos;
			if (l2 > l) l2 = l;
			//memcpy(buf.Data, b.Data + rpos, l2);
			s.Write(b.SubBuffer(rpos, l2));
			rpos += l2;
			if (rpos >= b.Length) {
				//memcpy(buf.Data + l2, b.Data, l - l2);
				s.Write(b.SubBuffer(0, l - l2));
				rpos = l - l2;
			}
			len -= l;
			return l;
		}
		virtual void Write(const BufferRef& buf)
		{
			BufferRef buf2(buf);
			if (buf2.Length > b.Length) buf2.Clip(buf2.Length - b.Length, b.Length);
			Int wpos((rpos + len) % b.Length);
			Int l2(b.Length - wpos);
			if (l2 > buf2.Length) l2 = buf2.Length;
			memcpy(b.Data + wpos, buf2.Data, l2);
			if ((wpos = (wpos + l2)) >= b.Length) {
				memcpy(b.Data, buf2.Data + l2, buf2.Length - l2);
				wpos = buf2.Length - l2;
			}
			len += buf2.Length;
			if (len > b.Length) len = b.Length;
			rpos = modulus(wpos - len, b.Length);
		}
		virtual Int Write(Stream& s, Int length)
		{
			//BufferRef buf2(buf);
			if (length > b.Length) s.Skip(length - b.Length);
			Int wpos((rpos + len) % b.Length);
			Int l2(b.Length - wpos);
			if (l2 > length) l2 = length;
			//memcpy(b.Data + wpos, buf2.Data, l2);
			Int br = s.Fill(b.SubBuffer(wpos, l2));
			if (br < l2) {
				length -= (l2 - br);
				l2 = br;
			}
			if ((wpos = (wpos + l2)) >= b.Length) {
				//memcpy(b.Data, buf2.Data + l2, buf2.Length - l2);
				wpos = s.Fill(b.SubBuffer(0, length - l2));
				if (wpos < (length - l2)) len -= (length - l2) - wpos; //compensate for buffer short-read
				//wpos = length - l2;
			}
			len += length;
			if (len > b.Length) len = b.Length;
			rpos = modulus(wpos - len, b.Length);
			return length;
		}
		virtual void Flush()
		{
		}
		virtual void Close()
		{
		}
		virtual void Seek(Long n, SeekFrom from = SeekFrom::Begin)
		{
		}
		virtual Long Position()
		{
			return 0;
		}
		virtual Long Length(Long newlen = -1)
		{
			if (newlen >= 0 && newlen < len) {
				rpos = (rpos + len - newlen) % b.Length;
				len = newlen;
			}
			return len;
		} //set or get length
		virtual Cap Capabilities()
		{
			return Cap::ReadWrite | Cap::Length;
		}
		struct iterator
		{
			CircularStream& s;
			Int pos;
			iterator(CircularStream& s, Int pos) :
					s(s), pos(pos)
			{
			}
			inline Byte operator*()
			{
				return s.b.Data[pos % s.b.Length];
			}
			inline void operator++(int i)
			{
				pos = (pos + 1) % (s.b.Length * 2);
			}
			inline void operator--(int i)
			{
				if (--pos < 0) pos += (s.b.Length * 2);
			}
			inline bool operator==(const iterator& other)
			{
				return pos == other.pos;
			}
			inline bool operator!=(const iterator& other)
			{
				return pos != other.pos;
			}
			inline iterator operator+(int i)
			{
				return {s,(pos+i)%(s.b.Length*2)};
			}
			inline iterator operator-(int i)
			{
				return {s,modulus(pos-i,s.b.Length*2)};
			}
			inline void operator+=(int i)
			{
				pos = (pos + i) % (s.b.Length * 2);
			}
			inline void operator-=(int i)
			{
				pos = modulus(pos - i, s.b.Length * 2);
			}
		};
		inline iterator begin()
		{
			return {*this,rpos};
		}
		inline iterator end()
		{
			return {*this,(rpos+len)%(b.Length*2)};
		}
		void Clip(const iterator& begin, const iterator& end)
		{
			if (begin.pos % b.Length > rpos) rpos = begin.pos % b.Length;
			//Int tmp_l = modulus(end.pos - begin.pos, b.Length * 2);
			Int endpos = (rpos + len) % (b.Length * 2);
			if (end.pos < endpos) len = modulus(end.pos - rpos, b.Length * 2);
		}
		void GetData(const iterator& begin, const iterator& end, Stream& s)
		{
			Int l = modulus(end.pos - begin.pos, b.Length * 2);
			Int p = begin.pos % b.Length;
			//Int l(length);
			Int l2;
			if (l <= 0) return;
			l2 = b.Length - p;
			if (l2 > l) l2 = l;
			//memcpy(buf.Data, b.Data + rpos, l2);
			s.Write(b.SubBuffer(p, l2));
			p += l2;
			if (p >= b.Length) {
				//memcpy(buf.Data + l2, b.Data, l - l2);
				s.Write(b.SubBuffer(0, l - l2));
			}
		}
	};
	inline Long Splice(Stream& from, CircularStream& to, Long c)
	{
		return to.Write(from, c);
	}
	inline Long Splice(CircularStream& from, Stream& to, Long c)
	{
		return from.Read(to, c);
	}
	template<class T, class Allocator = allocator<T> > class vectorlist
	{
	public:
		struct list_item
		{
			vector<T, Allocator> v;
		};
		list<list_item, Allocator> l;
		class iterator
		{
		public:
			vectorlist<T, Allocator>& vl;
			typename list<list_item>::iterator it;
			int vect_index;
			inline T& operator*() const
			{
				return (*it).v[vect_index];
			}
			inline void operator++(int i)
			{
				//if (it == vl.l.end())
				//	return;
				vect_index++;
				if (vect_index >= (*it).v.size()) {
					it++;
					vect_index = 0;
				}
			}
			void operator--(int i)
			{
				vect_index--;
				if (vect_index < 0) {
					if (it == vl.l.begin()) {
						vect_index = 0;
						return;
					}
					it--;
					vect_index = (*it).v.size() - 1;
				}
			}
			inline bool is_end() const
			{
				return it == vl.l.end();
			}
			inline bool operator==(const iterator& other) const
			{
				return (it == other.it && vect_index == other.vect_index);
				//return true;
				//if (is_end() && other.is_end())
				//	return true;
				//return false;
			}
			inline bool operator!=(const iterator& other) const
			{
				return !(*this == other);
			}
		};
		int vect_size;
		vectorlist(int vect_size = 4096) :
				vect_size(vect_size)
		{

		}
		void push_back(const T& item)
		{
			auto it = l.end();
			if (it == l.begin()) {
				//create new vector
				vector<T> tmp
				{ item };
				l.push_back(
				{ tmp });
				return;
			}
			it--;
			if ((*it).v.size() >= vect_size) {
				//create new vector
				vector<T> tmp
				{ item };
				l.push_back(
				{ tmp });
				return;
			}
			(*it).v.push_back(item);
			return;
			//{	*this, it,(*it).v.size()-1};
		}
		void pop_front()
		{
			auto it = l.begin();
			if (it == l.end()) return;
			auto& v = (*it).v;
			if (v.size() <= 0) return;
			v.erase(0);
			if (v.size() <= 0) l.erase(it);
		}
		void erase(const iterator& it)
		{
			auto& it1 = it.it;
			auto& v = (*it1).v;
			v.erase(v.begin() + it.vect_index);
			if (v.size() <= 0) l.erase(it1);
		}
		iterator begin()
		{
			return
			{	*this, l.begin(),0};
		}
		iterator end()
		{
			return
			{	*this,l.end(),0};
		}
	};
	class PointerException: public Exception
	{
	public:
		inline PointerException() :
				Exception("segmentation fault")
		{
			//this();
		}
		inline PointerException(std::string message) :
				Exception(message)
		{

		}
	};


	class Util_c
	{
	public:

		int _argc;
		char** _argv;
		string GetDirFromPath(const string path)
		{
			Int i = path.rfind("/");
			if (i < 0) return string();
			return path.substr(0, i + 1);
		}
		string GetProgramPath()
		{
			char buf[256];
			Int i = readlink("/proc/self/exe", buf, sizeof(buf));
			if (i < 0) throw Exception(errno);
			return string(buf, i);
		}
		void ChDir(string dir)
		{
			if (chdir(dir.c_str()) < 0) throw Exception(errno);
		}
		void RestartOnCrash(int argc, char** argv)
		{
			_argc = argc;
			_argv = argv;
			pid_t pid = fork();
			if (pid == 0) {
				return;
			} else if (pid < 0) {
				throw Exception(errno);
			}
		}
		void ParseArgs(int argc, char** argv,
				const function<void(char*, const function<char*()>&)>& cb)
		{
			int i = 1;
			function<char*()> func = [&]()->char*
			{
				if(i+1>=argc)return NULL;
				return argv[(++i)];
			};
			for (; i < argc; i++) {
				if (argv[i][0] == '\x00') continue;
				if (argv[i][0] == '-') {
					cb(argv[i] + 1, func);
				} else {
					cb(NULL, [argv,i]()
					{	return argv[i];});
				}
			}
		}
#if __x86_64__
		typedef struct ucontext sig_ucontext_t;
		static void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext)
		{
			sig_ucontext_t * uc = (sig_ucontext_t *) ucontext;
			void * caller_address = (void *) uc->uc_mcontext.gregs[REG_RIP]; // x86 specific
			std::cerr << "signal " << sig_num << " (" << strsignal(sig_num) << "), address is "
			<< info->si_addr << " from " << caller_address << std::endl << std::endl;
			void * array[50];
			int size = backtrace(array, 50);
			array[1] = caller_address;
			char ** messages = backtrace_symbols(array, size);
			// skip first stack frame (points here)
			for (int i = 1; i < size && messages != NULL; ++i)
			{
				char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;
				// find parantheses and +address offset surrounding mangled name
				for (char *p = messages[i]; *p; ++p)
				{
					if (*p == '(')
					{
						mangled_name = p;
					}
					else if (*p == '+')
					{
						offset_begin = p;
					}
					else if (*p == ')')
					{
						offset_end = p;
						break;
					}
				}
				// if the line could be processed, attempt to demangle the symbol
				if (mangled_name && offset_begin && offset_end && mangled_name < offset_begin)
				{
					*mangled_name++ = '\0';
					*offset_begin++ = '\0';
					*offset_end++ = '\0';
					int status;
					char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

					// if demangling is successful, output the demangled function name
					if (status == 0)
					{
						std::cerr << "[bt]: (" << i << ") " << messages[i] << " : " << real_name
						<< "+" << offset_begin << offset_end << std::endl;
					}
					// otherwise, output the mangled function name
					else
					{
						std::cerr << "[bt]: (" << i << ") " << messages[i] << " : " << mangled_name
						<< "+" << offset_begin << offset_end << std::endl;
					}
					free(real_name);
				}
				// otherwise, print the whole line
				else
				{
					std::cerr << "[bt]: (" << i << ") " << messages[i] << std::endl;
				}
			}
			std::cerr << std::endl;
			free(messages);
			exit(EXIT_FAILURE);
		}
#else
		// This structure mirrors the one found in /usr/include/asm/ucontext.h
		typedef struct _sig_ucontext {
		   unsigned long     uc_flags;
		   struct ucontext   *uc_link;
		   stack_t           uc_stack;
		   struct sigcontext uc_mcontext;
		   sigset_t          uc_sigmask;
		} sig_ucontext_t;
		static void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext) {
			sig_ucontext_t * uc = (sig_ucontext_t *)ucontext;

			// Get the address at the time the signal was raised from the EIP (x86)
			void * caller_address = (void *) uc->uc_mcontext.eip;

			std::cerr << "signal " << sig_num
					  << " (" << strsignal(sig_num) << "), address is "
					  << info->si_addr << " from "
					  << caller_address << std::endl;
			void * array[50];
			int size = backtrace(array, 50);
			std::cerr << __FUNCTION__ << " backtrace returned "
					  << size << " frames\n\n";
			// overwrite sigaction with caller's address
			array[1] = caller_address;
			char ** messages = backtrace_symbols(array, size);

			// skip first stack frame (points here)
			for (int i = 1; i < size && messages != NULL; ++i) {
				std::cerr << "[bt]: (" << i << ") " << messages[i] << std::endl;
			}
			std::cerr << std::endl;
			free(messages);
			exit(EXIT_FAILURE);
		}

#endif
		void SetHandlers()
		{
//#if __x86_64__
			struct sigaction sigact;
			sigact.sa_sigaction = &Util_c::crit_err_hdlr;
			sigact.sa_flags = SA_RESTART | SA_SIGINFO;
			sigaction(SIGSEGV, &sigact, (struct sigaction *) NULL);
			sigaction(SIGABRT, &sigact, (struct sigaction *) NULL);
			sigaction(SIGFPE, &sigact, (struct sigaction *) NULL);
//#endif
		}
	};
	extern Util_c Util;
}
#endif
