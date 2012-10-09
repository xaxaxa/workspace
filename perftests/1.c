#include <stdio.h>
#define iterations 30000
#define bufsize 300000
int a;
int main()
{
	int i,ii;
	int asdf=0;
	for(i=0;i<iterations;i++) {
		char c[bufsize];
		for(ii=0;ii<bufsize-4;ii+=5) {
			c[ii]='c';
			c[ii+1]='d';
			c[ii+2]='e';
			c[ii+3]='a';
			c[ii+4]='b';
		}
		int* ints=(int*)c;
		for(ii=0;ii<bufsize/sizeof(int)-2;ii+=3) {
			ints[ii+2]=ints[ii]*2+ints[ii+1]*3;
		}
		ints[0]=ints[bufsize/sizeof(int)-2];
		ints[1]=ints[bufsize/sizeof(int)-1];
		for(ii=0;ii<bufsize/sizeof(int)-2;ii+=3) {
			ints[ii+2]=ints[ii]*2+ints[ii+1]*3;
		}
		asdf+=ints[bufsize/sizeof(int)-1];
	}
	
	printf("%i\n",asdf);
}

