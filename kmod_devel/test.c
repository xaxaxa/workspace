#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int fd=open("./troll",O_RDWR);
	if(fd<0) {
		perror("open"); return 1;
	}
	ioctl(fd, 1, NULL);
	perror("ioctl");
	return 0;
}
