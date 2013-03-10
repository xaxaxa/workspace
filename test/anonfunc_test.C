#include <iostream>
#include <stdio.h>
#include <delegate.H>
using namespace std;
void call(Delegate<void()> d) {
	d();
}
int main() {
	struct tmp {
		int a;
		void operator()() {
			cout << a << endl;
		}
	};
	tmp tmp1;
	tmp1.a=5;
	call(&tmp1);
	return 0;
}
