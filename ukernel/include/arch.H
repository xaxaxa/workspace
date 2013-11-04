#ifndef UKERNEL_ARCH_H
#define UKERNEL_ARCH_H

//interfaces to architecture-specific code
namespace ukernel
{
	namespace arch
	{
		class ProcessImage;
	/*
	 ProcessImage interface:
	 class ProcessImage
	 {
	 //call the child process image; function does not
	 //return until the child calls exit() below
	 void call();
	 //exit the process image context, and return from call()
	 void exit();
	 };

	 */
	}
}
#endif
