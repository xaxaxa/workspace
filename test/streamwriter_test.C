#include <cpoll/cpoll.H>
#include <unistd.h>

using namespace CP;
int main(int argc, char** argv) {
	/*MemoryStream ms;
	StreamWriter sw(ms);
	for(int a=0;a<100000;a++) {
		for(int i=0;i<10;i++) {
			sw.writeF("a %i %i ",a,i);
		}
		sw.write("\n");
	}
	sw.flush();
	write(1,ms.buffer,ms.len);*/
	StandardStream ss;
	
	MemoryStream ms;
	{
		StreamWriter sw((Stream&)ms);
		sw.write("aaaaa\n");
		sw.writeF("aaa %i sss\n",argc);
	}
	ss.write(ms.data(),ms.length());
}
