#include <iostream>
#include <functional>
#include <string.h>
using namespace std;
struct s
{
	int data;
	int func(int a)
	{
		int tmp=data;
		data=a;
		return tmp;
	}
};
function<int(int,int)> f;
void setfunc()
{
	f=[](int x, int y) -> int { return x + y; };
	//f=function(x,y){return x+y;};
	
	auto tmp=[](){};
	tmp();
}
function<void()> get_f()
{
	function<void()> f=[&f]()
	{
		cout << "a" << endl;
		f();
	};
	return f;
}
int main()
{
	/*
	setfunc();
	cout << f(5,6) << endl;
	s s1;
	s1.data=123;
	auto f2=&s::func;
	cout << (s1.*f2)(32) << endl;
	cout << s1.data << endl;
	//*/
	
	function<void()> f=get_f();
	{
		long asdf[100];
		memset(asdf,100,123);
	}
	//cout <<a<<b<<c<<d<<e<<endl;
	f();
}
