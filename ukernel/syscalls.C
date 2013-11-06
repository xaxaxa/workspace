/*
 * syscalls.C
 *
 *  Created on: Nov 5, 2013
 *      Author: xaxaxa
 */
#include "include/kernel.H"
#include <time.h>
#include <sys/mman.h>
#include "arch/include/misc.H"

namespace ukernel
{
	namespace core
	{
		int KernelInstance::sys_open(const char* path, int flags) {
			auto tmp = this->open(path, flags);
			if (tmp) {
				return allocateFD(tmp());
			} else return -tmp.error;
		}
		int KernelInstance::sys_close(int fd) {
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f != nullptr) {
				f->release();
				current->fdt->fds.remove(fd);
				return 0;
			}
			return -EBADF;
		}
		int KernelInstance::sys_read(int fd, void* buf, size_t len) {
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return -EBADF;
			return f->sread((u8*) buf, (s64) len);
		}
		int KernelInstance::sys_write(int fd, const void* buf, size_t len) {
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return -EBADF;
			return f->swrite((u8*) buf, (s64) len);
		}
		int KernelInstance::sys_fstat(int fd, struct stat *buf) {
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return -EBADF;
			if (f->type != FileDescription::T_File) return -EINVAL;
			FileDescription_File* fdf = (FileDescription_File*) f;
			return vfs.stat(fdf->path, *buf);
		}
		int KernelInstance::sys_stat(const char *path, struct stat *buf) {
			auto tmp = iterativeLookup(path);
			syncWait(tmp, current->eventQueue);
			if (tmp.error != 0) return -tmp.error;
			return vfs.stat(tmp.value, *buf);
		}
		int KernelInstance::sys_lstat(const char *path, struct stat *buf) {
			return sys_stat(path, buf);
		}
		void* KernelInstance::sys_mmap2(void *addr, size_t length, int prot, int flags, int fd,
				off_t pgoffset) {
			if ((flags & MAP_ANONYMOUS) || (flags & MAP_ANON)) {
				return mmap(addr, length, prot, flags, fd, pgoffset * PAGESIZE);
			}
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return MAP_FAILED;
			if (f->type != FileDescription::T_File) return MAP_FAILED;
			FileDescription_File* fdf = (FileDescription_File*) f;
			return vfs.mmap(fdf->path, (u8*) addr, (ptr_int) length, s64(pgoffset) * PAGESIZE, prot,
					flags);
		}
		int KernelInstance::sys_munmap(void *addr, size_t length) {
			return munmap(addr, length);
		}
		int KernelInstance::sys_nanosleep(const struct timespec *req, struct timespec *rem) {
			return nanosleep(req, rem);
		}
	}
}
