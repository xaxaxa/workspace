#include <delegate.H>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>
#include <string.h>
#include <vector>
#include <map>

using namespace std;

static inline long long timespec_to_ns(const struct timespec *tv)
{
	return ((long long) tv->tv_sec * 1000000000) +
		tv->tv_nsec;
}
int64_t operator-(const timespec& t1, const timespec& t2) {
	return timespec_to_ns(&t1)-timespec_to_ns(&t2);
}

class BenchmarkRunner;
class BenchmarkThread;
class Benchmark
{
public:
	virtual void prepareThreads(int threads) {}
	virtual void destroyThreads() {}
	virtual double valueFunc(int64_t time,int64_t timeCPU,void* miscData)=0;
	virtual string unit()=0;
	virtual void doRun(BenchmarkThread& t)=0;
	inline void operator()(BenchmarkThread& t) {
		return doRun(t);
	}
};
struct BenchmarkRun
{
	BenchmarkRunner* runner;
	Benchmark* target;
	int threads;
};
class BenchmarkThread
{
public:
	const BenchmarkRun* run;		//user-set
	void* miscData;			//user-set
	timespec beginTime;		//internal
	//beginTimeCPU: only includes time that the program is running (not pre-empted)
	timespec beginTimeCPU;	//internal
	timespec endTime;		//internal
	timespec endTimeCPU;	//internal
	pthread_t thread;		//user-set
	int threadIndex;		//user-set
	void init() {
		miscData=NULL;
	}
	void beginTiming() {
		clock_gettime(CLOCK_THREAD_CPUTIME_ID,&beginTimeCPU);
		clock_gettime(CLOCK_MONOTONIC_RAW,&beginTime);
	}
	void endTiming() {
		clock_gettime(CLOCK_THREAD_CPUTIME_ID,&endTimeCPU);
		clock_gettime(CLOCK_MONOTONIC_RAW,&endTime);
	}
};

//time diffs are in nanoseconds

struct BenchmarkResult
{
	struct PerThread
	{
		int64_t time;
		int64_t timeCPU;
		double value;
		void* miscData;
	};
	int64_t avgTime;
	int64_t avgTimeCPU;
	double avgValue;
	//deviation from average of the values, not the times
	double maxThreadDeviation;	//max deviation of each thread vs the average (among threads) in this run
	vector<PerThread> threads;
	void computeStats() {
		avgTime=avgTimeCPU=0;
		avgValue=maxThreadDeviation=0;
		for(int i=0;i<(int)threads.size();i++) {
			avgTime+=threads[i].time;
			avgTimeCPU+=threads[i].timeCPU;
			avgValue+=threads[i].value/threads.size();
		}
		avgTime/=threads.size();
		avgTimeCPU/=threads.size();
		for(int i=0;i<(int)threads.size();i++) {
			double d=double(threads[i].value-avgValue)/double(avgValue);
			if(d>maxThreadDeviation)maxThreadDeviation=d;
		}
	}
};
struct BenchmarkResultGroup
{
	vector<BenchmarkResult> results;
	int64_t avgTime;
	int64_t avgTimeCPU;
	double avgValue;
	double maxDeviation;		//max deviation of the average (among threads) of one run 
								//vs the average among all runs
	double maxThreadDeviation;	//maximum of 
								//{max deviation of each thread vs the average (among threads) in that run}
								//for each run
	string unit;
	int threads;
	void computeStats() {
		avgTime=avgTimeCPU=0;
		avgValue=maxDeviation=maxThreadDeviation=0;
		for(int i=0;i<(int)results.size();i++) {
			avgTime+=results[i].avgTime;
			avgTimeCPU+=results[i].avgTimeCPU;
			avgValue+=results[i].avgValue/results.size();
		}
		avgTime/=results.size();
		avgTimeCPU/=results.size();
		for(int i=0;i<(int)results.size();i++) {
			double d=double(results[i].avgValue-avgValue)/double(avgValue);
			if(d>maxDeviation)maxDeviation=d;
			if(results[i].maxThreadDeviation>maxThreadDeviation)
				maxThreadDeviation=results[i].maxThreadDeviation;
		}
	}
};

class BenchmarkRunner
{
public:
	int threads=-1;
	int runs=-1;
	static void* _doRun(void* v) {
		BenchmarkThread* t=(BenchmarkThread*)v;
		(*t->run->target)(*t);
		return NULL;
	}
	BenchmarkResult doRun(const BenchmarkRun& r) {
		int t=r.threads;
		BenchmarkThread bt[t];
		for(int i=0;i<t;i++) {
			bt[i].run=&r;
			bt[i].threadIndex=i;
			bt[i].init();
		}
		if(t==1) {
			(*r.target)(bt[0]);
		} else {
			for(int i=0;i<t;i++)
				if (pthread_create(&bt[i].thread, NULL, BenchmarkRunner::_doRun, &bt[i]) != 0) {
					throw runtime_error(strerror(errno));
				}
			for(int i=0;i<t;i++)
				pthread_join(bt[i].thread,NULL);
		}
		BenchmarkResult res;
		res.threads.resize(t);
		for(int i=0;i<t;i++) {
			res.threads[i].time=bt[i].endTime-bt[i].beginTime;
			res.threads[i].timeCPU=bt[i].endTimeCPU-bt[i].beginTimeCPU;
			res.threads[i].value=r.target->valueFunc(res.threads[i].time,
				res.threads[i].timeCPU, bt[i].miscData);
			res.threads[i].miscData=bt[i].miscData;
		}
		res.computeStats();
		return res;
	}
	BenchmarkResultGroup doRun(Benchmark& target, int threads, int runs) {
		BenchmarkRun r{this,&target,threads};
		BenchmarkResultGroup rg;
		rg.results.resize(runs);
		target.prepareThreads(r.threads);
		for(int i=0;i<runs;i++) {
			rg.results[i]=doRun(r);
		}
		target.destroyThreads();
		rg.computeStats();
		rg.unit=target.unit();
		rg.threads=r.threads;
		return rg;
	}
	int _runs() { return runs<=0 ? 20 : runs; }
	int _threads() { return threads<=0 ? (int)sysconf(_SC_NPROCESSORS_ONLN) : threads; }
	BenchmarkResultGroup runTest_single(Benchmark& target) {
		return doRun(target,_threads(),_runs());
	}
	void displayResult_single(const char* name, const BenchmarkResultGroup& rg) {
		//printf("format:		name 	runs	avgTime	avgCPUTime	maxDeviation	maxThreadDeviation\n");
		auto& tmp=rg;
		
		printf("full-load benchmark for '%s':\n",name);
		printf("   threads:      %13i\n",(int)tmp.threads);
		printf("   valuePerCore: %13.2lf %s\n",tmp.avgValue,tmp.unit.c_str());
		printf("   valueTotal:   %13.2lf %s\n",tmp.avgValue*tmp.threads,tmp.unit.c_str());
		printf("   avgTime:      %13.2lf ms\n",double(tmp.avgTime)/1000000);
		printf("   avgCPUTime:   %13.2lf ms\n",double(tmp.avgTimeCPU)/1000000);
		printf("   maxDeviation: %13.2lf %%\n",tmp.maxDeviation*100);
		printf("   maxThreadDev: %13.2lf %%\n",tmp.maxThreadDeviation*100);
		printf("\n");
	}
	void runTest_concurrencyAnalysis(const char* name,Benchmark& target) {
		printf("concurrency analysis for benchmark '%s':\n",name);
		printf("   values are in %s\n",target.unit().c_str());
		printf("   threads      value        perCore  maxDev  maxTDev singleCoreRatio\n");
		double maxDev=0,maxTDev=0;
		double sc=0;
		for(int i=1;i<=_threads();i++) {
			BenchmarkResultGroup rg=doRun(target,i,_runs());
			auto& tmp=rg;
			
			if(i==1) sc=tmp.avgValue;
			printf("   %3i  %13.2lf  %13.2lf  %6.2lf   %6.2lf %13.2f %%\n",
				i,tmp.avgValue*tmp.threads,tmp.avgValue,tmp.maxDeviation*100,
				tmp.maxThreadDeviation*100,tmp.avgValue/sc*100);
			if(tmp.maxDeviation>maxDev)maxDev=tmp.maxDeviation;
			if(tmp.maxThreadDeviation>maxTDev)maxTDev=tmp.maxThreadDeviation;
		}
		printf("   maxDeviation: %13.2lf %%\n",maxDev*100);
		printf("   maxThreadDev: %13.2lf %%\n",maxTDev*100);
		printf("\n");
	}
	void runDefaultTests(Benchmark& target, string name) {
		printf("global parameters:\n");
		printf("   runs:         %13i\n\n",(int)_runs());
		displayResult_single(name.c_str(),runTest_single(target));
		runTest_concurrencyAnalysis(name.c_str(),target);
	}
};
