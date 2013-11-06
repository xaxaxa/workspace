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
namespace ukernel
{
	namespace arch
	{
		typedef Elf32_Ehdr ELFHeader;
		typedef Elf32_Phdr ELFPHeader;
		typedef Elf32_auxv_t ELFAUXV;
		typedef unsigned long int PTR;
		struct executableInfo
		{
			u8* image;
			u8* loaderImage;
			int imageLen, loaderLen;
			int fd, loaderfd;
		};
		struct ProcessImage
		{
			u8* baseAddr;
			//call the child process image; function does not
			//return until the child calls exit() below
			void call();
			//exit the process image context, and return from call()
			void exit();
		};
		ProcessImage* loadProcess(core::KernelInstance* k, string path);
	}
}
#endif
