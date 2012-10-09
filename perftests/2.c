#include <stdio.h>
#define iterations 300
#define bufsize (5*6*8*2000)
int a;
int main()
{
	int i,ii;
	long double asdf=0;
	for(i=0;i<iterations;i++) {
		char c[bufsize];
		for(ii=0;ii<bufsize-4;ii+=5) {
			c[ii]='a';
			c[ii+1]='b';
			c[ii+2]='c';
			c[ii+3]='d';
			c[ii+4]='e';
		}
		long double* ints=(long double*)c;
		for(ii=0;ii<bufsize/sizeof(long double)-2;ii+=3) {
			ints[ii+2]=ints[ii]*2+ints[ii+1]*3;
		}
		ints[0]=ints[bufsize/sizeof(long double)-2];
		ints[1]=ints[bufsize/sizeof(long double)-1];
		for(ii=0;ii<bufsize/sizeof(long double)-2;ii+=3) {
			ints[ii+2]=ints[ii]*2+ints[ii+1]*3;
		}
		asdf+=ints[bufsize/sizeof(long double)-1];
	}
	
	printf("%Lf\n",asdf);
}
