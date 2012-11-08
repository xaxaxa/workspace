#include <libvirt/libvirt.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	if(argc<2) {
		printf("usage: %s uri\n", argv[0]);
		return 1;
	}
	virConnectPtr conn=virConnectOpen(argv[1]);
	int l=virConnectNumOfDefinedDomains(conn);
	char* arr[l];
	l=virConnectListDefinedDomains(conn, arr, l);
	for(int i=0;i<l;i++) {
		printf("%s\n", arr[i]);
	}
	virConnectClose(conn);
}
