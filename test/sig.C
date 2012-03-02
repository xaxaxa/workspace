#include <iostream>
#include <memory.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

using namespace std;


void handle_sigchld(int sig, siginfo_t* inf, void* v)
{
	cout << inf->si_pid << endl;
}
int main()
{
	/*struct sigaction sa;
	memset(&sa,0,sizeof(sa));
	sa.sa_sigaction=&handle_sigchld;
	sigaction(SIGCHLD,&sa,NULL);*/
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	cout << t.tv_nsec << endl;
	sigset_t tmp;
	sigemptyset(&tmp);
	sigaddset(&tmp, SIGCHLD);
	sigprocmask(SIG_BLOCK,&tmp,NULL);
	int fd=signalfd(-1,&tmp,0);
	signalfd_siginfo si;
	
	int i;
	for(i=0;i<3;i++)
	{
		if(fork()==0)
		{
			//sleep(1);
			return 0;
		}
	}
	//while(1)sleep(1);
	//sleep(2);
	while(read(fd,&si,sizeof(si))>0)
	{
		pid_t pid;
		while((pid=waitpid(-1, NULL, WNOHANG))>0)
			cout << "SIGCHLD: " << pid << endl;
	}
}
