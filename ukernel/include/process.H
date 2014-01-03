/*
 * process.H
 *
 *  Created on: Nov 2, 2013
 *      Author: xaxaxa
 */

#ifndef PROCESS_H_
#define PROCESS_H_
#include "core.H"
#include "vfs.H"
namespace ukernel
{
	namespace core
	{
		struct ProcessFSContext
		{
			FS::Path root;
			FS::Path cwd;
		};
		//represents a thread of execution
		//maps 1-to-1 to actual pthreads
		struct Process
		{
			kpid id;
			pthread_t _pth;
			EventQueue eventQueue;
			FDTable* fdt;
			ProcessFSContext* fsc;
		};
		//represents a loaded in-memory image
		//of an executable; a process could have
		//multiple images loaded simultaneously,
		//and each image can call each other's functions
		struct ProcessImage;

		extern __thread Process* current;
	}
}

#endif /* PROCESS_H_ */
