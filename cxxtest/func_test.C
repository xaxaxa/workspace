#include <functional>
#include <iostream>
#include <string>
#include <cplib/cplib.hpp>
using namespace std;
using namespace placeholders;
using namespace xaxaxa;
int asdf(void* a, int b, string c)
{
	cout << a << ' ' << b << ' ' << c << endl;
	return b+1;
}
void call10times(void (*func)(void*), void* param)
{
	for(int i=0;i<10;i++)
		func(param);
}
int main(int argc, char** argv)
{
	/*int (*f)(void*,int,string);
	f=&asdf;
	f(argv,argc,"asdfg");
	function<int(void*,int,string)> f1;
	//function<int(int,string)> f2;
	f1=f;
	auto f2=std::bind(f1,(void*)NULL);
	f2();*/
	FunctionWrapper<void()> w([argv](){cout << argv[0] << endl;});
	call10times(w.Get(),&w);
}
