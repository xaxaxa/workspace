#include <cppsp/urlparser.H>
#include <stdio.h>
using namespace CP;
int main(int argc, char** argv) {
	if(argc<2) {
		printf("usage: %s string\n",argv[0]);
		return 1;
	}
	//MemoryStream ms;
	StandardStream ss;
	{
		StreamWriter sw(ss);
		struct {
			StreamWriter& sw;
			void operator()(const char* name, int nameLen, const char* value, int valueLen) {
				sw.write(name,nameLen);
				sw.write(": ");
				sw.write(value,valueLen);
				sw.write('\n');
			}
		} cb {sw};
		cppsp::parseQueryString(argv[1],strlen(argv[1]),&cb);
	}
	//StandardStream().write(ms.data(),ms.length());
}
