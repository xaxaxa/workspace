#include <linux/aio_abi.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stropts.h>
#include <linux/fs.h>
#include <time.h>

typedef int64_t s64;
typedef uint64_t u64;
typedef uint8_t u8;
typedef long long signed int lls;
typedef long long unsigned int llu;


int _checkError(int r, const char* fn) {
	if(r<0) {
		perror(fn);
		exit(1);
	}
	return r;
}
#define ADDQUOTE2(x) #x
#define ADDQUOTE(x) ADDQUOTE2(x)
#define checkError(X) _checkError(X,"line " ADDQUOTE(__LINE__) ": " #X)

//range: [min,max)
s64 randRange(s64 min, s64 max) {
	return s64(drand48()*(max-min))+min;
}
inline int io_setup(unsigned nr, aio_context_t *ctxp) {
	return syscall(__NR_io_setup, nr, ctxp);
}
inline int io_destroy(aio_context_t ctx) {
	return syscall(__NR_io_destroy, ctx);
}
inline int io_submit(aio_context_t ctx, long nr,  struct iocb **iocbpp) {
	return syscall(__NR_io_submit, ctx, nr, iocbpp);
}
inline int io_getevents(aio_context_t ctx, long min_nr, long max_nr,
		struct io_event *events, struct timespec *timeout) {
	return syscall(__NR_io_getevents, ctx, min_nr, max_nr, events, timeout);
}

u8** bufferPool;
int bufferPoolLength;


void submitReads(aio_context_t ctx,int fd,int blockSize,s64 fileSize,int count) {
	iocb cbs[count];
	iocb* cbs_[count];
	memset(cbs,0,sizeof(iocb)*count);
	for(int i=0;i<count;i++) {
		cbs_[i]=cbs+i;
		s64 offset=randRange(0,fileSize/blockSize)*blockSize;
		cbs[i].aio_lio_opcode=IOCB_CMD_PREAD;
		cbs[i].aio_fildes=fd;
		cbs[i].aio_buf=(u64)bufferPool[--bufferPoolLength];
		cbs[i].aio_nbytes=blockSize;
		cbs[i].aio_offset=offset;
		cbs[i].aio_data=cbs[i].aio_buf;
		//printf("fd=%i buf=%p offset=%lli nbytes=%i\n",fd,(void*)cbs[i].aio_buf,offset,blockSize);
	}
	checkError(io_submit(ctx,count,cbs_));
}
int doSyncReads(int fd,int blockSize,s64 fileSize,int count) {
	for(int i=0;i<count;i++) {
		s64 offset=randRange(0,fileSize/blockSize)*blockSize;
		checkError(pread(fd,bufferPool[0],blockSize,(off_t)offset));
	}
	return count;
}
extern int optind;
int main(int argc, char** argv) {
	int concurrency=1,requestSize=4096;
	s64 requestCount=1024;
	bool synchronous=false;
	while(true) {
		int opt;
		switch((opt=getopt(argc,argv,"?hc:n:s:S"))) {
			case '?':
			case 'h':
			print_usage:
				printf("usage: %s [-c concurrency] [-n requestCount] [-s requestSize] [-S] file\n"
				"-S: use synchronous I/O\n",argv[0]);
				return 1;
			case -1: goto _break;
			case 'c': concurrency=atoi(optarg); break;
			case 'n': requestCount=(s64)atoll(optarg); break;
			case 's': requestSize=atoi(optarg); break;
			case 'S': synchronous=true; break;
		}
	}
_break:
	if(optind>=argc) goto print_usage;
	int fd=checkError(open(argv[optind],O_RDONLY|O_DIRECT));
	s64 fileSize;
	struct stat st;
	checkError(stat(argv[optind],&st));
	if(S_ISBLK(st.st_mode)) {
		checkError(ioctl(fd, BLKGETSIZE64, &fileSize));
	} else fileSize=st.st_size;
	aio_context_t ctx=0;
	if(!synchronous) checkError(io_setup(concurrency,&ctx));
	int inTransit=0;
	io_event events[concurrency];
	s64 requestsDone=0;
	bufferPool=new u8*[concurrency];
	for(int i=0;i<concurrency;i++)
		bufferPool[i]=(u8*)aligned_alloc(requestSize,requestSize);
	bufferPoolLength=concurrency;
	
	srand48(time(NULL));
	timespec time1,time2;
	checkError(clock_gettime(CLOCK_MONOTONIC,&time1));
	if(synchronous) {
		if(concurrency!=1) {
			printf("concurrency other than 1 is not supported in synchronous mode\n");
			return 1;
		}
		requestsDone=doSyncReads(fd,requestSize,fileSize,requestCount);
	} else {
		while(true) {
			int tmp=concurrency-inTransit;
			if(requestCount-requestsDone-inTransit<tmp)tmp=int(requestCount-requestsDone-inTransit);
			if(tmp>0) submitReads(ctx,fd,requestSize,fileSize,tmp);
			else if(inTransit<=0) break;
			inTransit+=tmp;
			int n=checkError(io_getevents(ctx,1,concurrency,events,NULL));
			for(int i=0;i<n;i++) {
				bufferPool[bufferPoolLength++]=(u8*)events[i].data;
			}
			requestsDone+=n;
			inTransit-=n;
		}
	}
	checkError(clock_gettime(CLOCK_MONOTONIC,&time2));
	double seconds=(time2.tv_sec-time1.tv_sec)+double(time2.tv_nsec-time1.tv_nsec)/1000000000;
	printf("%.2f seconds, %lli requests, %.2f requests/sec\n",seconds,requestsDone,double(requestsDone)/seconds);
	return 0;
}
