/*
 * ukernel: userspace kernel
 */

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
			WithError<FileDescription*> open(string path, int flags);
			int allocateFD(FileDescription* fd);
			AsyncValue<FS::Path> iterativeLookup(string path) {
				return vfs.iterativeLookup(current->fsc->root, current->fsc->cwd, path);
			}
			int mount(string path, FS::FSInstance* fsi);

			int sys_open(const char* path, int flags);
			int sys_close(int fd);
			int sys_read(int fd, void* buf, size_t len);
			int sys_write(int fd, const void* buf, size_t len);
			int sys_fstat(int fd, struct stat *buf);
			int sys_stat(const char *path, struct stat *buf);
			int sys_lstat(const char *path, struct stat *buf);
			void* sys_mmap2(void *addr, size_t length, int prot, int flags, int fd, off_t pgoffset);
			int sys_munmap(void *addr, size_t length);
			int sys_nanosleep(const struct timespec *req, struct timespec *rem);
		};
	}
}

#endif /* KERNEL_H_ */
