#include <stdio.h>
#include <unistd.h>
#include <math.h>
inline double abs1(double d)
{
	return d<0?-d:d;
}
inline double logn(double n, double x)
{
	return log(x) / log(n);
}

int main(int argc, char** argv) {
	struct strct
	{
		double freq;
		double val;
	};
	strct buf[1024];
	
	int r;
	while((r=read(0,buf,sizeof(buf)))>0)
	{
		for(int sss=0;sss<(r/sizeof(strct));sss++) {
			if(buf[sss].freq==0)continue;
			double p=logn(2,buf[sss].freq)*16;
			int nearest_note=(int)round(p);
			double dist=abs1(p-nearest_note);
			buf[sss].val*=(0.5-dist);
		}
		write(1,buf,r);
	}
}
