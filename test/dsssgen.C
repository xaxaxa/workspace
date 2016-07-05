#include <iostream>
#include <jack/jack.h>
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
#include <stdexcept>
#include <assert.h>

#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/base64.h>
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>

using namespace std;
typedef int16_t s16;
typedef int32_t s32;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint8_t u8;
typedef uint64_t u64;

class autocorr {
public:
	int len;
	double* output;	//allocated and managed by this class; size is len

	fftw_plan fwdPlan,revPlan;
	//float array of waveform to calculate autocorrelation of;
	//also the array where results are stored
	double* tmpReal;
	//fft of tmpIn
	fftw_complex* tmpComplex;
	
	autocorr(int len): len(len) {
		tmpReal = fftw_alloc_real(len);
		tmpComplex = fftw_alloc_complex(len);
		if(tmpReal==NULL || tmpComplex==NULL) throw bad_alloc();
		fwdPlan = fftw_plan_dft_r2c_1d(len,tmpReal,tmpComplex,FFTW_MEASURE);
		revPlan = fftw_plan_dft_c2r_1d(len,tmpComplex,tmpReal,FFTW_MEASURE);
		output = tmpReal;
	}
	
	//assumes input is of len bytes; bytes should be either 1 or 0
	void perform(u8* input) {
		for(int i=0;i<len;i++)
			tmpReal[i]=(input[i]==0)?-1:1;
		fftw_execute(fwdPlan);
		for(int i=0;i<(len/2+1);i++) {
			fftw_complex& val=tmpComplex[i];
			double mag2=val[0]*val[0]+val[1]*val[1];
			val[0]=mag2/len;
			val[1]=0;
		}
		fftw_execute(revPlan);
	}
};

struct worker {
	pthread_t th;
	autocorr a;
	int iterations;
	u8* bestCode;
	double best;
	worker(int len): a(len),best(0) {
		bestCode=new u8[len+1];	//1 extra byte for null byte when converting to string
	}
	~worker() {
		delete[] bestCode;
	}
};

void* workerThread(void* param) {
	worker& w=*(worker*)param;
	autocorr& a(w.a);
	u8* bestCode = w.bestCode;
	double& best(w.best);
	int codeLen=a.len;
	
	int randomDev=open("/dev/urandom",O_RDONLY);
	if(randomDev<0) {
		perror("open(/dev/urandom)");
		exit(1);
	}
	u64 seed;
	assert(read(randomDev,&seed,sizeof(seed))==sizeof(seed));
	
	CryptoPP::AutoSeededRandomPool prng;
	
	
	drand48_data r;
	srand48_r((long)seed, &r);
	
	for(int i=0;i<w.iterations;i++) {
		//generate a random bit sequence
		u8 seq[codeLen];
		prng.GenerateBlock((byte*)seq, sizeof(seq) );
	recheck:
		int offset=0;
		for(int ii=0;ii<codeLen;ii++) {
			seq[ii]&=1;
			if(seq[ii]==0) offset--;
			else offset++;
		}
		int offsetA=offset<0?-offset:offset;
		for(int ii=0;ii<offsetA;ii++) {
			long r2;
			lrand48_r(&r,&r2);
			seq[r2%codeLen]=offset<0?1:0;
		}
		if(offsetA!=0) goto recheck;
		
		//calculate autocorrelation
		a.perform(seq);
		
		//calculate figure of merit (peak/average power ratio)
		double peak=a.output[0]*a.output[0];
		double avg=a.output[1]*a.output[1];
		double otherpeak=avg;
		for(int i=2;i<codeLen;i++) {
			double power=a.output[i]*a.output[i];
			avg+=power;
			if(power>otherpeak) otherpeak=power;
		}
		avg/=(codeLen-1);
		
		double par=peak/avg;
		double ppr=peak/otherpeak;
		double fom=ppr;
		if(fom>best) {
			best=fom;
			memcpy(bestCode,seq,codeLen);
		}
	}
	return NULL;
}

void printCode(u8* code, int len) {
	int maxline=64;
	for(int i=0;i<len;i+=maxline) {
		int l=len-i;
		if(l>maxline)l=maxline;
		string tmp((char*)code+i,l);
		u8* s=(u8*)tmp.data();
		for(int i=0;i<l;i++) {
			s+=(u8)'0';
		}
		printf("\"%s\" &\n",tmp.c_str());
	}
}

int main() {
	fftw_init_threads();
	int iterationsPerWorker=1000000;
	int codeLen=128;
	int nWorkers=8;
	
	worker* w[nWorkers];
	for(int i=0;i<nWorkers;i++) {
		w[i]=new worker(codeLen);
		w[i]->iterations=iterationsPerWorker;
	}
	
	printf("fftw initialized\n");
	
	for(int i=0;i<nWorkers;i++) {
		if(pthread_create(&w[i]->th, NULL, workerThread,w[i])<0) {
			perror("pthread_create");
			exit(1);
		}
	}
	
	void* retval;
	for(int i=0;i<nWorkers;i++)
		pthread_join(w[i]->th,&retval);
	
	int bestIndex=0;
	for(int i=1;i<nWorkers;i++) {
		if(w[i]->best>w[bestIndex]->best)
			bestIndex=i;
	}
	
	worker& best=*w[bestIndex];
	
	printf("center peak to highest other peak power ratio: %lf\n", best.best);
	
	/*
	best.a.perform(best.bestCode);
	for(int i=0;i<codeLen;i++) {
		printf("%lf\n",best.a.output[i]);
	}*/
	int offset=0;
	for(int i=0;i<codeLen;i++) {
		if(best.bestCode[i]==0) offset--;
		else offset++;
	}
	printf("dc offset: %d\n", offset);
	
	for(int i=0;i<codeLen;i++)
		best.bestCode[i]+=(u8)'0';
	printCode(best.bestCode,codeLen);
	//best.bestCode[codeLen]=0;
	//printf("%s\n",best.bestCode);
}
