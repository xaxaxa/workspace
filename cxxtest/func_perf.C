#include <iostream>
#include <list>
#include <vector>
#include <functional>
using namespace std;

typedef int (*func_t)(int, int);
struct str1
{
	func_t f;
};
int func(int a, int b)
{
	return a+b;
}

int main()
{
	const int funcs=500000000;
	
	/*cout << "constructing functions" << endl;
	function<int(int,int)>* f=new function<int(int,int)>[funcs];
	cout << "done" << endl;
	cout << "sizeof(function<int(int,int)>) = " << sizeof(function<int(int,int)>) << endl;
	cout << "creating functions" << endl;
	for(int i=0;i<funcs;i++)
		//f[i]=[](int a, int b) -> int {return a+b;};
		f[i]=&func;
	cout << "done" << endl;
	cout << "calling functions" << endl;
	for(int i=0;i<funcs;i++)
		f[i](i,i+5);
	cout << "done" << endl;*/
	
	/*cout << "constructing functions" << endl;
	typedef int (*func_t)(int, int);
	func_t* f=new func_t[funcs];
	cout << "done" << endl;
	cout << "sizeof(int (*)(int, int)) = " << sizeof(func_t) << endl;
	cout << "creating functions" << endl;
	for(int i=0;i<funcs;i++)
		f[i]=&func;
	cout << "done" << endl;
	cout << "calling functions" << endl;
	for(int i=0;i<funcs;i++)
		f[i](i,i+5);
	cout << "done" << endl;*/
	
	str1 s;
	s.f=&func;
	
	//function<int(int,int)> f=func;
	for(int i=0;i<funcs;i++)
		//func(5,6);
		s.f(5,6);
}
