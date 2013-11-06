/*
 * hostfs.C
 *
 *  Created on: Oct 31, 2013
 *      Author: xaxaxa
 */
#include "include/hostfs.H"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

namespace ukernel
{
	namespace FS
	{
		class HostFSI: public FSInstance
		{
		public:
			string hostRoot;
			struct HostFS_Directory: public Directory
			{
				string path;
				int fd;
			};
			struct HostFS_File: public File
			{
				string path;
				int fd;
				~HostFS_File() {
					close(fd);
				}
			};
			HostFS_Directory _root;
			AsyncValue<Object*> lookup(Directory* parent, string name) override {
				HostFS_Directory* d = (HostFS_Directory*) parent;
				struct stat st;
				string p = d->path + name;
				if (fstatat(d->fd, name.c_str(), &st, 0) < 0) return ErrorValue(errno);
				else {
					if (S_ISDIR(st.st_mode)) {
						int fd = openat(d->fd, name.c_str(), O_RDONLY);
						if (fd < 0) return ErrorValue(errno);
						HostFS_Directory* newDir = New<HostFS_Directory>();
						newDir->type = Object::T_Directory;
						newDir->path = p + "/";
						newDir->fd = fd;
						newDir->info = st;
						return newDir;
					} else {
						int fd = openat(d->fd, name.c_str(), O_RDWR);
						if (fd < 0) fd = openat(d->fd, name.c_str(), O_RDONLY);
						if (fd < 0) return ErrorValue(errno);
						HostFS_File* f = New<HostFS_File>();
						f->type = Object::T_File;
						f->path = p;
						f->fd = fd;
						f->info = st;
						return f;
					}
				}
			}
			AsyncValue<int> read(File* f, u8* buffer, s64 offset, s64 length) override {
				HostFS_File* file = (HostFS_File*) f;
				int tmp = ::pread(file->fd, buffer, length, offset);
				return (tmp < 0) ? ErrorValue(errno) : AsyncValue<int>(tmp);
			}
			AsyncValue<int> write(File* f, u8* buffer, s64 offset, s64 length) override {
				HostFS_File* file = (HostFS_File*) f;
				int tmp = ::pwrite(file->fd, buffer, length, offset);
				return (tmp < 0) ? ErrorValue(errno) : AsyncValue<int>(tmp);
			}
			WithError<u8*> mmap(File* f, u8* addr, ptr_int length, s64 offset, int prot, int flags)
					override {
				HostFS_File* file = (HostFS_File*) f;
				u8* tmp = (u8*) ::mmap(addr, length, prot, flags, file->fd, (off_t) offset);
				return tmp == nullptr ? ErrorValue(errno) : WithError<u8*>(tmp);
			}
			int init() {
				if (hostRoot.length() == 0 || hostRoot[hostRoot.length() - 1] != '/') hostRoot += '/';
				_root.path = hostRoot;
				if (stat(hostRoot.c_str(), &_root.info) < 0) return errno;
				if ((_root.fd = open(hostRoot.c_str(), O_RDONLY)) < 0) return errno;
				root = &_root;
				this->initRootDentry();
				this->flags = F_SUPPORTS_MMAP;
				return 0;
			}
		};
		AsyncValue<FSInstance*> HostFS::mount(string dev, string opts) {
			HostFSI* fsi = New<HostFSI>();
			fsi->hostRoot = dev;
			int i;
			if ((i = fsi->init()) != 0) return ErrorValue(i);
			return fsi;
		}
	}
}
