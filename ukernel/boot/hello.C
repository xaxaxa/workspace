#include <sys/syscall.h>
#include <time.h>

extern "C" int __syscall1(int sys,...);
extern "C" int __syscall2(int sys,...);
extern "C" int main(int argc, char** argv) {
	struct timespec t;
	t.tv_sec=2;
	t.tv_nsec=0;
	__syscall2(__NR_nanosleep,&t,(void*)0);
	return argc+2;
}
