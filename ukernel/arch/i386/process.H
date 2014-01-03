/*
 * process.H
 *
 *  Created on: Nov 2, 2013
 *      Author: xaxaxa
 */
#ifndef __ARCH_PROCESS_H
#define __ARCH_PROCESS_H
#include <elf.h>
#include "../../include/core.H"
#include "misc.H"
namespace ukernel
{
	namespace arch
	{
		typedef Elf32_Ehdr ELFHeader;
		typedef Elf32_Phdr ELFPHeader;
		typedef Elf32_auxv_t ELFAUXV;
		typedef unsigned long int PTR;
		static const int stackSize = PAGESIZE * 1024;
		extern ELFAUXV* host_auxv;
		void setHostAuxv(int argc, char** argv);
		struct executableInfo
		{
			u8* image;
			u8* loaderImage;
			int imageLen, loaderLen;
			int fd, loaderfd;
			executableInfo() {
				image = loaderImage = nullptr;
				fd = loaderfd = -1;
				imageLen = loaderLen = 0;
			}
		};
		struct ProcessImage;
		//structure at the top of user stacks
		struct threadInfo
		{
			int kernel_gs;
			//where to return to after exit()
			u8* kernel_return_sp;
			u8* kernel_return_addr;
			int* putExitCode;

			core::KernelInstance* kernel;
			ProcessImage* processImage;
		};
		extern ELFHeader vSyscallHeader;
		struct ProcessImage
		{
			executableInfo inf;
			u8* base;
			u8* loaderBase;
			//switch to "userspace" and call the child process image; function does not
			//return until the child calls exit() below
			//returns exit code, or -error
			int call(core::KernelInstance* k, int argc, int envc, const char** argv,
					const char** envv);
		};
		//exit the current process image context, and return from call()
		void exitProcessImage(int ret);
		WithError<ProcessImage*> loadProcess(core::KernelInstance* k, string path);
		void** initSyscallTable();

		threadInfo* get_threadInfo();
		extern "C" int _handle_syscall();
	}
}
#endif
