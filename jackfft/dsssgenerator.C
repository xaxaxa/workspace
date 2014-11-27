#include <iostream>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fftw3.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

typedef double jackfft_float;
typedef jackfft_float jackfft_complex[2];

int srate;

template<class T>
T* jackfft_alloc(int c) {
	T* tmp = (T*)fftw_malloc(c*sizeof(T));
	if(tmp==NULL) throw bad_alloc();
	return tmp;
}
void jackfft_free(void* v) {
	fftw_free(v);
}
string itoa(int i) {
	char buf[32];
	snprintf(buf,sizeof(buf),"%i",i);
	return string(buf);
}

static inline void mult(jackfft_complex& c1, jackfft_complex& c2, double r) {
	jackfft_float r1=c1[0]*r;
	jackfft_float i1=c1[1]*r;
	c1[0]=(r1*c2[0]-i1*c2[1]);
	c1[1]=(r1*c2[1]+i1*c2[0]);
}

int readAll(int fd, void* b, int len) {
	char* buf=(char*)b;
	int br=0;
	while(br<len) {
		int r=read(fd,buf+br,len-br);
		if(r<=0) break;
		br+=r;
	}
	return br;
}
int writeAll(int fd, void* b, int len) {
	char* buf=(char*)b;
	int br=0;
	while(br<len) {
		int r=write(fd,buf+br,len-br);
		if(r<=0) break;
		br+=r;
	}
	return br;
}

double theta=0;
//center frequency
double fc=9000;
int main(int argc, char** argv) {
	if(argc<4) {
		printf("usage: %s codeseq.pcm fractionalbandwidth data\nonly 64-bit float pcm files supported\n",argv[0]);
		return 1;
	}
	int fd,ss,fbw;
	{
		struct stat st;
		if(stat(argv[1],&st)!=0) {
			perror("stat"); return 2;
		}
		ss=st.st_size/sizeof(double);
	}
	fd=open(argv[1],O_RDONLY);
	if(fd<0) {
		perror("open"); return 2;
	}
	fbw=atoi(argv[2]);
	
	jackfft_float* coeff_t=jackfft_alloc<jackfft_float>(ss);
	jackfft_complex* tmpbuf=jackfft_alloc<jackfft_complex>(ss/2+1);
	
	fftw_plan tmpp1=fftw_plan_dft_r2c_1d(ss,coeff_t,tmpbuf,0);
	readAll(fd,coeff_t,ss*sizeof(jackfft_float));
	//upsample
	fftw_execute(tmpp1);
	fftw_destroy_plan(tmpp1);

	jackfft_complex* tmpbuf2=jackfft_alloc<jackfft_complex>(ss*fbw/2+1);
	jackfft_float* coeff2_t=jackfft_alloc<jackfft_float>(ss*fbw);
	
	tmpp1=fftw_plan_dft_c2r_1d(ss*fbw,tmpbuf2,coeff2_t,0);
	//memcpy(tmpbuf2,tmpbuf,sizeof(jackfft_complex)*(ss/2+1));
	for(int i=0;i<(ss/2+1);i++) {
		tmpbuf2[i][0]=tmpbuf[i][0];
		tmpbuf2[i][1]=tmpbuf[i][1];
	}
	for(int i=ss/2+1;i<(ss*fbw/2+1);i++) {
		tmpbuf2[i][0]=0;
		tmpbuf2[i][1]=0;
	}
	fftw_execute(tmpp1);
	fftw_destroy_plan(tmpp1);
	jackfft_free(coeff_t);
	jackfft_free(tmpbuf);
	jackfft_free(tmpbuf2);
	for(int i=0;i<ss*fbw/2;i++) {
		double tmp=coeff2_t[i];
		coeff2_t[i]=coeff2_t[ss*fbw/2+i];
		coeff2_t[ss*fbw/2+i]=tmp;
	}
	double aaaaa=1.0/sqrt(ss*fbw*ss);
	for(int i=0;i<ss*fbw;i++) {
		coeff2_t[i]*=aaaaa;
	}
	
	jackfft_float* outputbuf=jackfft_alloc<jackfft_float>(ss*fbw);
	char* data=argv[3];
	int l=strlen(data);
	for(int i=0;i<l;i++) {
		int a=(data[i]=='0'?-1:1);
		for(int i=0;i<ss*fbw;i++)
			outputbuf[i]=coeff2_t[i]*a*cos(theta+double(i)*2*M_PI*fc/44100.0);
		theta+=double(ss*fbw)*2*M_PI*fc/44100.0;
		writeAll(1,outputbuf,sizeof(jackfft_float)*ss*fbw);
	}
}
