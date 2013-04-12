#include <cpoll/cpoll.H>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include "jsoncpp/json/value.h"
#include "jsoncpp/json/reader.h"

using namespace std;
using namespace CP;
#define nthreads 16
int tsCompare(struct timespec time1, struct timespec time2) {
	if (time1.tv_sec < time2.tv_sec) return (-1); /* Less than. */
	else if (time1.tv_sec > time2.tv_sec) return (1); /* Greater than. */
	else if (time1.tv_nsec < time2.tv_nsec) return (-1); /* Less than. */
	else if (time1.tv_nsec > time2.tv_nsec) return (1); /* Greater than. */
	else return (0); /* Equal. */
}
bool compDate(string a,string b) {
	//return a<b;
	struct stat st1,st2;
	bool s1=stat(a.c_str(),&st1)>=0;
	bool s2=stat(b.c_str(),&st2)>=0;
	if(!s1) return s2;
	if(!s2) return false;
	return tsCompare(st1.st_mtim,st2.st_mtim)<0;
}
bool compDate2(string a,string b) {
	StringStream ms1,ms2;
	{
		File f1(open(a.c_str(),O_RDONLY));
		File f2(open(b.c_str(),O_RDONLY));
		f1.readToEnd(ms1);
		f2.readToEnd(ms2);
		ms1.flush();		ms2.flush();
		ms1.write("\0",1);	ms2.write("\0",1);
	}
	Json::Value root1,root2;
	Json::Reader reader1,reader2;
	bool s1 = reader1.parse(ms1.str(), root1, false);
	bool s2 = reader2.parse(ms2.str(), root2, false);
	if(!s1) return s2;
	if(!s2) return false;
	Json::Value ct1=root1["created_time"];
	Json::Value ct2=root2["created_time"];
	if(ct1.isNull()) return !ct2.isNull();
	if(ct2.isNull()) return false;
	return ct1.asString()<ct2.asString();
}

struct entry
{
	string fn;
	string date;
};
bool compDate3(const entry& e1, const entry& e2) {
	//if(e1.date.length()==0) return e2.date.length()!=0;
	//if(e2.date.length()==0) return false;
	return e1.date<e2.date;
}
struct processorThread
{
	vector<entry>* list;
	int start,end;
};
string getDate(string fn) {
	StringStream ms1;
	{
		File f1(open(fn.c_str(),O_RDONLY));
		f1.readToEnd(ms1);
		ms1.flush();
		ms1.write("\0",1);
	}
	Json::Value root1;
	Json::Reader reader1;
	bool s1 = reader1.parse(ms1.str(), root1, false);
	if(!s1) return string();
	if(!root1.isObject()) return string();
	Json::Value ct1=root1["created_time"];
	if(ct1.isNull())return string();
	return ct1.asString();
}
void* th1(void* v) {
	processorThread& inf=*(processorThread*)v;
	for(int i=inf.start;i<inf.end;i++) {
		(*inf.list)[i].date=getDate((*inf.list)[i].fn);
	}
}
int main(int argc, char** argv) {
	vector<entry> list;
	struct {
		vector<entry>& list;
		void operator()(const char* name) {
			list.push_back({name,string()});
		}
	} cb {list};
	listDirectory(".",&cb);
	
	int threads=nthreads;
	pthread_t thr[threads];
	processorThread thr1[threads];
	int tmp=0;
	int incr=list.size()/threads;
	if(incr<1)incr=1;
	for(int i=0;i<threads;i++) {
		thr1[i].list=&list;
		thr1[i].start=tmp;
		if(i==threads-1) thr1[i].end=list.size();
		else {
			tmp+=incr;
			thr1[i].end=tmp;
			if(thr1[i].end>list.size()) {
				threads=i;
				break;
			}
		}
		pthread_create(&thr[i],NULL,th1,&thr1[i]);
	}
	for(int i=0;i<threads;i++) {
		void* ret;
		pthread_join(thr[i],&ret);
	}
	sort(list.rbegin(),list.rend(),compDate3);
	File f(open("index.txt",O_CREAT|O_RDWR|O_TRUNC,0666));
	StreamWriter sw(f);
	for(int i=0;i<list.size();i++) {
		sw.writeF("%s\n",list[i].fn.c_str());
	}
}
