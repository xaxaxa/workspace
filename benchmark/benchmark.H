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

class Benchmark;
class BenchmarkThread;
struct BenchmarkRun
{
	Benchmark* benchmark;
	Delegate<void(BenchmarkThread&)> target;
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
class Benchmark
{
public:
	int threads=-1;
	int runs=-1;
	map<string,BenchmarkResultGroup> results;
	static void* _doRun(void* v) {
		BenchmarkThread* t=(BenchmarkThread*)v;
		t->run->target(*t);
		return NULL;
	}
	Delegate<void(int)> prepareThreads;
	Delegate<void()> destroyThreads;
	Delegate<double(int64_t time,int64_t timeCPU,void* miscData)> valueFunc;	//required
	string unit;
	BenchmarkRun prepare(Delegate<void(BenchmarkThread&)> target) {
		int t=threads;
		if(t<=0) t=(int)sysconf(_SC_NPROCESSORS_ONLN);
		if(t<=0) t=1;
		return {this,target,t};
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
			r.target(bt[0]);
		} else {
			for(int i=0;i<t;i++)
				if (pthread_create(&bt[i].thread, NULL, Benchmark::_doRun, &bt[i]) != 0) {
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
			res.threads[i].value=valueFunc(res.threads[i].time,
				res.threads[i].timeCPU, bt[i].miscData);
			res.threads[i].miscData=bt[i].miscData;
		}
		res.computeStats();
		return res;
	}
	BenchmarkResultGroup doRun(Delegate<void(BenchmarkThread&)> target, int runs) {
		BenchmarkRun r=prepare(target);
		BenchmarkResultGroup rg;
		rg.results.resize(runs);
		if(prepareThreads!=nullptr)prepareThreads(r.threads);
		for(int i=0;i<runs;i++) {
			rg.results[i]=doRun(r);
		}
		if(destroyThreads!=nullptr)destroyThreads();
		rg.computeStats();
		rg.unit=unit;
		rg.threads=r.threads;
		return rg;
	}
	void run(Delegate<void(BenchmarkThread&)> target, string name) {
		int r=runs;
		if(r==-1)r=20;
		results.insert({name,doRun(target,r)});
	}
	void displayResults() {
		//printf("format:		name 	runs	avgTime	avgCPUTime	maxDeviation	maxThreadDeviation\n");
		for(auto it=results.begin();it!=results.end();it++) {
			auto& tmp=(*it).second;
			printf("%s:\n",(*it).first.c_str());
			printf("\tthreads:	%i\n",(int)tmp.threads);
			printf("\truns:		%i\n",(int)tmp.results.size());
			printf("\tavgValue:	%lf %s\n",tmp.avgValue,tmp.unit.c_str());
			printf("\tavgTime:	%lli\n",(long long int)tmp.avgTime);
			printf("\tavgCPUTime:	%lli\n",(long long int)tmp.avgTimeCPU);
			printf("\tmaxDeviation:	%lf%%\n",tmp.maxDeviation*100);
			printf("\tmaxThreadDev:	%lf%%\n",tmp.maxThreadDeviation*100);
		}
	}
};
