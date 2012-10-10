#include <stdio.h>
#define iterations 30
#define bufsize (5*6*16)
int a;
int main()
{
	int i,ii;
	double asdf=0;
	for(i=0;i<iterations;i++) {
		char c[bufsize];
		for(ii=0;ii<bufsize-4;ii+=5) {
			c[ii]='a';
			c[ii+1]='b';
			c[ii+2]='c';
			c[ii+3]='d';
			c[ii+4]='e';
		}
		double* ints=(double*)c;
		ints[0]=123;
		ints[1]=345;
		for(ii=0;ii<bufsize/sizeof(double)-2;ii+=3) {
			ints[ii+2]=ints[ii]-ints[ii+1];
		}
		ints[0]=ints[bufsize/sizeof(long double)-2];
		ints[1]=ints[bufsize/sizeof(long double)-1];
		for(ii=0;ii<bufsize/sizeof(long double)-2;ii+=3) {
			ints[ii+2]=ints[ii]-ints[ii+1];
		}
		asdf+=ints[bufsize/sizeof(double)-1];
	}
	
	printf("%lf\n",asdf);
}
