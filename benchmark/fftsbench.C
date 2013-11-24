#include <stdio.h>
#include <math.h>
#include "benchmark.H"
#include <ffts/ffts.h>

typedef float fft_float;
typedef float fft_complex[2];

class FFTBench: public Benchmark
{
public:
	int fftsize;
	int iters;
	fft_float* globalData;
	struct threadData
	{
		fft_float* data;
		fft_complex* data_c;
		ffts_plan_t *p1,*p2;
	};
	vector<threadData> threads;
	
	void* fft_alloc(int sz) {
		return aligned_alloc(128,sz);
	}
	FFTBench(int fftsize, int iters):fftsize(fftsize),iters(iters) {
		globalData=(fft_float*)fft_alloc(fftsize*sizeof(fft_float));
		for(int i=0;i<fftsize;i++) {
			globalData[i]=drand48();
		}
	}
	inline int complexSize() {
		return fftsize/2+1;
	}
	void prepareThreads(int t) {
		threads.resize(t);
		for(int i=0;i<t;i++) {
			threads[i].data=(fft_float*)fft_alloc(fftsize*sizeof(fft_float));
			threads[i].data_c=(fft_complex*)fft_alloc(complexSize()*sizeof(fft_complex));
			threads[i].p1 = ffts_init_1d_real(fftsize,-1);
			threads[i].p2 = ffts_init_1d_real(fftsize,1);
		}
	}
	void destroyThreads() {
		for(int i=0;i<(int)threads.size();i++) {
			free(threads[i].data);
			free(threads[i].data_c);
			ffts_free(threads[i].p1);
			ffts_free(threads[i].p2);
		}
	}
	void doRun(BenchmarkThread& th) {
		threadData& d=threads[th.threadIndex];
		//double freq=1./2000;	//periods per sample
		th.beginTiming();
		for(int i=0;i<iters;i++) {
			memcpy(d.data,globalData,sizeof(fft_float)*fftsize);
			ffts_execute(d.p1,d.data,d.data_c);
			double cs1=1.d/(double)complexSize();
			for(int x=0;x<complexSize();x++) {
				d.data_c[x][0]*=double(x)*cs1;
				d.data_c[x][1]*=double(x)*cs1;
			}
			ffts_execute(d.p2,d.data_c,d.data);
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
	br.runDefaultTests(fftb,"FFTS");
}

