#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <stdint.h>
#include <exception>
#include <stdexcept>
#include <sys/eventfd.h>
#include <sys/mman.h>



using namespace std;
using namespace CryptoPP;

#ifndef eprintf
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#endif
#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif

int digestSize;
byte* value;



struct scopeLock
{
	pthread_mutex_t* mutex;
	scopeLock(pthread_mutex_t& m):mutex(&m)
	{pthread_mutex_lock(mutex);}
	~scopeLock()
	{pthread_mutex_unlock(mutex);}
};
void initMutex(pthread_mutex_t& mutex) {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	int e;
	if((e=pthread_mutex_init(&mutex,&attr))!=0) {
		const char* err=strerror(e);
		throw runtime_error("mutex initialization failed: "+string(err));
	}
	pthread_mutexattr_destroy(&attr);
}
void destroyMutex(pthread_mutex_t& mutex) {
	pthread_mutex_destroy(&mutex);
}

//thread safe
class FDPool
{
public:
	vector<int32_t> fds;
	function<int32_t()> cb;
	pthread_mutex_t mutex;
	int32_t max;
	FDPool(const function<int32_t()>& cb, int32_t max=3): cb(cb), max(max) {
		initMutex(mutex);
	}
	~FDPool() {
		pthread_mutex_destroy(&mutex);
		for(size_t i=0;i<fds.size();i++) {
			close(fds[i]);
		}
	}
	int32_t get() {
		scopeLock l(mutex);
		if(fds.size()<=0) return cb();
		else {
			int32_t fd=fds[fds.size()-1];
			fds.resize(fds.size()-1);
			return fd;
		}
	}
	void put(int32_t fd) {
		scopeLock l(mutex);
		if((int32_t)fds.size()<max) fds.push_back(fd);
		else close(fd);
	}
};

struct IDLETHREAD
{
	function<void()> func;
	function<void()> cb;
	pthread_t thread;
	//void* arg;
	FDPool& fdp;
	int32_t efd;
	enum {
		none=0,
		invoke,
		kill
	} op;
	IDLETHREAD(FDPool& fdp): fdp(fdp), efd(fdp.get()),op(none) {}
	~IDLETHREAD() {
		fdp.put(efd);
	}
	void signal() {
		eventfd_t i=1;
		eventfd_write(efd, i);
	}
};
void* idleThread(void* v) {
	while(true) {
		{
			eventfd_t i;
			eventfd_read(((IDLETHREAD*)v)->efd, &i);
		}
		switch(((IDLETHREAD*)v)->op) {
		case IDLETHREAD::invoke:
			try {
				((IDLETHREAD*)v)->func();
			} catch(...) {}
			((IDLETHREAD*)v)->cb();
			break;
		case IDLETHREAD::kill:
			delete (IDLETHREAD*)v;
			return NULL;
		default:
			break;
		}
	}
}

//protected by mutex; all methods are thread-safe
class ThreadPool
{
public:
	FDPool fdp;
	vector<IDLETHREAD*> threads;
	//list<IDLETHREAD*> used_threads;
	pthread_mutex_t mutex;
	int32_t max;
	ThreadPool(int32_t max=3): fdp([]()->int32_t{return eventfd(0, EFD_SEMAPHORE);}), max(max) {
		initMutex(mutex);
	}
	~ThreadPool() {
		pthread_mutex_destroy(&mutex);
		for(size_t i=0;i<threads.size();i++) {
			threads[i]->op=IDLETHREAD::kill;
			threads[i]->signal();
		}
	}
	IDLETHREAD* create() {
		IDLETHREAD* tmp=new IDLETHREAD(fdp);
		if(pthread_create(&tmp->thread,NULL,idleThread,(void*)tmp)==0)
			return tmp;
		else {
			delete tmp;
			return NULL;
		}
	}
	IDLETHREAD* get() {
		size_t threadcount;
		{
			scopeLock l(mutex);
			threadcount=threads.size();
		}
		if(threadcount<=0) return create();
		else {
			scopeLock l(mutex);
			IDLETHREAD* t=threads[threads.size()-1];
			threads.resize(threads.size()-1);
			return t;
		}
	}
	void put(IDLETHREAD* t) {
		bool b;
		{
			scopeLock l(mutex);
			if((b=((int32_t)threads.size()<max))) threads.push_back(t);
		}
		if(!b) {
			t->op=IDLETHREAD::kill;
			t->signal();
		}
	}
	IDLETHREAD* invoke(const function<void()>& func) {
		IDLETHREAD* t=get();
		//used_threads.push_back(t);
		t->cb=[this,t]() {
			put(t);
		};
		t->func=func;
		t->op=IDLETHREAD::invoke;
		t->signal();
		return t;
	}
};

//memory efficient string list
class packedList
{
public:
	string data;
	vector<int> index;
	const char* operator[](int i) {
		return data.data()+index[i];
	}
	int getItemLength(int i) {
		if((i+1)<(int)index.size())
			return index[i+1]-index[i];
		else return data.length()-index[i];
	}
	void append(const char* s, int len) {
		int a=data.length();
		data.append(s, len);
		index.push_back(a);
	}
	int length() {
		return (int)index.size();
	}
};
uint64_t tested=0;
void do_attempt(HashTransformation* hash_algo, const char* s, int len) {
	//eprintf("testing %s\n", s);
	byte tmp[digestSize];
	hash_algo->CalculateDigest(tmp, (const byte*)s, len);
	//bool b=hash_algo->VerifyDigest(value, (const byte*)s, len);
	if(unlikely(memcmp(tmp,value,digestSize)==0)) {
	//if(unlikely(b)) {
		eprintf("cracked!!! value: \n");
		write(1, s, len);
		eprintf("\n");
		exit(0);
	}
	//++tested;
	//if(tested%1000001 == 0)
	//	eprintf("tested %llu texts; last tested: %s\n",tested,s);
}

void attempt1(HashTransformation* hash_algo, char* s, int len) {
	do_attempt(hash_algo,s, len);
	//char s[len];
	//memcpy(s, s1, len);
	//all uppercase
	for(int i=0;i<len;i++)
		if(s[i]>=97 && s[i]<=122)s[i]-=(97-65);
	do_attempt(hash_algo,s, len);
	//all lowercase
	for(int i=0;i<len;i++)
		if(s[i]>=65 && s[i]<=90)s[i]+=(97-65);
	do_attempt(hash_algo,s, len);
	//first letter uppercase
	for(int i=0;i<len;i++)
		if(s[i]==' ' && (i+1)<len && s[i+1]>=97 && s[i+1]<=122)s[i+1]-=(97-65);
	if(s[0]>=97 && s[0]<=122)s[0]-=(97-65);
	do_attempt(hash_algo,s, len);
}

void attempt(HashTransformation* hash_algo, const char* s1, int len) {
	char s[len+3];
	memcpy(s, s1, len);
	s[len]='\r';
	s[len+1]='\n';
	s[len+2]='\0';
	attempt1(hash_algo,s, len+2);
	s[len]='\n';
	s[len+1]='\0';
	attempt1(hash_algo,s, len+1);
	s[len]='\r';
	attempt1(hash_algo,s, len+1);
	s[len]='\0';
	attempt1(hash_algo,s, len);
	
	//preserve only first and last word
	char* i1=(char*)memchr((char*)s1, ' ', len);
	char* i2=(char*)memrchr((char*)s1, ' ', len);
	if(i1!=NULL && i2!=NULL && i1<i2) {
		memcpy(s, s1, (i1-s1));
		s[i1-s1]=' ';
		memcpy(s+(i1-s1)+1, i2+1, s1+len-i2-1);
		int l=(i1-s1)+(s1+len-i2-1)+1;
		s[l]='\0';
		attempt1(hash_algo,s, l);
		s[l]='\r';
		s[l+1]='\0';
		attempt1(hash_algo,s, l+1);
		s[l]='\n';
		attempt1(hash_algo,s, l+1);
		s[l]='\r';
		s[l+1]='\n';
		s[l+2]='\0';
		attempt1(hash_algo,s, l+2);
	}
}
inline byte parseHexChar(char c) {
	if(c>=48 && c<=57) return (byte)c-48;
	if(c>=65 && c<=90) return (byte)c-65+10;
	if(c>=97 && c<=122) return (byte)c-97+10;
	return 0;
}
//len is the length of output
void parseHex(const char* s, byte* out, int len) {
	for(int i=0;i<len;i++) {
		out[i]=parseHexChar(s[i*2])*16+parseHexChar(s[i*2+1]);
	}
}

void attempt2(HashTransformation* hash_algo, const char* s1, int len) {
	//do_attempt(hash_algo,s1, len);
	char s[len];
	memcpy(s,s1,len);
	//first letter uppercase
	for(int i=0;i<len;i++)
		if(s[i]==' ' && (i+1)<len && s[i+1]>=97 && s[i+1]<=122)s[i+1]-=(97-65);
	if(s[0]>=97 && s[0]<=122)s[0]-=(97-65);
	do_attempt(hash_algo,s, len);
}
void attempt3(HashTransformation* hash_algo, const char* s1, int len) {
	static const int max_combos=1024;
	char s[len+1];
	s[len]='\0';
	memcpy(s, s1, len);
	int alpha_chars=0;
	//to uppercase
	for(int i=0;i<len;i++)
		if(s[i]>=97 && s[i]<=122) {
			s[i]-=(97-65);
			alpha_chars++;
		} else if(s[i]>=65 && s[i]<=90)
			alpha_chars++;
	int combos=pow(2,alpha_chars);
	if(combos>max_combos) return;
	for(int i=0;i<combos;i++) {
		do_attempt(hash_algo, s, len);
		
		for(int x=len-1;x>=0;x--) {
			if(s[x]>=65 && s[x]<=90) {
				s[x]+=(97-65);
				break;
			} else if(s[x]>=97 && s[x]<=122) {
				s[x]-=(97-65);
			}
		}
	}
}

struct batch
{
	packedList lnames;
};
void* alignedAlloc(int size) {
	long ps = sysconf(_SC_PAGESIZE);
	size=int(ceil(double(size)/double(ps))*ps);
	void* tmp=mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if(tmp==MAP_FAILED)tmp=NULL;
	return tmp;
}
int main(int argc, char** argv) {
	int batchSize=1000000;
	if(argc<3) {
		eprintf("usage: %s type hash\ntype: either sha1, sha256, or sha512\n",argv[0]);
		return 1;
	}
	function<HashTransformation*()> hash_ctor;
	if(strcmp(argv[1],"sha1")==0)
		hash_ctor=[](){return new SHA1();};
	else if(strcmp(argv[1],"sha256")==0)
		hash_ctor=[](){return new SHA256();};
	else if(strcmp(argv[1],"sha512")==0)
		hash_ctor=[](){return new SHA512();};
	else {
		eprintf("hash algorithm %s not recognized\n",argv[1]);
		return 1;
	}
	HashTransformation* tmp_hash=hash_ctor();
	digestSize=tmp_hash->DigestSize();
	delete tmp_hash;
	if(digestSize!=(int)strlen(argv[2])/2) {
		eprintf("hash value length wrong; should be %i bytes\n",digestSize);
		return 1;
	}
	value=(byte*)alignedAlloc(digestSize);
	if(value==NULL) {
		perror("alignedAlloc");
		return 2;
	}
	parseHex(argv[2], value, digestSize);
	
	//uncomment to read test cases from stdin
	/*
	tmp_hash=hash_ctor();
	int tested=0;
	while(true) {
		char* line=NULL;
		size_t n=0;
		ssize_t l=getline(&line, &n, stdin);
		if(l<=0) break;
		l--;
		attempt3(tmp_hash,line, l);
		++tested;
		if(tested%50001 == 0)
			eprintf("tested %llu texts; last tested: %s\n",tested,line);
		free(line);
	}
	//*/
	
	//uncomment for hard brute force
	/*
	int fnl,lnl;
	//char fn[256];
	//char ln[256];
	char* fn;
	char* ln;
	char name[512+2];
	int nl=2;
	while(true) {
		for(lnl=1;lnl<nl;lnl++) {
			fnl=nl-lnl;
			eprintf("testing fnl=%i lnl=%i\n", fnl, lnl);
			uint64_t fns=pow(26,fnl);
			uint64_t lns=pow(26,lnl);
			fn=name;
			ln=name+fnl+1;
			
			memset(fn, 'a', fnl);
			for(uint64_t i=0;i<fns;i++) {
				memset(ln, 'a', lnl);
				name[fnl]=' ';
				for(uint64_t i=0;i<lns;i++) {
					//memcpy(name,fn,fnl);
					//name[fnl]=' ';
					//memcpy(name+fnl+1,ln,lnl);
					attempt2(name,fnl+lnl+1);
					
					ln[lnl-1]++;
					for(int i=lnl-1;i>=0;i--)
						if(unlikely(ln[i]>'z')) {
							ln[i-1]++;
							ln[i]='a';
						}
				}
				fn[fnl-1]++;
				for(int i=fnl-1;i>=0;i--)
					if(unlikely(fn[i]>'z')) {
						fn[i-1]++;
						fn[i]='a';
					}
			}
			
		}
		nl++;
		//if(fnl>2 || lnl>2) break;
	}
	//*/
	//uncomment to read first/lastnames from files
	//*
	if(argc<5) {
		eprintf("usage: %s type hash firstnamefile lastnamefile [nthreads]\n",argv[0]);
		return 1;
	}
	int nthreads=1;
	if(argc>5) {
		nthreads=atoi(argv[5]);
	}
	ThreadPool tp;
	//read firstname list into ram
	packedList fnames;
	FILE* file=fopen(argv[3],"rb");
	if(file==NULL) {
		perror("fopen");
		return 2;
	}
	while(true) {
		char* line=NULL;
		size_t n=0;
		ssize_t l=getline(&line, &n, file);
		if(l<=0) break;
		l--;
		fnames.append(line, l);
		free(line);
	}
	fclose(file);
	batchSize/=fnames.length();
	file=fopen(argv[4],"rb");
	if(file==NULL) {
		perror("fopen");
		return 2;
	}
	int cur_threads=0;
	pthread_mutex_t mutex;
	initMutex(mutex);
	if(nthreads<=1) {
		while(true) {
			char* line=NULL;
			size_t n=0;
			ssize_t l=getline(&line, &n, file);
			//write(1,line,l);
			//break;
			
			if(l<=0) break;
			l--;
			int len1=fnames.length();
			for(int i=0;i<len1;i++) {
				int len2=fnames.getItemLength(i);
				char tmp[len2+1+l];
				memcpy(tmp,fnames[i],len2);
				tmp[len2]=' ';
				memcpy(tmp+len2+1,line,l);
				attempt2(hash_ctor(),tmp,len2+1+l);
			}
			free(line);
		}
	} else {
		bool ended=false;
		int efd=eventfd(0,EFD_CLOEXEC);
		//list<batch> current_tasks;
		
		while(!ended) {
			//retrieve one batch
			batch* current;
			current=new batch();
			for(int x=0;x<batchSize;x++) {
				char* line=NULL;
				size_t n=0;
				ssize_t l=getline(&line, &n, file);
				if(l<=0) {
					ended=true;
					break;
				}
				l--;
				current->lnames.append(line,l);
				free(line);
			}
			if(current->lnames.length()<=0) {
				delete current;
				break;
			}
			while(true) {
				{
					scopeLock lck(mutex);
					if(cur_threads<nthreads) {
						cur_threads++;
						break;
					}
				}
				eventfd_t efd_val;
				eventfd_read(efd,&efd_val);
			}
			tp.invoke([efd,&cur_threads,&fnames,&mutex,current,&hash_ctor]() {
				HashTransformation* hash_algo=hash_ctor();
				int itemcount=current->lnames.length();
				{
					int l=current->lnames.getItemLength(0);
					char tmp[l+1];
					memcpy(tmp,current->lnames[0],l);
					tmp[l]='\0';
					eprintf("submitted batch; first item: %s\n", tmp);
				}
				for(int z=0;z<itemcount;z++) {
					const char* line=current->lnames[z];
					int l=current->lnames.getItemLength(z);
					
					int len1=fnames.length();
					for(int i=0;i<len1;i++) {
						int len2=fnames.getItemLength(i);
						char tmp[len2+1+l];
						memcpy(tmp,fnames[i],len2);
						tmp[len2]=' ';
						memcpy(tmp+len2+1,line,l);
						attempt2(hash_algo,tmp,len2+1+l);
					}
				}
				{
					scopeLock lck(mutex);
					cur_threads--;
				}
				eventfd_write(efd, 1);
				delete current;
			});
			
		}
		while(true) {
			{
				scopeLock lck(mutex);
				if(cur_threads<=0) break;
			}
			eventfd_t efd_val;
			eventfd_read(efd,&efd_val);
		}
		exit(0);
	}
	//*/
}

