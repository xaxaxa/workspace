/*
 * hostfs.H
 *
 *  Created on: Oct 31, 2013
 *      Author: xaxaxa
 */

#ifndef HOSTFS_H_
#define HOSTFS_H_
#include "vfs.H"
namespace ukernel
{
	namespace FS
	{
		struct HostFS: public Filesystem
		{
			AsyncValue<FSInstance*> mount(string dev, string opts) override;
		};
	}
}
#endif /* HOSTFS_H_ */
