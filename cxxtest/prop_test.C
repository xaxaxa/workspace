#define __debug_obj123

#include <iostream>
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;








struct c1
{
	Property<Object> a;
	//Object* a;
};

void func1()
{
	cout << "func1 called" << endl;
}
void call10times(function<void()> func)
{
	for(int i=0;i<10000000;i++)
		func();
}
void aaaaa()
{
	objref<Object> tmp;
	
	
	
	
	/*c1 obj;
	obj.a=tmp();
	obj.a=newobj<Object>();*/
	//tmp=new Object();
}
int main(int argc, char **argv)
{
	aaaaa();
	
}
