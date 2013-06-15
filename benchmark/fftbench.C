#include <stdio.h>
#include <fftw3.h>
#include <math.h>
#include "benchmark.H"
class FFTBench
{
public:
	int fftsize;
	int iters,runs;
	struct threadData
	{
		double* data;
		fftw_complex* data_c;
		fftw_plan p;
	};
	vector<threadData> threads;
	void prepareThreads(int t) {
		threads.resize(t);
		for(int i=0;i<t;i++) {
			threads[i].data=(double*)fftw_malloc(fftsize*sizeof(double));
			threads[i].data_c=(fftw_complex*)fftw_malloc(fftsize*sizeof(fftw_complex));
			threads[i].p = fftw_plan_dft_r2c_1d(fftsize, threads[i].data,
				threads[i].data_c, FFTW_PATIENT|FFTW_PRESERVE_INPUT);
		}
	}
	void destroyThreads() {
		for(int i=0;i<(int)threads.size();i++) {
			fftw_free(threads[i].data);
			fftw_free(threads[i].data_c);
			fftw_destroy_plan(threads[i].p);
		}
	}
	void target(BenchmarkThread& th) {
		auto& d=threads[th.threadIndex];
		double freq=1./2000;	//periods per sample
		for(int i=0;i<fftsize;i++) {
			d.data[i]=double(rand())/RAND_MAX;
		}
		th.beginTiming();
		for(int i=0;i<iters;i++)
			fftw_execute(d.p);
		th.endTiming();
	}
	double valueFunc(int64_t t, int64_t tCPU, void* v) {
		return double(iters)/tCPU*1000000000;
	}
	void run(Benchmark& b) {
		b.runs=runs;
		b.prepareThreads={&FFTBench::prepareThreads,this};
		b.destroyThreads={&FFTBench::destroyThreads,this};
		b.valueFunc={&FFTBench::valueFunc,this};
		b.unit="FFTs/s";
		b.run({&FFTBench::target,this},"FFT");
	}
};
int main(int argc, char** argv) {
	if(argc<4) {
		printf("usage: %s fftsize iterations runs\n",argv[0]);
		return 1;
	}
	FFTBench fftb{atoi(argv[1]),atoi(argv[2]),atoi(argv[3])};
	Benchmark b;
	fftb.run(b);
	b.displayResults();
}

