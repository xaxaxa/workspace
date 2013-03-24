#include <cpoll/cpoll.H>
#include <unistd.h>
#include <string>
#include <iostream>

using namespace std;
using namespace CP;
int main() {
	StandardStream ss;
	/*char buf[10];
	struct {
		StandardStream& ss;
		char* buf;
		void operator()(int r) {
			ss.write(buf,10);
		}
	} cb2 {ss,buf};
	ss.readAll(buf,10,&cb2);
	
	Poll p;
	ss.addToPoll(p);
	p.loop();
	return 0;*/
	
	StreamReader sr(ss);
	
	//*
	struct {
		StreamReader& sr;
		void operator()(string s) {
			//if(s.length()==0 && sr.eof) 
			cout << "got string (length=" << s.length() << "): " << s << endl;
			sr.readLine(this);
		}
	} cb {sr};
	sr.readLine(&cb);
	//*/
	
	/*
	while(true) {
		string s=sr.readLine();
		if(s.length()==0 && sr.eof) return 0;
		cout << s << endl;
	}
	//*/
	Poll p;
	ss.addToPoll(p);
	p.loop();
}
