
#include <cplib/cplib.hpp>
#include <iostream>

using namespace std;
using namespace xaxaxa;
class a
{
public:
	DELEGATE(void,mydelegate,int,int);
	EVENT(mydelegate) event1;
	void raise(int x, int y)
	{
		RAISEEVENT(event1,x,y);
		RAISEEVENT(event1,y,x);
	}
};
class b
{
public:
	void handler1(int x, int y)
	{
		cout << "handler1 called x="<<x<<"; y="<<y<<endl;
	}
};
class c
{
public:
	int z;
	void handler2(int x, int y)
	{
		cout << "handler2 called x="<<x<<"; y="<<y<<"; z="<<z<<endl;
	}
};
void handler3(void* user, int x, int y)
{
	cout << "handler3 called x="<<x<<"; y="<<y<<endl;
}
int main()
{
	a a1;
	b b1;
	c c1;
	c1.z=678;
	ADDHANDLER(a1.event1,a::mydelegate(&b::handler1,&b1));
	ADDHANDLER(a1.event1,a::mydelegate(&c::handler2,&c1));
	ADDHANDLER(a1.event1,a::mydelegate(&handler3,NULL));
	a1.raise(12,345);
}
