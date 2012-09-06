//#include <memory.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	static const int s=1024*1024*16; //16MiB
	while(1) {
		void* v=malloc(s);
		memset(v, 0xFF, s);
	}
	return 0;
}

