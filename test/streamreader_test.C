#include <cpoll/cpoll.H>
#include <unistd.h>
#include <string>
#include <iostream>

using namespace std;
using namespace CP;
int main() {
	StandardStream ss;
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
