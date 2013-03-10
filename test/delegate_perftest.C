#include <delegate.H>
#include <stdio.h>
#include <functional>
#include <string.h>
#include <stdlib.h>
using namespace std;
//these functions are in a separately compiled shared library to avoid inlining
void call100times(Delegate<void()> d);
void call100times_i(Delegate<void(int)> d);

void call100times(function<void()>& d);
void call100times_i(function<void(int)>& d);

void call100000times(Delegate<void()> d);
void call100000times_i(Delegate<void(int)> d);

void call100000times(function<void()>& d);
void call100000times_i(function<void(int)>& d);

void call100000times(void (*d)());
void call100000times_i(void (*d)(int));

const int iterations=5000;

//for C function ptrs
int X;
void Func(int i) {
	X+=i;
}
int main(int argc, char** argv) {
	if(argc<2) {
		printf("usage: %s del|std|cfunc [c]\n(tests Delegate, std::function, or C function pointers, respectively)\n",argv[0]);
		printf("specify \"c\" to re-create the function object every 100 iterations (not applicable to cfunc)\n");
		return 1;
	}
	printf("calling function %i times...\n",iterations*100000);
	if(strcmp(argv[1],"del")==0) {
		if(argc>=3 && argv[2][0]=='c') {
			for(int i=0;i<iterations*1000;i++) {
				struct{
					int x;
					void operator()(int i) {
						x+=i;
					}
				} func;
				func.x=0;
				call100times_i(&func);
			}
			printf("done.\n");
		}
		else {
			struct{
				int x;
				void operator()(int i) {
					x+=i;
				}
			} func;
			func.x=0;
			for(int i=0;i<iterations;i++)
				call100000times_i(&func);
			printf("done. %i\n",func.x);
		}
	} else if(strcmp(argv[1],"std")==0) {
		if(argc>=3 && argv[2][0]=='c') {
			for(int i=0;i<iterations*1000;i++) {
				int x=0;
				function<void(int)> func=[&x](int i) {
					x+=i;
				};
				call100times_i(func);
			}
			printf("done.\n");
		}
		else {
			int x=0;
			function<void(int)> func=[&x](int i) {
				x+=i;
			};
			for(int i=0;i<iterations;i++)
				call100000times_i(func);
			printf("done. %i\n",x);
		}
	} else if(strcmp(argv[1],"cfunc")==0) {
		X=0;
		for(int i=0;i<iterations;i++)
			call100000times_i(Func);
		printf("done. %i\n",X);
	}
	
}
