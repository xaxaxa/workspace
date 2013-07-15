#include <cpoll/cpoll.H>
using namespace CP;
int main() {
	StandardStream ss;
	StreamReader sr(ss);
	StreamWriter sw(ss);
	
	MemoryStream ms;
	while(sr.readLine(ms)>0) {
		String d=ms.String();
		int i=d.indexOf(" ");
		uint32_t a=(uint32_t)atoll(d.subString(0,i));
		int b=atoi(d.subString(i+1));
		
		char buf[a];
		sw.writeF("%p\n",(char*)buf);
		sw.flush();
		buf[0]=(char)b;
		
		
		ms.clear();
	}
}
