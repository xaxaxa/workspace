#include <cpoll/cpoll.H>
#include <cpoll/http.H>
//#include <curl_httpreq.H>
#include <iostream>
#include <unistd.h>

using namespace CP;
using namespace std;
int main(int argc, char **argv)
{
	Poll p;
	/*
	Timer timer((uint64_t)500);
	struct {
		void operator()(int i) {
			cout << i << endl;
		}
	} cb;
	timer.setCallback(&cb);
	p.add(timer);
	p.loop();
	return 0;*/
	curl::instance inst;
	curl::newInstance(&inst,&p);
	int total=0;
	curl::transferInfo* t=curl::addTransfer(&inst,argv[1],
		[&](const void* data, int len, int state)
	{
		cout << len << endl;
		total+=len;
		if(data!=NULL && len>0)
			write(1,data,len);
		return true;
	});
	curl::beginTransfer(&inst, t);
	//curl::eventLoop(&inst);
	//curl::checkQueue(&inst);
	p.loop();
	curl::dispose(&inst);
	cout << "totalBytes=" << total << endl;
	return 0;
}
