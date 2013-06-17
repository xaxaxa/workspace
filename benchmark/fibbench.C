#include <stdio.h>
#include "benchmark.H"
int64_t fib(int n){
    if(n<2)
        return (1);
    else
        return (fib(n-2)+fib(n-1));
}

class FibBench: public Benchmark
{
public:
	int n,iters;
	FibBench(int n,int i):n(n),iters(i) {}
	void doRun(BenchmarkThread& th) override {
		int64_t res=0;
		th.beginTiming();
		for(int i=0;i<iters;i++)
			res+=fib(n);
		th.endTiming();
		write(-1,&res,sizeof(res));
	}
	double valueFunc(int64_t t, int64_t tCPU, void* v) override {
		return 1000000000/double(tCPU)*iters;
	}
	string unit() override {
		return "1/s";
	}
};
int main(int argc, char** argv) {
	if(argc<4) {
		printf("usage: %s n iterations/run runs\n",argv[0]);
		return 1;
	}
	FibBench b(atoi(argv[1]),atoi(argv[2]));
	BenchmarkRunner br;
	br.runs=atoi(argv[3]);
	br.runDefaultTests(b,"Fibonacci");
}

