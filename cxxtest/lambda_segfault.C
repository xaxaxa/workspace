#include <iostream>
#include <functional>
using namespace std;

function<void(int)> func;
void aaa()
{
	struct s {
		function<void()> f;
		function<void(int)> f2;
		char buf[4096];
	};
	s* s1=new s();
	s1->f2=[s1](int i) {
		if(i==5) s1->f();
		_exit(0);
	};
	s1->f=[s1]() {
		delete s1;
		_exit(0);
	};
	func=s1->f2;
}
int main()
{
	aaa();
	func(5);
}
