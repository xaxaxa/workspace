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
#ifdef cplib_glib_wrappers
#include <glibmm.h>
#include <giomm.h>
#endif
//#define FUNCTION(RETVAL,...) struct{RETVAL(*f)(void*,__VA_ARGS__);void* obj;}
//RETVAL(*)(__VA_ARGS__)

#define FUNCTION_ISNULL(fn) ((fn).f==NULL)
#define FUNCTION_DECLARE(NAME,RETVAL,...) \
	struct NAME;struct NAME{\
		RETVAL(*f)(void*,__VA_ARGS__);\
		void* obj;\
		inline NAME(RETVAL(*f)(void*,__VA_ARGS__),void* obj)\
		{this->f=f;\
		 this->obj=obj;}\
		template<class FGSAFJGFJSA>inline NAME(RETVAL(FGSAFJGFJSA::*f)(__VA_ARGS__),void* obj)\
		{this->f=reinterpret_cast<RETVAL(*)(void*,__VA_ARGS__)>((void*)f);\
		this->obj=obj;}\
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
	}
/*#define FUNCTION_DECLARE(NAME,RETVAL,...) \
	struct NAME;\
	struct NAME{\
	boost::function<RETVAL (void*,__VA_ARGS__)> f;\
	void* obj;\
	template<class X>inline NAME(boost::function<RETVAL (X,__VA_ARGS__)> f,void* obj)\
	{this->f=*(reinterpret_cast<boost::function<RETVAL (void*,__VA_ARGS__)>*>(&f));this->obj=obj;}\
	inline NAME(){f=NULL;}\
	static NAME null;}*/
//#define FUNCTION_EXPORT(CLASS,NAME,FNAME,RETVAL,...) static RETVAL NAME(void* obj,__VA_ARGS__){return ((CLASS*)obj)->FNAME(__VA_ARGS__);}
#define FUNCTION_DECLWRAPPER(NAME,RETVAL,...) static inline RETVAL NAME(void* obj,__VA_ARGS__)
//#define FUNCTION_GET(TYPE,WRAPPER_NAME) {TYPE __func;__func.f=WRAPPER_NAME;__func.obj=this;}
//#define FUNCTION_GETSTATIC(TYPE,WRAPPER_NAME) (TYPE __func,__func.f=WRAPPER_NAME,__func.obj=NULL)
//#define FUNCTION_GET1(TYPE,OBJ,WRAPPER_NAME) {TYPE __func;__func.f=OBJ->WRAPPER_NAME;__func.obj=OBJ;}
#define FUNCTION_CALL(FUNC,...) FUNCTION_ISNULL(FUNC)?throw Exception("attempting to call null function"):(FUNC).f((FUNC).obj,__VA_ARGS__)

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
#define WARN(LEVEL,MSG) if(LEVEL<=WARNLEVEL){if(LEVEL<=1)cerr << "\x1B[41;1;32m"; else if(LEVEL<=2)cerr << "\x1B[1;1;1m"; cerr << MSG << "\x1B[0;0;0m" << endl;}

using namespace std;

namespace xaxaxa
{
//typedef void* Function;

	void __objs_inc();
	void __objs_dec();
	int __objs_get();
	class Object
	{
	public:

		int RetainCount; //reference counter
		Object()
		{
			RetainCount = 1;
#ifdef __debug_obj123
			__objs_inc();
			cout<<"construct<"<<typeid(*this).name()<<">; "<<__objs_get()<<" objects total"<<endl;
#endif
		}
		virtual ~Object()
		{
#ifdef __debug_obj123
			__objs_dec();
			cout<<"destruct<"<<typeid(*this).name()<<">; "<<__objs_get()<<" objects total"<<endl;
#endif
		}
		inline void Retain()
		{
			RetainCount++;
#ifdef __debug_obj123
			cout<<"retain<"<<typeid(*this).name()<<"> = "<<RetainCount<<"; "<<__objs_get()<<" objects total"<<endl;
#endif
		}
		inline void Release()
		{
			RetainCount--;
#ifdef __debug_obj123
			cout<<"release<"<<typeid(*this).name()<<"> = "<<RetainCount<<"; "<<__objs_get()<<" objects total"<<endl;
#endif
			dbgprint("retaincount=" << RetainCount);
			if (RetainCount == 0)
			{
				delete this;
				//cout << o;
			}
		}
	};
	class Exception: public Object
	{
	public:
		int Code;
		std::string Message;
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
	};
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
	};
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
	};
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
	};
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
	typedef __int32_t Int;
	typedef __uint32_t UInt;
	typedef __int64_t Long;
	typedef __uint64_t ULong;
	typedef float Float;
	typedef double Double;
	typedef long double Decimal;
	class Buffer
	{
	public:
		boost::shared_array<Byte> pbuf;
		Byte* Data;
		bool IsRaw;
		Int Length;
		inline Buffer()
		{
			Data = NULL;
			IsRaw = true;
			Length = 0;
		}
		Buffer(void* buf, int length)
		{
			this->Data = (Byte*) buf;
			this->Length = length;
			this->IsRaw = true;
		}
		Buffer(char* buf)
		{
			this->Data = (Byte*) buf;
			this->Length = strlen(buf);
			this->IsRaw = true;
		}
		Buffer(const char* buf)
		{
			this->Data = (Byte*) buf;
			this->Length = strlen(buf);
			this->IsRaw = true;
		}
		Buffer(Byte* buf, int length)
		{
			this->Data = (Byte*) buf;
			this->Length = length;
			this->IsRaw = true;
		}
		Buffer(Byte* buf, int length, boost::shared_array<Byte> orig)
		{
			this->Data = buf;
			this->Length = length;
			this->IsRaw = false;
			this->pbuf = orig;
		}
		Buffer(int length)
		{
			this->Length = length;
			if (length <= 0)
			{
				this->Data = NULL;
				this->IsRaw = true;
				return;
			}
			this->Data = new Byte[length];
			this->pbuf = boost::shared_array<Byte>(this->Data);
			this->IsRaw = false;
		}
		inline Buffer SubBuffer(int index, int length) const
		{
			if (index < 0 || index + length > this->Length)
				throw Exception("SubBuffer() out of range");
			if (IsRaw)
				return Buffer(Data + index, length);
			else
				return Buffer(Data + index, length, pbuf);
		}
		inline Buffer SubBuffer(int index) const
		{
			return SubBuffer(index, Length - index);
		}
		inline void Release()
		{
			if (IsRaw)
				throw Exception(
						"Attempting to release a buffer initialized with a raw pointer. This can be dangerous.");
			pbuf.reset();
		}
	};

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
		virtual Int Read(Buffer buf)=0;
		virtual void Write(Buffer buf)=0;
		bool Fill(Buffer buf)
		{
			int br;
			while((br=Read(buf))>0)
			{
				if(br>=buf.Length)return true;
				buf=buf.SubBuffer(br);
			}
			return false;
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
		//bool AutoClose;
		//typedef boost::function<void (void*,Stream*)> Callback;
		FUNCTION_DECLARE(Callback,void,Stream*);
		int __tmp;
		virtual void BeginRead(Buffer buf, Callback cb)
		{
			__tmp = Read(buf);
			FUNCTION_CALL(cb, this);
		}
		virtual void BeginWrite(Buffer buf, Callback cb)
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
		Stream()
		{}
		virtual ~Stream()
		{
			//if(AutoClose)Close();
		}
	};
	Stream::Cap operator|(Stream::Cap c1, Stream::Cap c2);
	Stream::Cap operator&(Stream::Cap c1, Stream::Cap c2);
	Stream::Cap operator~(Stream::Cap c);
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
		virtual Int Read(Buffer buf)
		{
			return 0;
		}
		virtual void Write(Buffer buf)
		{
		}
		virtual void Flush()
		{
		}
		virtual void Close()
		{
		}
	};
	class Poller;
	inline Poller* GetDefaultPoller();
	class Pollable
	{ //represents something that needs to wait for events but doesn't know how
	public:
		typedef uint32_t event_t;
		Poller* p;
		virtual ~Pollable()
		{
		}
		virtual int GetFileDesc()=0;
		virtual void Trigger(event_t events)=0;
	};
	class StringBuilder: public Stream
	{
	public:
		StringBuilder(int initsize = 4096);
		virtual ~StringBuilder();
		virtual Cap Capabilities()
		{
			return Cap::ReadWrite | Cap::Length | Cap::Seek;
		}
		void *buf;
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
			if (Capacity >= c)
				return;
			int tmp = this->Capacity;
			while (tmp < c)
			{
				tmp *= 2;
			}
			this->buf = realloc(this->buf, tmp);
			this->Capacity = tmp;
		}
		void Append(Buffer buf);
		void Append(STRING buf);
		inline void Append(const char* buf, int length)
		{
			this->EnsureCapacity(this->position + length);
			memcpy((char*) this->buf + this->position, buf, length);
			this->position += length;
			if (this->position > this->length)
				this->length = this->position;
		}
		void Append(const char* buf);
		void Append(const StringBuilder* s);
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
		/*inline void Append(Float n)
		 {
		 //char c[log10(0xFFFFFFFF)+2];
		 char c[22];
		 int i = snprintf(c, sizeof(c), "%f", n);
		 Append(c, i);
		 }*/
		int CompareTo(Buffer buf);
		int CompareTo(const StringBuilder* sb);
		STRING ToString();
		Buffer ToBuffer();
		char* ToCString()
		{
			EnsureCapacity(length + 1);
			((char*) buf)[length] = '\x00';
			return ((char*) buf);
		}
		virtual int Read(Buffer buf);
		virtual void Write(Buffer buf);
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
			if (n < 0)
				n = 0;
			position = n;
		}
		virtual Long Position()
		{
			return position;
		}
		virtual Long Length(Long newlen = -1)
		{
			if (newlen < 0)
				return length;
			else
				return (length = newlen);
		}
	};
	typedef int FILEDES;
#define CreateFile open
	struct File
	{
		FILEDES _f;
		inline File()
		{
		}
		virtual inline ~File()
		{
		}
		inline File(const char *path, int flags)
		{
			_f = CreateFile(path, flags);
			if (_f < 0)
				throw Exception(errno);
			//int set = 1;
			//setsockopt(_s, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
			dbgprint("file " << _f << " created");
			//this->autoClose=autoClose;
		}
		inline File(const char *path, int flags, mode_t mode)
		{
			_f = CreateFile(path, flags, mode);
			if (_f < 0)
				throw Exception(errno);
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
		inline virtual void Close()
		{
			//throw Exception();
			dbgprint("file " << _f << " closed");
			if (_f != -1)
				close(_f);
			_f = -1;
		}
		inline virtual Int Write(Buffer buf)
		{
			Int tmp = write(_f, buf.Data, buf.Length);
			if (tmp < 0)
				throw Exception(errno);
			return tmp;
		}
		inline virtual Int Read(Buffer buf)
		{
			Int tmp = read(_f, buf.Data, buf.Length);
			if (tmp < 0)
				throw Exception(errno);
			return tmp;
		}
		inline virtual off_t Seek(off_t offset, int whence)
		{
			off_t tmp = lseek(_f, offset, whence);
			if (tmp == (off_t) -1)
				throw Exception(errno);
			return tmp;
		}
		virtual void Flush()
		{
		}
		inline Int GetFlags()
		{
			return fcntl(_f, F_GETFL, 0);
		}
		inline void SetFlags(Int f)
		{
			if (fcntl(_f, F_SETFL, f) < 0)
				throw Exception(errno, "could not set file flags");
		}
	};
	class FileStream: public Stream
	{
	public:
		File f;
		FileStream(File f);
		virtual ~FileStream();
		virtual int Read(Buffer buf);
		virtual void Write(Buffer buf);
		virtual void Flush();
		virtual void Close();
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
		FileStream in, out;
		StandardStream() :
				in(File(0)), out(File(1))
		{
		}
		virtual int Read(Buffer buf)
		{
			return in.Read(buf);
		}
		virtual void Write(Buffer buf)
		{
			out.Write(buf);
		}
		virtual void Flush()
		{
			out.Flush();
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
		virtual int Read(Buffer buf)
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
		virtual void Write(Buffer buf)
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
			}
			else throw NotSupportedException();
		}
		virtual Cap Capabilities()
		{	return c;}
	};
#endif
	class StreamReaderWriter: public Stream
	{
	public:
		Stream* s;
		void* buf;
		StringBuilder wbuf;
		int wbuffersize;
		int max_wbuffer_copy;
		int buf_size;
		int buf_index;
		int buf_length;
		StreamReaderWriter(Stream& s, int rbuffersize = 4096, int wbuffersize = 4096);
		virtual ~StreamReaderWriter();
		virtual int Read(Buffer buf);
		virtual int Read(StringBuilder& buf, int length);
		virtual int Read(StringBuilder& buf, const char* delimitors, int delimitor_count);
		//virtual int read(Stream *buf,int length);

		//skips over any extra delimitors
		virtual int Read(Stream& buf, const char* delimitors, int delimitor_count);
		inline Short ReadByte()
		{
			if (buf_length <= 0)
			{
				Buffer tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0)
					return -1;
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
		virtual int Read(Stream& buf, const STRING* delimitors, int delimitor_count,
				int* delim_index = NULL);

		//delimitors have to be 1, 2, 4, or 8 bytes
		int fast_readline(Stream& buf);
		virtual int ReadLine(Stream& buf);

		virtual void Write(Buffer buf);
		virtual void Flush();
		virtual void Close();
		void do_flush()
		{
			if (wbuf.length <= 0)
				return;
			s->Write(wbuf.ToBuffer());
			wbuf.Clear();
		}
		inline void flush_if_full(int space = 0)
		{
			if (wbuf.length > (wbuffersize - space))
				do_flush();
		}
		void Write(void* buf, int len)
		{
			if (len > max_wbuffer_copy)
			{
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
	inline Int operator>>(Stream& s, Buffer& b)
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
	template<class S, class X> inline S& operator<<(S& os, const vector<X>& dt)
	{
		os << "[";
		bool notfirst = false;
		for (typename vector<X>::const_iterator it = dt.begin(); it != dt.end(); it++)
			(notfirst || !(notfirst = true)) ? os << ", " << (*it) : os << (*it);
		os << "]";
		return os;
	}

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
		if (Capacity >= c)
			return;
		int tmp = this->Capacity;
		while (tmp < c)
		{
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
			if (__def_BufferManager == NULL
			)
				__def_BufferManager = new BufferManager();
			return __def_BufferManager;
		}
		ArrayList<Buffer> l;
		int maxSpare, bufferSize;
		BufferManager(int bufferSize = 65536, int maxSpare = 10) :
				l(maxSpare > 32 ? 32 : maxSpare)
		{
			this->maxSpare = maxSpare;
			this->bufferSize = bufferSize;
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
			if (l.Length <= 0)
				return Buffer(bufferSize);
			else
				return l.array[--l.Length];
		}
		void Return(Buffer& b)
		{
			if (l.Length < maxSpare)
				l.Append(b);
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
		Int size;
		Int objsize;
		Int __wrap;
		Int s1, s2, e1, e2;
		BitArray b;
		CircularQueue(Int size, Int objsize = 1) :
				b(size), size(size), objsize(objsize), __wrap(size * 2), s1(0), s2(0), e1(0), e2(0)
		{
			array = new T[size * objsize];}
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
				if (i >= size || i < 0)
				throw new OutOfRangeException("CircularQueue::GetPointer() out of range");
				return array[i*objsize]; //__intwrap(i,size);
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
	typedef struct ucontext sig_ucontext_t;

	class Util_c
	{
	public:

		int _argc;
		char** _argv;
		string GetDirFromPath(const string path)
		{
			Int i = path.rfind("/");
			if (i < 0)
				return string();
			return path.substr(0, i + 1);
		}
		string GetProgramPath()
		{
			char buf[256];
			Int i = readlink("/proc/self/exe", buf, sizeof(buf));
			if (i < 0)
				throw Exception(errno);
			return string(buf, i);
		}
		void ChDir(string dir)
		{
			if (chdir(dir.c_str()) < 0)
				throw Exception(errno);
		}
		void RestartOnCrash(int argc, char** argv)
		{
			_argc = argc;
			_argv = argv;
			pid_t pid = fork();
			if (pid == 0)
			{
				return;
			}
			else if (pid < 0)
			{
				throw Exception(errno);
			}
		}
#if __x86_64__
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
#endif
		void SetHandlers()
		{
#if __x86_64__
			struct sigaction sigact;
			sigact.sa_sigaction = &Util_c::crit_err_hdlr;
			sigact.sa_flags = SA_RESTART | SA_SIGINFO;
			sigaction(SIGSEGV, &sigact, (struct sigaction *) NULL);
			sigaction(SIGABRT, &sigact, (struct sigaction *) NULL);
			sigaction(SIGFPE, &sigact, (struct sigaction *) NULL);
#endif
		}
	};
	extern Util_c Util;
	template<class T> inline T modulus(T number, T modulus)
	{
		T result = number % modulus;
		if (result < 0)
			result += modulus;
		return result;
	}
}
#endif
