#include <stdio.h>
#include <stdexcept>
#include <stdlib.h>
#include "benchmark.H"

class MemBench: public Benchmark
{
public:
	int n,iters;
	MemBench(int n,int i):n(n),iters(i) {}
	void doRun(BenchmarkThread& th) override {
		int64_t res=0;
		void* buf[2]={malloc(n),malloc(n)};
		if(buf[0]==NULL || buf[1]==NULL) throw bad_alloc();
		memset(buf[0],123,n);
		th.beginTiming();
		for(int i=0;i<iters;i++) {
			memcpy(buf[i%2],buf[(i+1)%2],n);
		}
		th.endTiming();
		
		write(-1,buf[0],1);	//make sure compiler doesn't optimize out the entire computation
		write(-1,buf[1],1);
		free(buf[0]);
		free(buf[1]);
	}
	double valueFunc(int64_t t, int64_t tCPU, void* v) override {
		return (1000000000./double(tCPU))*iters*n/1024./1024.;
	}
	string unit() override {
		return "MiB/s";
	}
};
int main(int argc, char** argv) {
	if(argc<4) {
		printf("usage: %s n iterations/run runs\n",argv[0]);
		return 1;
	}
	MemBench b(atoi(argv[1]),atoi(argv[2]));
	BenchmarkRunner br;
	br.runs=atoi(argv[3]);
	br.runDefaultTests(b,"Memcpy");
}

