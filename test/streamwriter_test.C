#include <cpoll/cpoll.H>
#include <unistd.h>

using namespace CP;
int main() {
	MemoryStream ms;
	StreamWriter sw(ms);
	for(int a=0;a<100000;a++) {
		for(int i=0;i<10;i++) {
			/*sw.write("a ");
			sw.write(a);
			sw.write(" ");
			sw.write(i);
			sw.write(" ");*/
			sw.writeF("a %i %i ",a,i);
		}
		sw.write("\n");
	}
	sw.flush();
	write(1,ms.buffer,ms.len);
}
