/*
 * page.H
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#ifndef PAGE_H_
#define PAGE_H_
#include <rgc.H>
#include <map>
#include <cpoll/cpoll.H>
#include "stringutils.H"
using namespace std;
namespace cppsp
{
	typedef CP::String String;
	static int ci_compare(String s1, String s2) {
		if (s1.length() > s2.length()) return 1;
		if (s1.length() < s2.length()) return -1;
		if (s1.length() == 0) return 0;
		char a, b;
		for (int i = 0; i < s1.length(); i++) {
			a = tolower(s1.data()[i]);
			b = tolower(s2.data()[i]);
			if (a < b) return -1;
			if (a > b) return 1;
		}
		return 0;
	}
	struct Header
	{
		String name;
		String value;
	};
	struct headerContainer
	{
		//pointer-to-pointer so that the real buffer can be relocated
		//(for example during MemoryStream reallocations due to resize)
		char** buffer;
		CP::StringPool* sp;
		struct item
		{
			int nameStart;
			int nameLength;
			int valueStart;
			int valueLength;
		};
		static const int bucketSize = 8;
		struct bucket
		{
			bucket* next;
			item items[bucketSize];
			int length;
		};

		struct iterator
		{
			headerContainer* cont;
			bucket* b;
			int i;
			void operator+=(int i) {
				this->i += i;
				while (this->i > bucketSize && b != NULL) {
					b = b->next;
					this->i -= bucketSize;
				}
				if (b != NULL && this->i >= b->length) b = NULL;
			}
			void operator++(int) {
				operator+=(1);
			}
			bool operator==(const iterator& other) {
				if (b == NULL && other.b == NULL) return true;
				return b == other.b && i == other.i;
			}
			bool operator!=(const iterator& other) {
				return !operator==(other);
			}
			Header operator*() {
				item& it = b->items[i];
				return { {(*cont->buffer)+it.nameStart,it.nameLength},
					{	(*cont->buffer)+it.valueStart,it.valueLength}};
			}
			item& get() {
				return b->items[i];
			}
		};
		bucket* _first = NULL;
		bucket* _last = NULL;
		headerContainer(char** buffer, CP::StringPool* sp) :
				buffer(buffer), sp(sp) {
		}
		void add(item it) {
			if (_last == NULL || _last->length >= bucketSize) addBucket();
			_last->items[_last->length] = it;
			_last->length++;
		}
		void addBucket() {
			bucket* b = (bucket*) sp->add(sizeof(bucket));
			b->next = NULL;
			b->length = 0;
			if (_last != NULL) _last->next = b;
			_last = b;
			if (_first == NULL) _first = b;
		}
		String operator[](String name) {
			char* tmp = (char*) *buffer;
			for (bucket* b = _first; b != NULL; b = b->next) {
				for (int i = 0; i < b->length; i++)
					if (ci_compare(name, { tmp + b->items[i].nameStart, b->items[i].nameLength }) == 0) return {tmp+b->items[i].valueStart,b->items[i].valueLength};
			}
			return {(char*)nullptr,0};
		}
		iterator find(String name) {
			char* tmp = (char*) *buffer;
			for (bucket* b = _first; b != NULL; b = b->next) {
				for (int i = 0; i < b->length; i++)
					if (ci_compare(name, { tmp + b->items[i].nameStart, b->items[i].nameLength }) == 0) return {this,b,i};
			}
			return end();
		}
		iterator begin() {
			return {this,_first,0};
		}
		iterator end() {
			return {this,NULL,0};
		}
		void clear() {
			_first = _last = NULL;
		}
	};
	//different version that uses absolute pointers instead of relative positions
	struct headerContainer2
	{
		CP::StringPool* sp;
		struct item
		{
			const char* name;
			const char* value;
			int nameLength;
			int valueLength;
		};
		static const int bucketSize = 8;
		struct bucket
		{
			bucket* next;
			item items[bucketSize];
			int length;
		};
		struct iterator
		{
			bucket* b;
			int i;
			void operator+=(int i) {
				this->i += i;
				while (this->i > bucketSize && b != NULL) {
					b = b->next;
					this->i -= bucketSize;
				}
				if (b != NULL && this->i >= b->length) b = NULL;
			}
			void operator++(int) {
				operator+=(1);
			}
			bool operator==(const iterator& other) {
				if (b == NULL && other.b == NULL) return true;
				return b == other.b && i == other.i;
			}
			bool operator!=(const iterator& other) {
				return !operator==(other);
			}
			Header operator*() {
				item& it = b->items[i];
				return { {it.name,it.nameLength}, {it.value,it.valueLength}};
			}
			item& get() {
				return b->items[i];
			}
		};
		bucket* _first = NULL;
		bucket* _last = NULL;
		headerContainer2(CP::StringPool* sp) :
				sp(sp) {
		}
		void add(item it) {
			if (_last == NULL || _last->length >= bucketSize) addBucket();
			_last->items[_last->length] = it;
			_last->length++;
		}
		void add(String name, String value) {
			add( { name.data(), value.data(), name.length(), value.length() });
		}
		void addCopy(String name, String value) {
			name = sp->addString(name);
			value = sp->addString(value);
			add( { name.data(), value.data(), name.length(), value.length() });
		}
		void addBucket() {
			bucket* b = (bucket*) sp->add(sizeof(bucket));
			b->next = NULL;
			b->length = 0;
			if (_last != NULL) _last->next = b;
			_last = b;
			if (_first == NULL) _first = b;
		}
		String operator[](String name) {
			for (bucket* b = _first; b != NULL; b = b->next) {
				for (int i = 0; i < b->length; i++)
					if (ci_compare(name, { b->items[i].name, b->items[i].nameLength }) == 0) return {b->items[i].value,b->items[i].valueLength};
			}
			return {(char*)nullptr,0};
		}
		iterator find(String name) {
			for (bucket* b = _first; b != NULL; b = b->next) {
				for (int i = 0; i < b->length; i++)
					if (ci_compare(name, { b->items[i].name, b->items[i].nameLength }) == 0) return {b,i};
			}
			return end();
		}
		iterator begin() {
			return {_first,0};
		}
		iterator end() {
			return {NULL,0};
		}
		void clear() {
			_first = _last = NULL;
		}
	};
	class Request: public RGC::Object
	{
	public:
		Request(CP::Stream& inp, CP::StringPool* sp);
		virtual ~Request();
		RGC::Ref<CP::Stream> inputStream;
		CP::StreamReader input;
		CP::StringPool* sp; //may be used to store headers, querystrings, and POST data
		typedef map<String, String, less<String>, CP::PoolAllocator<std::pair<const String, String> > > StringMap;
		CP::PoolAllocator<std::pair<const String, String> > alloc;
		headerContainer2 headers;
		StringMap queryString;
		StringMap form;
		String method;
		String path;
		//string httpVersion;
		union
		{
			struct
			{
				DelegateBase<void(Request&)> cb;
				CP::MemoryStream* ms;
				char ms1[sizeof(CP::MemoryStream)];
				int tmp_i;
			} _readPOST;
		};

		//read POST data; headers are always read before creating the Request object
		virtual void readPost(Delegate<void(Request&)> cb);

		void _readCB(int i);
		virtual void reset();
	};
	class Response: public RGC::Object
	{
	public:
		typedef Delegate<void(Response&)> Callback;
		Response(CP::Stream& out, CP::StringPool* sp);
		RGC::Ref<CP::Stream> outputStream;
		CP::MemoryStream buffer;
		CP::StreamWriter output;
		Callback _cb;
		CP::StringPool* sp;
		headerContainer2 headers;
		String statusName;
		iovec iov[2];
		int statusCode;

		bool headersWritten;
		bool closed;
		bool sendChunked;
		/*virtual void doWriteHeaders();
		 void writeHeaders() {
		 if (!headersWritten) {
		 doWriteHeaders();
		 headersWritten = true;
		 }
		 }*/
		virtual void flush(Callback cb);
		virtual void clear();
		void addDefaultHeaders();
		template<class ... T>
		void write(T&&... a) {
			output.write(std::forward<T>(a)...);
		}
		void _writeCB(int r);
		virtual void reset();
	};
	class Page;
	class Server: public RGC::Object
	{
	public:
		virtual const char* rootDir()=0;
		//path is relative to rootDir
		virtual void loadPage(CP::Poll& p, String path, Delegate<void(Page*, exception* ex)> cb);
		virtual void loadPageFromFile(CP::Poll& p, String path,
				Delegate<void(Page*, exception* ex)> cb);
		//path is relative to rootDir
		virtual string mapPath(string path);
	};
	class Page: public RGC::Object
	{
	public:
		typedef Delegate<void(Page&)> Callback;
		Page();
		Page(Request& req, Response& resp, CP::StringPool* sp);
		RGC::Ref<Request> request;
		RGC::Ref<Response> response;
		Delegate<void(Page*, exception* ex)> pageCB;
		CP::Poll* poll;
		Server* server;
		Callback cb;
		String filePath;
		CP::StringPool* sp;
		bool doRender;
		bool doReadPost;
		inline void cancelRender() {
			doRender = false;
		}
		//pointer to mmap()ed address of original cppsp file
		const uint8_t* __stringTable;
		void __writeStringTable(int i, int len);
		string mapPath(string path);
		string mapRelativePath(string path);
		//path is relative to current page
		void loadNestedPage(string path, Delegate<void(Page*, exception* ex)> cb);
		//path is absolute
		void loadNestedPageFromFile(string path, Delegate<void(Page*, exception* ex)> cb);
		virtual void handleRequest(Callback cb = nullptr);
		virtual void processRequest();
		//each page goes through these operations (in order): init, [readPost], load, render, flush, finalize

		virtual void init(); //to be overridden by base class; called before load() is called, and
									//must call initCB() or cancelLoad() after it's done
		virtual void doInit();
		virtual void initCB();
		virtual void cancelLoad(exception* ex = NULL);
		virtual void load(); //to be overridden by derived class; called before headers are written
		virtual void render(CP::StreamWriter& out);
		virtual void flush();
		virtual void flushCB();
		virtual void finalize();
		virtual void finalizeCB();
		virtual ~Page();

		void _readPOSTCB(Request& r);
		void _flushCB(Response& r);
		void _pageCB(Page* p, exception* ex);
	};

} /* namespace cppsp */
#endif /* PAGE_H_ */
