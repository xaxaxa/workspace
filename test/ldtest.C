#include <iostream>
#include <dlfcn.h>

void asdf(int a);
void (*asdf1)(int a);
void (*asdf2)(int a);

using namespace std;

int main()
{
	//asdf(3);
	void* v=dlopen("/home/xaxaxa/workspace/test/ldtest1.so",RTLD_LAZY|RTLD_GLOBAL);
	cout << v << endl;
	cout << (asdf1=(void(*)(int))dlsym(v,"_Z4asdfi")) << endl;
	
	/*void* v2=dlopen("./ldtest1.so",RTLD_LAZY|RTLD_LOCAL);
	cout << v2 << endl;
	cout << (asdf2=(void(*)(int))dlsym(v2,"_Z4asdfi")) << endl;*/
	//cout << "a" << endl;
	
	asdf(5);
}

