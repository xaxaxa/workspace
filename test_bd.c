#define _GNU_SOURCE
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define BS (1024*1024)
double avg_time=0;
//char buffer[BS];
char* buffer;
int64_t do_read(int fd, int* br) //returns speed in bytes/sec
{
	struct timespec t,t2;
	clock_gettime(CLOCK_MONOTONIC, &t);
	*br=read(fd,buffer,BS);
	if(*br<0)
	{
		perror("error reading");
		return *br;
	}
	clock_gettime(CLOCK_MONOTONIC, &t2);
	int64_t nsec=(t2.tv_nsec-t.tv_nsec)+(t2.tv_sec-t.tv_sec)*1000000000;
	return ((int64_t)*br)*1000000000/nsec;
}
void* malloc_aligned(int size, int asize, void** orig_buf)//returned address may not point to the beginning of the allocated buffer.
{
//	pagesize=getpagesize();
	void* tmp;
	tmp=malloc(size+asize);
	if(tmp==NULL)return NULL;
	void* ret=(void*)((((long unsigned int)tmp+asize-1)/asize)*asize);
	if(orig_buf!=NULL)*orig_buf=tmp;
	return ret;
}
int64_t offset=0;
static void hdl (int sig, siginfo_t *siginfo, void *context)
{
	printf ("Received signal %i; Sending PID: %ld, UID: %ld\n", sig,
			(long)siginfo->si_pid, (long)siginfo->si_uid);
	printf("last good position: %lli MiB\n", ((long long)offset)/BS);
	if(sig!=SIGUSR1 && sig!=SIGUSR2 && sig!=SIGALRM) exit(0);
}
int main(int argc, char** argv)
{
	int i;
	if(argc<5)
	{
		printf("usage: %s blockdevice offset(MiB, int) min_speed(MiB/s, float) [ignore(MiB)] [-write] [-stats]\n", argv[0]);
		return 1;
	}
	
	struct sigaction act;
	memset (&act, '\0', sizeof(act));
	/* Use the sa_sigaction field because the handles has two additional parameters */
	act.sa_sigaction = &hdl;
	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_SIGINFO;
	int sigs[12]={SIGHUP,SIGINT,SIGQUIT,SIGILL,SIGABRT,SIGFPE,SIGSEGV,SIGPIPE,SIGALRM,SIGTERM,SIGUSR1,SIGUSR2};
	for(i=0;i<(sizeof(sigs)/sizeof(int));i++)
		sigaction(sigs[i], &act, NULL);
	
	
	buffer=malloc_aligned(BS,getpagesize(),NULL);
	char* dev=argv[1];
	offset=atoll(argv[2])*BS;
	double minspeed=atof(argv[3])*BS;
	int64_t ignore=atoll(argv[4])*BS;
	int fd=open(dev,O_RDONLY|O_DIRECT);
	if(fd<0)
	{perror("open"); return 4;}
	lseek(fd,offset,SEEK_SET);
	int ret=0;
	int64_t br1=0;
	while(1)
	{
		int br;
		int64_t tmp=do_read(fd,&br);
		if(tmp<0)
		{
			ret=3;
			break;
		}
		if(tmp==0)
		{
			printf("EOF reached.\n");
			break;
		}
		br1+=br;
		if(br1>ignore && tmp<minspeed)
		{
			printf("speed %lli MiB/s is slower than the minimum of %lli MiB/s; terminating\n", ((long long)tmp)/BS, (long long)(minspeed/(double)BS));
			ret=2;
			break;
		}
		//speed is ok; increment position
		offset+=br;
	}
	printf("last good position: %lli MiB\n", ((long long)offset)/BS);
	return ret;
}

