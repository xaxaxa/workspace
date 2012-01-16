#include <iostream>
#include <functional>
using namespace std;

void f(function<void()> fn)
{
	fn();fn();
}
int main()
{
	/*function<void()> f1,f2;
	f1=[&f2](){f2();};
	f2=[&f1](){f1();};
	f1();
	auto func=[](){cout << "a" << endl;};
	f(func);
	
	a[i]
	*(a+i)
	*(i+a)
	i[a]*/
	//int* i=new int[123];
	//i[0]=321;
	//i[1]=567;
	
	/*char* c=(char*)i;
	c=c+1;
	i=(int*)c;*/
	//i=i+1;
	//cout << *i;
	
	float f=0;
	cout << f << endl;
	//cout << sizeof(float) << endl;
	int* i=(int*)&f;
	cout << *i << endl;
	
	
	/*float f=5.7;
	int i=f;
	cout << f << endl;
	cout << i << endl;*/
	return 0;
}
