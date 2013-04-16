#include <cpoll/cpoll.H>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <fcntl.h>

using namespace std;
using namespace CP;
#define nthreads 16
struct entry
{
	string fn;
};
struct processorThread
{
	vector<entry>* list;
	vector<entry> list_out;
	int start,end;
};

void* th1(void* v) {
	processorThread& inf=*(processorThread*)v;
	struct stat st;
	for(int i=inf.start;i<inf.end;i++) {
		//(*inf.list)[i].date=getDate((*inf.list)[i].fn);
		if(stat((*inf.list)[i].fn.c_str(),&st)==0 && S_ISDIR(st.st_mode)) {
			inf.list_out.push_back((*inf.list)[i]);
		}
	}
}
bool comp1(const entry& e1, const entry& e2) {
	//if(e1.date.length()==0) return e2.date.length()!=0;
	//if(e2.date.length()==0) return false;
	return e1.fn<e2.fn;
}
int main() {
	vector<entry> list;
	struct {
		vector<entry>& list;
		void operator()(const char* name) {
			list.push_back({name});
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
	vector<entry> a;
	for(int i=0;i<threads;i++) {
		void* ret;
		pthread_join(thr[i],&ret);
		a.insert(a.end(), thr1[i].list_out.begin(), thr1[i].list_out.end());
	}
	sort(a.begin(),a.end(),comp1);
	File f(open("index.txt",O_CREAT|O_RDWR|O_TRUNC,0666));
	StreamWriter sw(f);
	for(int i=0;i<a.size();i++) {
		sw.writeF("%s\n",a[i].fn.c_str());
	}
}
