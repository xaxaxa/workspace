#include <iostream>
#include <dlfcn.h>

using namespace std;
typedef int(*mainfunc)(int,char**);

int main(int argc, char** argv)
{
	mainfunc f;
	void* v=dlopen("./a",RTLD_LAZY);
	//cout << dlerror() << endl;
	cout << v << endl;
	cout << (f=(mainfunc)dlsym(v,"main")) << endl;
	f(argc,argv);
	
}

