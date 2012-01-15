#include <iostream>
#include <functional>
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
int main()
{
	setfunc();
	cout << f(5,6) << endl;
	s s1;
	s1.data=123;
	auto f2=&s::func;
	cout << (s1.*f2)(32) << endl;
	cout << s1.data << endl;
}
