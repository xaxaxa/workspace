#include <iostream>
#include <list>
#include <vector>
#include <functional>
using namespace std;

int func(int a, int b)
{
	return a+b;
}

int main()
{
	const int funcs=5000000;
	cout << "constructing functions" << endl;
	function<int(int,int)>* f=new function<int(int,int)>[funcs];
	cout << "done" << endl;
	cout << "creating functions" << endl;
	for(int i=0;i<funcs;i++)
		//f[i]=[](int a, int b) -> int {return a+b;};
		f[i]=&func;
	cout << "done" << endl;
	cout << "calling functions" << endl;
	for(int i=0;i<funcs;i++)
		f[i](i,i+5);
	cout << "done" << endl;
	
	/*cout << "constructing functions" << endl;
	typedef int (*func_t)(int, int);
	func_t* f=new func_t[funcs];
	cout << "done" << endl;
	cout << "creating functions" << endl;
	for(int i=0;i<funcs;i++)
		f[i]=&func;
	cout << "done" << endl;
	cout << "calling functions" << endl;
	for(int i=0;i<funcs;i++)
		f[i](i,i+5);
	cout << "done" << endl;*/
}
