#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
using namespace std;
struct yieldCall
{
	ucontext_t newctx;
	ucontext_t oldctx;
	int val;
	bool valid;
	bool read() {
		newctx.uc_link=&oldctx;
		swapcontext(&oldctx,&newctx);
		return valid;
	}
};
void yield(yieldCall* p,int i) {
	p->val=i;
	swapcontext(&p->newctx,&p->oldctx);
}
void Return(yieldCall* p) {
	p->valid=false;
	swapcontext(&p->newctx,&p->oldctx);
}
stack_t makeStack(int size) {
	stack_t tmp;
	if((tmp.ss_sp=malloc(size))==nullptr) throw bad_alloc();
	tmp.ss_size=(size_t)size;
	tmp.ss_flags=0;
	return tmp;
}
void call(yieldCall& yc,void (*func)(yieldCall*)) {
	getcontext(&yc.newctx);
	yc.newctx.uc_stack=makeStack(4096); //4k stack
	yc.valid=true;
	makecontext(&yc.newctx,(void(*)())func,sizeof(&yc)/sizeof(int),&yc);
}


//yield return version
void func(yieldCall* p) {
	for(int i=0;i<10000000;i++) {
		yield(p,i);
	}
	Return(p);
}

//explicit state machine version
struct iter {
	int i;
	int val;
	bool read() {
		if(i<10000000) {
			val=i++;
			return true;
		}
		return false;
	}
};
iter func2() {
	iter tmp;
	tmp.i=0;
	return tmp;
}



int main(int argc, char** argv) {
	int64_t x=0;
	
	if(argc>1) {
		yieldCall yc;
		call(yc,&func);
		while(yc.read()) {
			x+=yc.val;
		}
	} else {
		iter tmp=func2();
		while(tmp.read()) {
			x+=tmp.val;
		}
	}
	
	printf("%lli\n",(long long)x);
}
