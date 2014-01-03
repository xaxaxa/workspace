/*
 * path.H
 *
 *  Created on: Nov 3, 2013
 *      Author: xaxaxa
 */

#ifndef PATH_H_
#define PATH_H_

namespace ukernel
{
	namespace FS
	{
		struct FSInstance;
		struct DEntry;
		struct Path
		{
			FSInstance* fsi;
			DEntry* dentry;
			Path() {
			}
			Path(nullptr_t) :
					fsi(nullptr), dentry(nullptr) {
			}
			Path(FSInstance* fsi, DEntry* dentry) :
					fsi(fsi), dentry(dentry) {
			}
			bool operator==(nullptr_t) const {
				return fsi == nullptr && dentry == nullptr;
			}
		};
	}
}

#endif /* PATH_H_ */
