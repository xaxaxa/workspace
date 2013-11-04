/*
 * process.H
 *
 *  Created on: Nov 2, 2013
 *      Author: xaxaxa
 */
#include "../../include/core.H"
namespace ukernel
{
	namespace arch
	{
		class ProcessImage
		{
			//call the child process image; function does not
			//return until the child calls exit() below
			void call();
			//exit the process image context, and return from call()
			void exit();
		};
		ProcessImage* loadProcess(core::FileDescription* fd);
	}
}
