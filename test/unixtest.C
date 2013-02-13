#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

int main() {
	int socks[2];
	socketpair(AF_UNIX,SOCK_STREAM|SOCK_NONBLOCK,0,socks);
	char buf[4096];
	memset(buf,69,4096);
	while(true) {
		int r=send(socks[0],buf,4096,0);
		printf("sent %i bytes of data\n",r);
		if(r<=0) {
			printf("errno: %s\n", strerror(errno));
			break;
		}
	}
	return 0;
}
