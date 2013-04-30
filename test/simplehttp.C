#include <cpoll/cpoll.H>
#include <stdio.h>
#include <map>
#include <unistd.h>
#include <cpoll/statemachines.H>

using namespace CP;
String headers,content;

#define CACHELINE_SIZE 64

struct serverThread
{
	Socket* listensock;
	pthread_t thr;
	int threadid;
	serverThread() {}
	~serverThread() {}
} __attribute__((aligned(CACHELINE_SIZE)));

int ci_compare(String s1, String s2) {
	if(s1.length()>s2.length()) return 1;
	if(s1.length()<s2.length()) return -1;
	if(s1.length()==0) return 0;
	char a,b;
	for(int i=0;i<s1.length();i++) {
		a=tolower(s1.data()[i]);
		b=tolower(s2.data()[i]);
		if(a<b) return -1;
		if(a>b) return 1;
	}
	return 0;
}
struct headerContainer
{
	//pointer-to-pointer so that the real buffer can be relocated
	//(for example during MemoryStream reallocations due to resize)
	uint8_t** buffer;
	StringPool* sp;
	struct item {
		int nameStart;
		int nameLength;
		int valueStart;
		int valueLength;
	};
	static const int bucketSize=8;
	struct bucket {
		bucket* next;
		item items[bucketSize];
		int length;
	};
	struct iterator
	{
		bucket* b;
		int i;
		void operator+=(int i) {
			this->i+=i;
			while(this->i > bucketSize && b!=NULL) {
				b=b->next;
				this->i-=bucketSize;
			}
			if(b!=NULL && this->i > b->length) b=NULL;
		}
		void operator++() {
			operator+=(1);
		}
		bool operator==(const iterator& other) {
			if(b==NULL && other.b==NULL) return true;
			return b==other.b && i==other.i;
		}
		bool operator!=(const iterator& other) {
			return !operator==(other);
		}
	};
	bucket* _first=NULL;
	bucket* _last=NULL;
	headerContainer(uint8_t** buffer, StringPool* sp):buffer(buffer),sp(sp) {}
	void add(item it) {
		if(_last==NULL || _last->length>=bucketSize) addBucket();
		_last->items[_last->length]=it;
		_last->length++;
	}
	void addBucket() {
		bucket* b=(bucket*)sp->add(sizeof(bucket));
		b->next=NULL;
		b->length=0;
		if(_last!=NULL)_last->next=b;
		_last=b;
		if(_first==NULL)_first=b;
	}
	String operator[](String name) {
		char* tmp=(char*)*buffer;
		for(bucket* b=_first;b!=NULL;b=b->next) {
			for(int i=0;i<b->length;i++)
				if(ci_compare(name,{tmp+b->items[i].nameStart,b->items[i].nameLength})==0)
					return {tmp+b->items[i].valueStart,b->items[i].valueLength};
		}
		return {(char*)nullptr,0};
	}
	iterator begin() {
		return {_first,0};
	}
	iterator end() {
		return {NULL,0};
	}
	void clear() {
		_first=_last=NULL;
	}
};
//different version that uses absolute pointers instead of relative positions
struct headerContainer2
{
	StringPool* sp;
	struct item {
		const char* name;
		const char* value;
		int nameLength;
		int valueLength;
	};
	static const int bucketSize=8;
	struct bucket {
		bucket* next;
		item items[bucketSize];
		int length;
	};
	struct iterator
	{
		bucket* b;
		int i;
		void operator+=(int i) {
			this->i+=i;
			while(this->i > bucketSize && b!=NULL) {
				b=b->next;
				this->i-=bucketSize;
			}
			if(b!=NULL && this->i > b->length) b=NULL;
		}
		void operator++() {
			operator+=(1);
		}
		bool operator==(const iterator& other) {
			if(b==NULL && other.b==NULL) return true;
			return b==other.b && i==other.i;
		}
		bool operator!=(const iterator& other) {
			return !operator==(other);
		}
	};
	bucket* _first=NULL;
	bucket* _last=NULL;
	headerContainer2(StringPool* sp):sp(sp) {}
	void add(item it) {
		if(_last==NULL || _last->length>=bucketSize) addBucket();
		_last->items[_last->length]=it;
		_last->length++;
	}
	void add(String name, String value) {
		add({name.data(),value.data(),name.length(),value.length()});
	}
	void addCopy(String name, String value) {
		name=sp->addString(name);
		value=sp->addString(value);
		add({name.data(),value.data(),name.length(),value.length()});
	}
	void addBucket() {
		bucket* b=(bucket*)sp->add(sizeof(bucket));
		b->next=NULL;
		b->length=0;
		if(_last!=NULL)_last->next=b;
		_last=b;
		if(_first==NULL)_first=b;
	}
	String operator[](String name) {
		for(bucket* b=_first;b!=NULL;b=b->next) {
			for(int i=0;i<b->length;i++)
				if(ci_compare(name,{b->items[i].name,b->items[i].nameLength})==0)
					return {b->items[i].value,b->items[i].valueLength};
		}
		return {(char*)nullptr,0};
	}
	iterator begin() {
		return {_first,0};
	}
	iterator end() {
		return {NULL,0};
	}
	void clear() {
		_first=_last=NULL;
	}
};
struct handler:public RGC::Object {
	serverThread& thr;
	CP::Poll& p;
	Socket s;
	newPersistentStreamReader sr;
	StringPool sp;
	//uint8_t buf[8192];
	//MemoryStream ms;
	//CP::PoolAllocator<std::pair<const String,String> > alloc;
	//typedef map<String, String, less<String>, CP::PoolAllocator<std::pair<const String,String> > > StringMap;
	headerContainer headers;
	String path;
	String method;
	iovec iov[2];
	bool keepAlive;
	bool firstLine;
	void reset() {
		headers.clear();
		sp.clear();
		firstLine=true;
		keepAlive=true;
	}
	handler(serverThread& thr,CP::Poll& p,HANDLE s):thr(thr),p(p),
		s(s,thr.listensock->addressFamily,thr.listensock->type,thr.listensock->protocol),
		sr(8192),headers(&sr.buffer,&sp) {
		p.add(this->s);
		this->retain();
		sr.readUntilString("\r\n",2,true);
		startCycle();
	}
	void startCycle() {
		reset();
		if(process()) startRead();
	}
	void startRead() {
		String b=sr.beginPutData(4096);
		s.read(b.data(),b.length(),{&handler::readCB,this});
	}
	void readCB(int r) {
		if(r<=0) {
			destruct();
			return;
		}
		sr.endPutData(r);
		if(process()) startRead();
	}
	//returns whether or not to continue reading
	bool process() {
		newPersistentStreamReader::item it;
		while(sr.process(it)) {
			if(it.data.length()==0) {
				//process request
				handleRequest();
				return false; //break out of the readLine loop
			} else {
				if(!processLine({(char*)it.data.data(),it.data.length()}))
					return false;
			}
		}
		return true;
	}
	bool processLine(String l) {
		uint8_t* lineBuf = (uint8_t*)l.data();
		int lineBufLen = l.length();
		if (firstLine) {
			firstLine = false;
			uint8_t* tmp = (uint8_t*) memchr(lineBuf, ' ', lineBufLen);
			if (tmp == NULL) goto fail;
			method = sp.addString((char*) lineBuf, tmp - lineBuf);
			tmp++;
			if (lineBuf + lineBufLen - tmp <= 0) goto fail;
			uint8_t* tmp1 = (uint8_t*) memchr(tmp, ' ', lineBuf + lineBufLen - tmp);
			if (tmp1 == NULL) goto fail;
			const char* path = (const char*) tmp;
			int pathLen = tmp1 - tmp;
			if (pathLen <= 0) goto fail;
			this->path = sp.addString(path, pathLen);
		} else {
			headerContainer::item it;
			uint8_t* tmp = (uint8_t*) memchr(lineBuf, ':', lineBufLen);
			if (tmp == NULL || tmp == lineBuf) goto fail;
			uint8_t* tmp1 = tmp - 1;
			while (tmp1 > lineBuf && *tmp1 == ' ')
				tmp1--;
			it.nameStart=(int)(lineBuf-sr.buffer);
			it.nameLength=(int)(tmp1 - lineBuf + 1);

			tmp1 = tmp + 1;
			while (tmp1 < (lineBuf + lineBufLen) && *tmp1 == ' ')
				tmp1++;
			it.valueStart=(int)(tmp1-sr.buffer);
			it.valueLength=(int)(lineBuf + lineBufLen - tmp1);
			headers.add(it);
		}
		end:
		return true;
	fail:
		destruct();
		return false;
	}
	void handleRequest() {
		iov[0]={::headers.data(),(size_t)::headers.length()};
		iov[1]={::content.data(),(size_t)::content.length()};
		s.writevAll(iov,2,{&handler::writeCB,this});
	}
	void writeCB(int r) {
		startCycle();
	}
	~handler() {
	}
};
void* thread1(void* v) {
	serverThread* thr=(serverThread*)v;
	Poll p;
	MemoryPool handlerPool(sizeof(handler));
	struct {
		Poll& p;
		serverThread* thr;
		MemoryPool& handlerPool;
		void operator()(HANDLE sock) {
			//printf("thread %i: accepted socket: %p (%i)\n",thr->threadid,sock,sock->handle);
			handler* hdlr=new (handlerPool.alloc()) handler(*thr,p,sock);
			hdlr->allocator=&handlerPool;
		}
	} cb {p, thr, handlerPool};
	thr->listensock->repeatAcceptHandle(&cb);
	p.add(*thr->listensock);
	p.loop();
}
int main(int argc, char** argv) {
	if(argc<3) {
		printf("usage: %s host:port threads\n",argv[0]);
		return 1;
	}
	string listen(argv[1]);
	int i=(int)listen.find(':');
	if(i<0) throw runtime_error("expected \":\" in listen");
	Socket listensock;
	listensock.bind(listen.substr(0,i).c_str(),
		listen.substr(i + 1, listen.length() - i - 1).c_str(), AF_UNSPEC, SOCK_STREAM);
	listensock.listen(512);
	int threads=atoi(argv[2]);
	
	//generate content
	MemoryStream ms;
	{
		StreamWriter sw(ms);
		char* tmp=sw.beginWrite(100);
		for(int i=0;i<100;i++)tmp[i]='X';
		sw.endWrite(100);
	}
	content={(char*)ms.data(),ms.length()};
	MemoryStream ms2;
	{
		StreamWriter sw(ms2);
		sw.writeF("HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Length: %i\r\n\r\n",content.length());
	}
	headers={(char*)ms2.data(),ms2.length()};
	
	printf("starting %i threads\n",threads);
	serverThread* th=new serverThread[threads];
	for(int i=0;i<threads;i++) {
		Socket* tmps=new Socket(dup(listensock.handle), listensock.addressFamily,
			listensock.type, listensock.protocol);
		th[i].listensock=tmps;
		th[i].threadid=i+1;
		if (pthread_create(&th[i].thr, NULL, thread1, &th[i]) != 0) {
			throw runtime_error(strerror(errno));
		}
	}
	while(1)sleep(3600);
}
