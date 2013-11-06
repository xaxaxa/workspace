#include "include/kernel.H"
#include "include/hostfs.H"
#include <unistd.h>
#include <fcntl.h>

using namespace ukernel;
using namespace core;
using namespace FS;
int main() {
	KernelInstance kernel;
	HostFS hostfs;
	auto tmp=hostfs.mount("/home/xaxaxa/r","");
	if(!tmp) throw runtime_error("hostfs did not mount immediately");
	if(tmp.error!=0) throw runtime_error(strerror(tmp.error));
	
	FSInstance* rootfs=tmp();
	kernel.initialSetup(rootfs);
	
	kernel.mount("/b",hostfs.mount("/home/xaxaxa","")());
	
	int fd=kernel.sys_open("/b/ipt1",O_RDONLY);
	if(fd>0) {
		char buf[1024];
		int r=kernel.sys_read(fd,(u8*)buf,1024);
		if(r>=0) write(1,buf,r);
		else throw runtime_error(strerror(-r));
	} else {
		throw runtime_error(strerror(-fd));
	}
	return 0;
}
