//run_tasks: generic command line task queue

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void doSpawn(const char* cmd)
{
	pid_t pid=fork();
	if(pid==0) {
		const char* v[4]={"/bin/sh","-c",cmd,(char*)NULL};
		execv("/bin/sh",(char* const*)v);
		perror("execv");
		_exit(-1);
	} else if(pid<0) {
		perror("fork");
	} else {
		printf("issuing pid %i: %s",(int)pid,cmd);
	}
}
bool doWait() {
	int s;
	pid_t pid=wait(&s);
	if(pid>0) {
		printf("finished pid %i with exit code %i\n",(int)pid,s);
		return true;
	}
	return false;
}
int main(int argc, char** argv) {
	if(argc<2) {
		printf("usage: %s N\n\tN: number of tasks to run in parallel\n",argv[0]);
		return 1;
	}
	int curTasks=0;
	int maxTasks=atoi(argv[1]);
	while(true) {
		if(curTasks<maxTasks) {
			char* line=NULL;
			size_t n;
			if(getline(&line,&n,stdin)<0) break;
			doSpawn(line);
			curTasks++;
		} else {
			if(doWait()) curTasks--;
		}
	}
	while(doWait());
	return 0;
}
