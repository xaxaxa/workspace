#include <delegate.H>
#include <functional>
using namespace std;
//these functions are in a separately compiled shared library to avoid inlining

void call100times(Delegate<void()> d) {
	for(int i=0;i<100;i++)
		d();
}
void call100times_i(Delegate<void(int)> d) {
	for(int i=0;i<100;i++)
		d(i);
}

void call100times(function<void()>& d) {
	for(int i=0;i<100;i++)
		d();
}
void call100times_i(function<void(int)>& d) {
	for(int i=0;i<100;i++)
		d(i);
}

void call100000times(Delegate<void()> d) {
	for(int i=0;i<100000;i++)
		d();
}
void call100000times_i(Delegate<void(int)> d) {
	for(int i=0;i<100000;i++)
		d(i);
}

void call100000times(function<void()>& d) {
	for(int i=0;i<100000;i++)
		d();
}
void call100000times_i(function<void(int)>& d) {
	for(int i=0;i<100000;i++)
		d(i);
}

void call100000times(void (*d)()) {
	for(int i=0;i<100000;i++)
		d();
}
void call100000times_i(void (*d)(int)) {
	for(int i=0;i<100000;i++)
		d(i);
}

