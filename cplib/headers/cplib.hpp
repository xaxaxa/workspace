
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
		template<class T>inline NAME(RETVAL(T::*f)(__VA_ARGS__),void* obj)\
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
#define WARN(LEVEL,MSG) if(LEVEL<=WARNLEVEL)cout << MSG << endl;



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

	int RetainCount;//reference counter
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
	virtual void asdf()
	{

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
		init(code,strerror(code));
		//delete err;
	}
	inline Exception(std::string message)
	{
		init(-1,message);
	}
	inline Exception(int code, std::string message)
	{
		init(code,message);
	}
	inline Exception()
	{
		Code=0;
	}
};
class OutOfRangeException: public Exception
{
public:
	inline OutOfRangeException(): Exception()
	{
		//this();
	}
	inline OutOfRangeException(std::string message): Exception(message)
	{

	}
};
struct STRING
{
	char* c;
	int length;
	STRING(char* c, int length)
	{
		this->c=c;
		this->length=length;
	}
	STRING(char* c)
	{
		STRING(c,strlen(c));
	}
	STRING()
	{}
};
#ifdef __debug_print123
void __Buffer_bytes_inc(int i);
void __Buffer_bytes_dec(int i);
int __Buffer_bytes_get();
#endif
/*struct Buffer
{
public:
	void* buf;
private:
	void* __location;
public:
	int length;
	//bool destruct;
	struct BufferInfo
	{
		int RetainCount;
	};
	inline Buffer()
	{
		buf=NULL;
	}
	inline Buffer(void* buf, int length, Buffer* src = NULL)
	{
		this->buf = buf;
		this->length = length;
		this->__location = (src == NULL ? NULL : src->__location);
		//this->destruct=destruct;
		//Retain();
	}
	inline Buffer(int length)
	{
		if(length<=0)
		{
			this->__location=NULL;
			return;
		}
		this->__location = malloc(sizeof(BufferInfo) + length);
		BufferInfo *inf = (BufferInfo*) __location;
		inf->RetainCount = 1;
		this->buf = (void*) (((__uint8_t *) this->__location)
				+ sizeof(BufferInfo));
		this->length = length;
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
			if (buf != NULL)
			{
				free(buf);
				buf = NULL;
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
			buf = NULL;
#ifdef __debug_print123
			__Buffer_bytes_dec(length);
			dbgprint(length << " bytes freed(total " << __Buffer_bytes_get() << ")");
#endif
		}
	}

	inline Buffer& Retain()
	{
		if (__location != NULL&&buf!=NULL)
		{
			BufferInfo *inf = (BufferInfo*) __location;
			inf->RetainCount++;
			dbgprint("Buffer::Retain()");
		}
		return *this;
	}
	inline Buffer SubBuffer(int index, int length, bool retain=false)
	{
		if (index < 0 || length > this->length) throw Exception(
				"SubBuffer() out of range");
		//Buffer b((void*)((char*)buf+index),length,this,false,retain);
		Buffer b;
		b.buf = (void*) ((char*) buf + index);
		b.length = length;
		b.__location = this->__location;
		if(retain)this->Retain();
		return b;
	}
	inline void Release()
	{
		if (length<=0||buf==NULL)return;
		if (__location != NULL)
		{
			BufferInfo *inf = (BufferInfo*) __location;
			inf->RetainCount--;
			dbgprint("Buffer::Release()");
			if (inf->RetainCount == 0) Free();
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

class Buffer
{
public:
	boost::shared_array<Byte> pbuf;
	Byte* Data;
	bool IsRaw;
	Int Length;
	inline Buffer()
	{
		Data=NULL;IsRaw=true;Length=0;
	}
	Buffer(void* buf, int length)
	{
		this->Data = (Byte*)buf;
		this->Length = length;
		this->IsRaw=true;
	}
	Buffer(char* buf)
	{
		this->Data = (Byte*)buf;
		this->Length = strlen(buf);
		this->IsRaw=true;
	}
	Buffer(Byte* buf, int length)
	{
		this->Data = (Byte*)buf;
		this->Length = length;
		this->IsRaw=true;
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
		this->Length=length;
		if(length<=0)
		{
			this->Data=NULL;this->IsRaw=true;
			return;
		}
		this->Data = new Byte[length];
		this->pbuf=boost::shared_array<Byte>(this->Data);
		this->IsRaw=false;
	}
	inline Buffer SubBuffer(int index, int length) const
	{
		if (index < 0 || length > this->Length) throw Exception(
				"SubBuffer() out of range");
		if(IsRaw)
			return Buffer(Data+index,length);
		else return Buffer(Data+index,length,pbuf);
	}
	inline Buffer SubBuffer(int index) const
	{
		return SubBuffer(index,Length-index);
	}
	inline void Release()
	{
		if(IsRaw)throw Exception("Attempting to release a buffer initialized with a raw pointer. This can be dangerous.");
		pbuf.reset();
	}
};

template<class T> class __event: public Object
{
public:
	map<T,void*> handlers;
	typedef typename map<T,void*>::iterator iter;
	inline void* newiter()
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
	}
	inline void addhandler(T delegate)
	{
		handlers.insert(pair<T,void*>(delegate,NULL));
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
		{void* gjfdjdsghddjh;\
		for(gjfdjdsghddjh=(NAME).newiter();!((NAME).isiterend(gjfdjdsghddjh));(NAME).incrementiter(gjfdjdsghddjh))\
		{CALL((NAME).getitervalue(gjfdjdsghddjh),__VA_ARGS__);}\
		(NAME).deleteiter(gjfdjdsghddjh);}
#define ADDHANDLER(NAME,DEL) (NAME).addhandler(DEL)
#define REMOVEHANDLER(NAME,DEL) (NAME).removehandler(DEL)
class Stream: public Object
{
public:
	virtual int Read(Buffer buf)=0;
	virtual void Write(Buffer buf)=0;
	virtual void Flush()=0;
	virtual void Close()=0;

	//typedef boost::function<void (void*,Stream*)> Callback;
	FUNCTION_DECLARE(Callback,void,Stream*);
	int __tmp;
	virtual void BeginRead(Buffer buf, Callback cb)
	{
		__tmp = Read(buf);
		FUNCTION_CALL(cb,this);
	}
	virtual void BeginWrite(Buffer buf, Callback cb)
	{
		Write(buf);
		FUNCTION_CALL(cb,this);
	}
	virtual int EndRead()
	{
		return __tmp;
	}
	virtual void EndWrite()
	{

	}
};
class StringBuilder: public Stream
{
public:
	StringBuilder(int initsize = 4096);
	~StringBuilder();
	void *buf;
	int Capacity;
	int Length;
	void Append(Buffer buf);
	void Append(STRING buf);
	void Append(char* buf, int length);
	void Append(char* buf);
	void Append(const StringBuilder* s);
	void EnsureCapacity(int c);
	int CompareTo(Buffer buf);
	int CompareTo(const StringBuilder* sb);
	STRING ToString();
	Buffer ToBuffer();
	virtual int Read(Buffer buf);
	virtual void Write(Buffer buf);
	virtual void Flush();
	virtual void Close();
};

class FileStream: public Stream
{
public:
	FILE *f;
	FileStream(FILE *f);
	~FileStream();
	virtual int Read(Buffer buf);
	virtual void Write(Buffer buf);
	virtual void Flush();
	virtual void Close();
};
class StreamReader: public Stream
{
public:
	Stream* s;
	void* buf;
	int buf_size;
	int buf_index;
	int buf_length;
	StreamReader(Stream* s, int buffersize = 4096);
	~StreamReader();
	virtual int Read(Buffer buf);
	virtual int Read(StringBuilder& buf, int length);
	virtual int Read(StringBuilder& buf, const char* delimitors,
			int delimitor_count);
	//virtual int read(Stream *buf,int length);
	virtual int Read(Stream& buf, const char* delimitors,
			int delimitor_count);
	virtual int ReadLine(StringBuilder& buf);

	virtual void Write(Buffer buf);
	virtual void Flush();
	virtual void Close();
};
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
	{//WARNING: no runtime check
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
	{//WARNING: no runtime check
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
typedef BitArray32 BitArray;// 32 bit
#else
typedef BitArray64 BitArray;// 64 bit
#endif
template<class T> class ArrayList: public Object
{
public:
	ArrayList(int initsize = 32);
	~ArrayList();
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

	inline static BufferManager* GetDefault(){
		if(__def_BufferManager==NULL)
			__def_BufferManager=new BufferManager();
		return __def_BufferManager;
	}
	ArrayList<Buffer> l;
	int maxSpare, bufferSize;
	BufferManager(int bufferSize=65536, int maxSpare=10):l(maxSpare>32?32:maxSpare)
	{
		this->maxSpare=maxSpare;
		this->bufferSize=bufferSize;
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
		if(l.Length<=0) return Buffer(bufferSize);
		else return l.array[--l.Length];
	}
	void Return(Buffer& b)
	{
		if(l.Length<maxSpare) l.Append(b);
		//else b->Release();
	}
};
template<class T> class CircularQueue: public Object
{
public:
#define __intwrap1(var,max) if((var)>=(max))(var)-=(max);
#define __intwrap(var,max) ((var)>=(max)?(var)-(max):(var))
	T* array;
	int size;
	int __wrap;
	int s1, s2, e1, e2;
	BitArray b;
	CircularQueue(int size):b(size)
	{
		s1=0; s2=0; e1=0; e2=0;
		this->size=size;
		this->__wrap=size*2;
		array=new T[size];
	}
	~CircularQueue()
	{
		delete[] array;
	}
	inline int __getlength(int i1, int i2, int wrap)
	{
		return (i2<i1?i2+wrap:i2)-i1;
	}
	inline T& GetPointer(int i)
	{
		__intwrap1(i,size);
		if(i>=size||i<0)throw new OutOfRangeException("CircularQueue::GetPointer() out of range");
		return array[i];//__intwrap(i,size);
	}
	inline bool CanAppend()
	{
		return __getlength(s1,e2,__wrap)<size;
	}
	int BeginAppend()
	{
		if(__getlength(s1,e2,__wrap)>=size) return -1;
		int tmp=e2++;
		__intwrap1(e2,__wrap);
		b.Set(__intwrap(tmp,size),true);
		return tmp;
	}
	void EndAppend(int i)
	{
		if(i==e1)
		{
			do
			{
				e1++;
				__intwrap1(e1,__wrap);
			}while(__getlength(e1,e2,__wrap)>0 && !(b.Get(__intwrap(e1,size))));
		}
		else b.Set(__intwrap(i,size),false);
	}
	inline bool CanDequeue()
	{
		return __getlength(s2,e1,__wrap)>0;
	}
	int BeginDequeue()
	{
		if(__getlength(s2,e1,__wrap)<=0) return -1;
		int tmp=s2++;
		__intwrap1(s2,__wrap);
		b.Set(__intwrap(tmp,size),true);
		return tmp;
	}
	void EndDequeue(int i)
	{
		if(i==s1)
		{
			do
			{
				s1++;
				__intwrap1(s1,__wrap);
			}while(__getlength(s1,s2,__wrap)>0 && !(b.Get(__intwrap(s1,size))));
		}
		else b.Set(__intwrap(i,size),false);
	}
};
class PointerException: public Exception
{
public:
	inline PointerException(): Exception("segmentation fault")
	{
			//this();
	}
	inline PointerException(std::string message): Exception(message)
	{

	}
};


}
#endif
