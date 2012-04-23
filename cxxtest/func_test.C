#include <functional>
#include <iostream>
#include <string>
using namespace std;
using namespace placeholders;
int asdf(void* a, int b, string c)
{
	cout << a << ' ' << b << ' ' << c << endl;
	return b+1;
}
int main(int argc, char** argv)
{
	int (*f)(void*,int,string);
	f=&asdf;
	f(argv,argc,"asdfg");
	function<int(void*,int,string)> f1;
	//function<int(int,string)> f2;
	f1=f;
	auto f2=std::bind(f1,(void*)NULL);
	f2();
}
