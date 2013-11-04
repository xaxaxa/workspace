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

namespace ukernel
{
	namespace FS
	{

		class HostFSI: public FSInstance
		{
		public:
			string hostRoot;
			class HostFS_Directory: public Directory
			{
			public:
				string path;
			};
			class HostFS_File: public File
			{
			public:
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
				if (stat(p.c_str(), &st) < 0) return nullptr;
				else {
					if (S_ISDIR(st.st_mode)) {
						HostFS_Directory* newDir = New<HostFS_Directory>();
						newDir->path = p + "/";
						newDir->info = st;
						return newDir;
					} else {
						int fd = open(p.c_str(), O_RDWR);
						if (fd < 0) fd = open(p.c_str(), O_RDONLY);
						if (fd < 0) return NULL;
						HostFS_File* f = New<HostFS_File>();
						f->path = p + "/";
						f->fd = fd;
						f->info = st;
						return f;
					}
				}
			}
			AsyncValue<int> read(File* f, u8* buffer, s64 offset, s64 length) override {
				HostFS_File* file = (HostFS_File*) f;
				return ::pread(file->fd, buffer, length, offset);
			}
			AsyncValue<int> write(File* f, u8* buffer, s64 offset, s64 length) override {
				HostFS_File* file = (HostFS_File*) f;
				return ::pwrite(file->fd, buffer, length, offset);
			}
			void init() {
				if (hostRoot.length() == 0 || hostRoot[hostRoot.length() - 1] != '/') hostRoot += '/';
				_root.path = hostRoot;
				root = &_root;
				this->initRootDentry();
			}
		};
		AsyncValue<FSInstance*> HostFS::mount(string dev, string opts) {
			HostFSI* fsi = New<HostFSI>();
			fsi->hostRoot = dev;
			fsi->init();
			return fsi;
		}
	}
}
