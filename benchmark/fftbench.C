#include <stdio.h>
#include <fftw3.h>
#include <math.h>
#include "benchmark.H"

#ifdef USE_FFTWF
typedef float fft_float;
typedef fftwf_complex fft_complex;
#define FFTWFUNC(x) fftwf_ ## x
#else
typedef double fft_float;
typedef fftw_complex fft_complex;
#define FFTWFUNC(x) fftw_ ## x
#endif

class FFTBench: public Benchmark
{
public:
	bool inverse;
	int fftsize;
	int iters;
	fft_float* globalData;
	fft_complex* globalData_c;
	struct threadData
	{
		fft_float* data;
		fft_complex* data_c;
		FFTWFUNC(plan) p1,p2;
	};
	vector<threadData> threads;
	
	FFTBench(int fftsize, int iters):fftsize(fftsize),iters(iters) {
		globalData=(fft_float*)FFTWFUNC(malloc)(fftsize*sizeof(fft_float));
		globalData_c=(fft_complex*)FFTWFUNC(malloc)(complexSize()*sizeof(fft_complex));
		for(int i=0;i<fftsize;i++) {
			globalData[i]=drand48();
		}
		for(int i=0;i<complexSize();i++) {
			globalData_c[i][0]=drand48();
			globalData_c[i][1]=drand48();
		}
		inverse=false;
	}
	inline int complexSize() {
		return fftsize/2+1;
	}
	void prepareThreads(int t) {
		threads.resize(t);
		for(int i=0;i<t;i++) {
			threads[i].data=(fft_float*)FFTWFUNC(malloc)(fftsize*sizeof(fft_float));
			threads[i].data_c=(fft_complex*)FFTWFUNC(malloc)(complexSize()*sizeof(fft_complex));
			threads[i].p1 = FFTWFUNC(plan_dft_r2c_1d)(fftsize, threads[i].data,
				threads[i].data_c, FFTW_ESTIMATE);
			threads[i].p2 = FFTWFUNC(plan_dft_c2r_1d)(fftsize, threads[i].data_c,
				threads[i].data, FFTW_ESTIMATE);
		}
	}
	void destroyThreads() {
		for(int i=0;i<(int)threads.size();i++) {
			FFTWFUNC(free)(threads[i].data);
			FFTWFUNC(free)(threads[i].data_c);
			FFTWFUNC(destroy_plan)(threads[i].p1);
			FFTWFUNC(destroy_plan)(threads[i].p2);
		}
	}
	void doRun(BenchmarkThread& th) {
		threadData& d=threads[th.threadIndex];
		//double freq=1./2000;	//periods per sample
		th.beginTiming();
		if(inverse) {
			for(int i=0;i<iters;i++) {
				memcpy(d.data_c,globalData_c,sizeof(fft_complex)*complexSize());
				FFTWFUNC(execute)(d.p2);
			}
		} else {
			for(int i=0;i<iters;i++) {
				memcpy(d.data,globalData,sizeof(fft_float)*fftsize);
				FFTWFUNC(execute)(d.p1);
			}
		}
		th.endTiming();
	}
	double valueFunc(int64_t t, int64_t tCPU, void* v) {
		return double(iters)/tCPU*1000000000;
	}
	string unit() {
		return "FFTs/s";
	}
};
int main(int argc, char** argv) {
	if(argc<4) {
		printf("usage: %s fftsize iterations runs\n",argv[0]);
		return 1;
	}
	FFTBench fftb(atoi(argv[1]),atoi(argv[2]));
	BenchmarkRunner br;
	br.runs=atoi(argv[3]);
	br.runDefaultTests(fftb,"FFT");
	fftb.inverse=true;
	br.runDefaultTests(fftb,"inverse FFT");
}

