//#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
	static const int s=1024*1024*16; //16MiB
	long long int allocated=0;
	while(1) {
		void* v=malloc(s);
		if(v<=NULL) {
			perror("error allocating memory");
			printf("total allocated: %lli MiB\n", allocated/1024/1024);
			return 0;
		}
		memset(v, 0xFF, s);
		allocated+=s;
	}
	return 0;
}

