#include <cpoll/cpoll.H>
#include <unistd.h>
#include <string>
#include <iostream>

using namespace std;
using namespace CP;
int main() {
	StandardStream ss;
	StreamBuffer sbuf(ss);
	StreamWriter sw(sbuf);
	sw.write("asssssss\n");
	sw.writeF("gfshjgs %p\n",&ss);
	sw.flush();
	sw.write(5);
	sw.write('\n');
	sw.flush();
	return 0;
}
