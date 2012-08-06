#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	size_t bs,br;
	if(argc<2)
	{printf("usage: %s buffersize\n",argv[0]); return;}
	bs=atoi(argv[1]);
	char* ch=malloc(bs);
	while((br=read(0,ch,bs))>0)
		write(1,ch,br);
	return 0;
}

