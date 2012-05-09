#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;

typedef int (*func_t)(int, int);
struct str1
{
	//func_t f;
	function<int(int,int)> f;
};
int func(int a, int b)
{
	return a+b;
}
int func2(int a, int b)
{
	return a-b;
}

int main(int argc, char** argv)
{
	const long long funcs=500000000;
	
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
	cout << "done" << endl;
	//*/
	
	/*cout << "constructing functions" << endl;
	Function<int(int,int)>* f=new Function<int(int,int)>[funcs];
	cout << "done" << endl;
	cout << "sizeof(Function<int(int,int)>) = " << sizeof(Function<int(int,int)>) << endl;
	cout << "creating functions" << endl;
	for(int i=0;i<funcs;i++)
		//f[i]=[](int a, int b) -> int {return a+b;};
		f[i]=Function<int(int,int)>(&func);
	cout << "done" << endl;
	cout << "calling functions" << endl;
	for(int i=0;i<funcs;i++)
		(f[i])(i,i+5);
	cout << "done" << endl;
	//*/
	/*
	cout << "constructing functions" << endl;
	Delegate<int(int,int)>** f=new Delegate<int(int,int)>*[funcs];
	cout << "done" << endl;
	cout << "sizeof(Delegate<int(int,int)>) = " << sizeof(Delegate<int(int,int)>) << endl;
	cout << "creating functions" << endl;
	struct:Delegate<int(int,int)>
	{
		int operator()(int a, int b)
		{
			return a+b;
		}
	} tmp_str;
	for(int i=0;i<funcs;i++)
		//f[i]=[](int a, int b) -> int {return a+b;};
		f[i]=&tmp_str;
	cout << "done" << endl;
	cout << "calling functions" << endl;
	for(int i=0;i<funcs;i++)
		(*f[i])(i,i+5);
	cout << "done" << endl;
	//*/
	
	/*cout << "constructing functions" << endl;
	typedef int (*func_t)(int, int);
	func_t* f=new func_t[funcs];
	cout << "done" << endl;
	cout << "sizeof(int (*)(int, int)) = " << sizeof(func_t) << endl;
	cout << "creating functions" << endl;
	for(int i=0;i<funcs;i++)
		f[i]=[](int a, int b){return a+b;};
	cout << "done" << endl;
	cout << "calling functions" << endl;
	for(int i=0;i<funcs;i++)
		f[i](i,i+5);
	cout << "done" << endl;
	//*/
	
	/*int asdf;
	str1 s;
	s.f=[argc,&asdf](int a, int b){asdf+=b;return a+b+argc;};
	//cout << (void*)s.f << endl;
	//while(1)sleep(1);
	//function<int(int,int)> f=func;
	int x=0;
	for(long long i=0;i<funcs;i++)
		//func(5,6);
		x+=s.f(5,i);
	cout << x << endl;
	//*/
	
	
	
	function<int(int, int)>* funct=new function<int(int, int)>[5];
	funct[1]=[](int a, int b){return a+b;};
	funct[2]=[](int a, int b){return a-b;};
	//call10times([](){cout << "aaa" << endl;});
	//*/
	/*typedef int (*func_t)(int, int);
	func_t* funct=new func_t[5];
	funct[1]=&func;
	funct[2]=&func2;
	//*/
	
	int aaa=2;
	for(int i=0;i<funcs;i++)
	{
		aaa=funct[argc](i,aaa);
	}
	//*/
}
