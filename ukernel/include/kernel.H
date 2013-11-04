/*
 * kernel.H
 *
 *  Created on: Nov 2, 2013
 *      Author: xaxaxa
 */

#ifndef KERNEL_H_
#define KERNEL_H_
#include "core.H"
#include "vfs.H"
#include "process.H"
namespace ukernel
{
	namespace core
	{
		struct KernelInstance
		{
			FS::VFS vfs;
			IDSet processes;
			//assigns current process as pid 1 and does initialization work
			void initialSetup(FS::FSInstance* rootfs);
			Process* createProcess(void* (*func)(void*), void* userdata);
			FileDescription* createFD(FS::Path p);
		};
	}
}

#endif /* KERNEL_H_ */
