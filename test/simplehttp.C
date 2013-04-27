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


struct handler:public RGC::Object {
	serverThread& thr;
	CP::Poll& p;
	Socket s;
	newPersistentStreamReader sr;
	StringPool sp;
	//uint8_t buf[8192];
	//MemoryStream ms;
	CP::PoolAllocator<std::pair<const String,String> > alloc;
	typedef map<String, String, less<String>, CP::PoolAllocator<std::pair<const String,String> > > StringMap;
	StringMap headers;
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
		sr(8192),alloc(&sp),headers(less<String>(), alloc) {
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
			uint8_t* tmp = (uint8_t*) memchr(lineBuf, ':', lineBufLen);
			if (tmp == NULL || tmp == lineBuf) goto fail;
			uint8_t* tmp1 = tmp - 1;
			while (tmp1 > lineBuf && *tmp1 == ' ')
				tmp1--;
			String n { sp.add((const char*) lineBuf, tmp1 - lineBuf + 1), tmp1 - lineBuf + 1 };

			tmp1 = tmp + 1;
			while (tmp1 < (lineBuf + lineBufLen) && *tmp1 == ' ')
				tmp1++;
			String v { sp.add((const char*) tmp1, lineBuf + lineBufLen - tmp1), lineBuf + lineBufLen
					- tmp1 };
			headers.insert(make_pair(n, v));
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
