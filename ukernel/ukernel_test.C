#include "include/kernel.H"
#include "include/hostfs.H"
#include "arch/include/process.H"
#include <unistd.h>
#include <fcntl.h>

using namespace ukernel;
using namespace core;
using namespace FS;
void setupStdio(KernelInstance& kernel) {
	kernel.allocateFD(RNew<FileDescription_HostFD>(0,true),0);
	kernel.allocateFD(RNew<FileDescription_HostFD>(1,true),1);
	kernel.allocateFD(RNew<FileDescription_HostFD>(2,true),2);
}
int main(int argc, char** argv) {
	arch::setHostAuxv(argc,argv);
	
	KernelInstance kernel;
	HostFS hostfs;
	auto tmp=hostfs.mount("/home/xaxaxa/r","");
	if(!tmp) throw runtime_error("hostfs did not mount immediately");
	if(tmp.error!=0) throw runtime_error(strerror(tmp.error));
	
	FSInstance* rootfs=tmp();
	kernel.initialSetup(rootfs);
	
	kernel.mount("/b",hostfs.mount("/home/xaxaxa","")());
	kernel.mount("/b/r/b",hostfs.mount("/","")());
	kernel.mount("/persist",hostfs.mount("/persist","")());
	kernel.mount("/usr",hostfs.mount("/usr","")());
	kernel.mount("/lib",hostfs.mount("/lib","")());
	kernel.mount("/bin",hostfs.mount("/bin","")());
	
	/*int fd=kernel.sys_open("/b/r/b/home/xaxaxa/ipt1",O_RDONLY);
	if(fd>0) {
		char buf[1024];
		int r=kernel.sys_read(fd,(u8*)buf,1024);
		if(r>=0) write(1,buf,r);
		else throw runtime_error(strerror(-r));
	} else {
		throw runtime_error(strerror(-fd));
	}*/
	{
		setupStdio(kernel);
		const char* args[2]={"cat","/b/hello.c"};
		int err;
		if((err=kernel.callExecutable("/b/cat",2,0,args,nullptr))<0) {
			throw runtime_error(strerror(-err));
		}
		printf("returned from callExecutable; exitcode=%i\n",err);
	}
	{
		setupStdio(kernel);
		const char* args[2]={"ls","/b"};
		int err=kernel.callExecutable("/b/ls",2,0,args,nullptr);
		printf("returned from callExecutable; exitcode=%i\n",err);
	}
	
	return 0;
}
