#include <iostream>
#include <cplib/cplib.hpp>
using namespace std;

/*void* func1()
{
	return &&aaaaa;
aaaaa:
	cout << "fuck" << endl;
	asm("ret");
}
void* zxcv;
void func2(int a, int b)
{
	goto *zxcv;
}*/

template<class SIGNATURE> struct delegate;

template<class RET, class... ARGS> struct delegate<RET(ARGS...)>
{
	virtual RET operator()(ARGS...)=0;
};
void __do_call(delegate<void()>& d)
{
	d();
}
DELEGATE(void,callback,int);
int main(int c, char** v)
{
	//zxcv=func1();
	//cout << zxcv << endl;
	//asm("call zxcv");
	
	/*int x=5,y=6;
	func2(7,8);
	func1(x,y);*/
	struct:delegate<void()>{void operator()()
	{
		cout << "aaaaa" << endl;
	}} tmp;
	__do_call(tmp);
	
	/*struct
	{
		int i;
		void call(int a)
		{
			cout << i << endl;
		}
	} cb1;
	cb1.i=10;
	callback tmp1(&decltype(cb1)::call,&cb1);
	FUNCTION_CALL(tmp1,0);*/
}

