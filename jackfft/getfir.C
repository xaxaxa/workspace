#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fftfilter.C"

using namespace std;
int srate;
template<class T>
T* jackfft_alloc(int c) {
	return (T*)fftw_malloc(c*sizeof(T));
}
void jackfft_free(void* v) {
	fftw_free(v);
}

int writeAll(int fd, const void* b, int len) {
	char* buf=(char*)b;
	int br=0;
	while(br<len) {
		int r=write(fd,buf+br,len-br);
		if(r<=0) break;
		br+=r;
	}
	return br;
}

void load_jfft(int file, double* coeff, int coeffs) {
	struct {
		double freq; double val;
	} buf[1024];
	int i1=0,br;
	double last_v=0.5;
	while((br=read(file,buf,sizeof(buf)))>0) {
		br/=sizeof(*buf);
		for(int x=0;x<br;x++) {
			int i2=(int)round(((double)buf[x].freq/(srate/2))*coeffs);
			if(i2>coeffs) goto break1;
			for(int i=i1;i<i2;i++)
				coeff[i]=last_v*(i2-i)/(i2-i1)+buf[x].val*(i-i1)/(i2-i1);
			i1=i2;
			last_v=buf[x].val;
		}
	}
break1:
	for(int i=i1;i<coeffs;i++) coeff[i]=last_v;
}

int main (int argc, char *argv[])
{
	if(argc<3)
	{
		cerr << "usage: " << argv[0] << " file.jfft srate size" << endl;
		return 0;
	}
	int f=open(argv[1],O_RDONLY);
	if(f<0) {
		perror("open");
		return 1;
	}
	srate=atoi(argv[2]);
	int os=atoi(argv[3]);
	
	jackfft_float* coeff_t=jackfft_alloc<jackfft_float>(os);
	double* coeff=jackfft_alloc<double>((os)/2+1);
	jackfft_complex* coeff1=jackfft_alloc<jackfft_complex>((os)/2+1);
	load_jfft(f,coeff,(os)/2+1);
	close(f);
	fftw_plan tmpp1=fftw_plan_dft_c2r_1d(os,coeff1,coeff_t,0);
	for(int i=0;i<(os)/2+1;i++) {
		coeff1[i][0]=coeff[i]/(os);
		coeff1[i][1]=0;
	}
	jackfft_free(coeff);
	fftw_execute(tmpp1);
	fftw_destroy_plan(tmpp1);
	jackfft_free(coeff1);
	for(int i=0;i<os/2;i++) {
		double tmp=coeff_t[i];
		coeff_t[i]=coeff_t[os/2+i];
		coeff_t[os/2+i]=tmp;
	}
	//writeAll(1,coeff_t,sizeof(*coeff_t)*os);
	for(int i=0;i<os;i++)
		printf("%f\n",coeff_t[i]);
	return 0;
}
