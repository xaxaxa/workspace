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
#include <fcntl.h>

namespace ukernel
{
	namespace core
	{
		using namespace FS;
		int KernelInstance::sys_open(const char* path, int flags) {
			auto tmp = this->open(path, current->fsc->cwd, flags);
			if (tmp) {
				return allocateFD(tmp());
			} else return -tmp.error;
		}
		int KernelInstance::sys_openat(int fd, const char* path, int flags) {
			Path wd;
			if (fd == AT_FDCWD) wd = current->fsc->cwd;
			else {
				FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
				if (f == nullptr) return -EBADF;
				if (f->type != FileDescription::T_File) return -EINVAL;
				FileDescription_File* fdf = (FileDescription_File*) f;
				wd = fdf->path;
			}
			auto tmp = this->open(path, wd, flags);
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
		int KernelInstance::sys_getdents(int fd, FS::linux_dirent64* buf, int size) {
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return -EBADF;
			auto tmp = f->getdents(buf, size);
			if (!tmp) return ENOTSUP;
			if (tmp.error != 0) return -tmp.error;
			return tmp();
		}
		int KernelInstance::sys_write(int fd, const void* buf, size_t len) {
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return -EBADF;
			return f->swrite((u8*) buf, (s64) len);
		}
		int KernelInstance::sys_fstat(int fd, struct stat *buf) {
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return -EBADF;
			return f->stat(buf);
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
		int KernelInstance::sys_fstatfs(int fd, struct statfs *buf) {
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return -EBADF;
			if (f->type != FileDescription::T_File) return -EINVAL;
			FileDescription_File* fdf = (FileDescription_File*) f;
			return vfs.statfs(fdf->path, *buf);
		}
		int KernelInstance::sys_statfs(const char *path, struct statfs *buf) {
			auto tmp = iterativeLookup(path);
			syncWait(tmp, current->eventQueue);
			if (tmp.error != 0) return -tmp.error;
			return vfs.statfs(tmp.value, *buf);
		}
		void* KernelInstance::sys_mmap(void *addr, size_t length, int prot, int flags, int fd,
				off_t offset) {
			return sys_mmap2(addr, length, prot, flags, fd, offset / PAGESIZE);
		}
		void* KernelInstance::sys_mmap2(void *addr, size_t length, int prot, int flags, int fd,
				off_t pgoffset) {
			if ((flags & MAP_ANONYMOUS) || (flags & MAP_ANON)) {
				u8* tmp;
				if ((tmp = (u8*) mmap(addr, length, prot, flags, fd, pgoffset * PAGESIZE)) == MAP_FAILED) return TO_MMAP_ERROR(
						errno);
				else return tmp;
			}
			FileDescription* f = (FileDescription*) current->fdt->fds.find(fd);
			if (f == nullptr) return MAP_FAILED;
			if (f->type != FileDescription::T_File) return MAP_FAILED;
			FileDescription_File* fdf = (FileDescription_File*) f;
			return vfs.mmap(fdf->path, (u8*) addr, (ptr_int) length, s64(pgoffset) * PAGESIZE, prot,
					flags);
		}
		int KernelInstance::sys_mprotect(void *addr, size_t length, int prot) {
			int tmp;
			if ((tmp = mprotect(addr, length, prot)) < 0) return -errno;
			else return tmp;
		}
		int KernelInstance::sys_munmap(void *addr, size_t length) {
			int tmp;
			if ((tmp = munmap(addr, length)) < 0) return -errno;
			else return tmp;
		}
		int KernelInstance::sys_nanosleep(const struct timespec *req, struct timespec *rem) {
			int tmp;
			if ((tmp = nanosleep(req, rem)) < 0) return -errno;
			else return tmp;
		}
		void KernelInstance::sys_exit(int ret) {
			arch::exitProcessImage(ret);
		}
	}
}
