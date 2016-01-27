#include <stdio.h>
#include <string>
#include <fftw3.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
using namespace std;
//most of the area resides in [-3,3]
double gaussian(double x) {
	return exp(-pow(x,2)/2);
}
//area resides in [0,1]
double gaussianWindow(double x) {
	return gaussian((x*2-1)*3);
}
int main() {
	//for(int i=0;i<=10;i++) printf("%lf\n",gaussianWindow(double(i)/10));
	
	int bs=1024*32;
	int sr=50000000;
	int freqMax=9000000;
	
	int len=0;
	string data;
	while(true) {
		data.resize(len+bs);
		int br=read(0,((char*)data.data())+len,bs);
		if(br<=0) break;
		len+=br;
	}
	data.resize(len);
	unsigned char* ch=(unsigned char*)data.data();
	
	double* in=(double*)fftw_malloc(len*sizeof(double));
	fftw_complex* out=(fftw_complex*)fftw_malloc((len/2+1)*sizeof(fftw_complex));
	assert(in!=NULL);
	assert(out!=NULL);
	
	fftw_plan p=fftw_plan_dft_r2c_1d(len,in,out,FFTW_ESTIMATE);
	assert(p!=NULL);
	fprintf(stderr,"fftw plan created\n");
	for(int i=0;i<len;i++) in[i]=double(ch[i]-127);//*gaussianWindow(double(i)/len);
	fftw_execute(p);
	
	//f is in kHz
	for(int f=0;f<freqMax/1000;f+=1) {
		int fEnd=f+1;
		int iS=int64_t(f)*1000*len/sr;
		int iE=int64_t(fEnd)*1000*len/sr;
		double max1=0;
		double sum=0;
		for(int i=iS;i<iE;i++) {
			double mag2=pow(out[i][0],2)+pow(out[i][1],2);
			if(mag2>max1) max1=mag2;
			sum+=mag2;
		}
		double dB=log10(max1)*10;
		printf("%d kHz (%d - %d): %lf\n",f,iS,iE,sum);
		//printf("%lf\n",round(dB));
	}
}
