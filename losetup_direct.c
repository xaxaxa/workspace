#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("usage: %s file\n",argv[0]);
		return 1;
	}
	int fd=open(argv[1],O_RDWR|O_LARGEFILE|O_DIRECT);
	dup2(fd,10);
	execlp("losetup","losetup","-f","/proc/self/fd/10",NULL);
}

