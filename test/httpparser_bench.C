#include <cpoll/cpoll.H>
#include <cppsp/httpparser.H>
#include <stdio.h>

using namespace CP;
using namespace cppsp;
int main() {
	StandardStream ss;
	StringPool sp;
	headerContainer hc(&sp);
	HTTPParser p(&hc);
	int reqs=0;
	while(true) {
		String tmp=p.beginPutData(4096);
		int i=ss.read(tmp.data(),tmp.length());
		if(i<=0) break;
		p.endPutData(i);
		while(p.process()) reqs++;
		p.reset();
		hc.clear();
		sp.clear();
	}
	printf("%i requests\n",reqs);
}
