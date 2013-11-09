/*
 * core.C
 *
 *  Created on: Oct 31, 2013
 *      Author: xaxaxa
 */
#include "include/kernel.H"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "arch/include/process.H"
#include "arch/include/misc.H"
#include <sys/syscall.h>

namespace ukernel
{
	namespace core
	{
		using namespace FS;
		__thread Process* current;
		inline void EventQueue::signal(Delegate<void()> userData) {
		}
		inline Delegate<void()> EventQueue::wait() {
			return nullptr;
		}
		void EventQueue::processEvent() {
			wait()();
		}
		void KernelInstance::initialSetup(FS::FSInstance* rootfs) {
			Process* p = New<Process>();
			p->id = 1;
			current = p;
			processes.add(1, p);
			p->fdt = New<FDTable>();
			p->fsc = New<ProcessFSContext>();
			p->fsc->cwd = p->fsc->root = FS::Path { rootfs, &rootfs->rootDentry };
			vfs.addRoot(p->fsc->root);
			syscallTable = arch::initSyscallTable();
		}
		Process* KernelInstance::createProcess(void* (*func)(void*), void* userdata) {
			Process* p = New<Process>();
			p->fdt = nullptr;
			p->fsc = nullptr;
			p->id = (kpid) processes.add(p);
			if (pthread_create(&p->_pth, nullptr, func, userdata) < 0) {
				Delete(p);
				return nullptr;
			}
			return p;
		}
		int FileDescription::sread(u8* buffer, s64 length) {
			auto tmp = read(buffer, length);
			if (tmp) return tmp.error == 0 ? tmp() : -tmp.error;
			else {
				SyncWaiter<int> w(tmp, current->eventQueue);
				return w.error == 0 ? w.value : -w.error;
			}
		}
		int FileDescription::swrite(u8* buffer, s64 length) {
			auto tmp = write(buffer, length);
			if (tmp) return tmp();
			else {
				SyncWaiter<int> w(tmp, current->eventQueue);
				return w.error == 0 ? w.value : -w.error;
			}
		}
		AsyncValue<int> FileDescription_File::read(u8* buffer, s64 length) {
			return kernel->vfs.read(path, buffer, __sync_fetch_and_add(&offset, length), length);
		}
		AsyncValue<int> FileDescription_File::write(u8* buffer, s64 length) {
			return kernel->vfs.write(path, buffer, __sync_fetch_and_add(&offset, length), length);
		}
		AsyncValue<int> FileDescription_File::getdents(linux_dirent64* buffer, int bufSize) {
			int bufOffset = 0;
			while (true) {
				DirectoryEntry tmp;
				auto r = this->kernel->vfs.readDir(this->path, &tmp, this->offset, 1);
				if (!r) return ErrorValue(ENOTSUP);
				if (r.error != 0) return ErrorValue(r.error);
				if (r() < 1) break;
				int sz = offsetof(linux_dirent64,d_name) + tmp.name.length() + 1;
				if (sz > (bufSize - bufOffset)) break;
				linux_dirent64* aaa = (linux_dirent64*) ((u8*) (buffer) + bufOffset);
				aaa->d_ino = this->offset + 1;
				aaa->d_off = this->offset;
				aaa->d_reclen = sz;
				aaa->d_type = 0;
				memcpy(aaa->d_name, tmp.name.c_str(), tmp.name.length() + 1);
				bufOffset += sz;
				this->offset = tmp.id;
			}
			return bufOffset;
			/*struct state
			 {
			 VFS* vfs;
			 Path dir;
			 linux_dirent64* buffer;
			 int bufSize;
			 int bufOffset;
			 u64 offset;
			 DirectoryEntry tmp;
			 Delegate<void(int, int)> cb;
			 void advance() {

			 }
			 int run() {
			 while (true) {
			 auto r = vfs->readDir(dir, &tmp, offset, 1);
			 if (r) {
			 if (r.error == 0) {
			 advance();
			 continue;
			 } else return r.error;
			 }
			 r.wait(this);
			 return -1;
			 }
			 }
			 void operator()(int i, int err) {
			 if (err != 0) cb(0, err);
			 else {
			 err = run();
			 if (err > 0) cb(0, err);
			 else if (err == 0) cb(bufOffset, 0);
			 }
			 }
			 };*/
		}
		int FileDescription_File::stat(struct stat *buf) {
			return kernel->vfs.stat(path, *buf);
		}
		u8* FileDescription_File::mmap(u8* addr, ptr_int length, s64 offset, int prot, int flags) {
			return kernel->vfs.mmap(path, addr, length, offset, prot, flags);
		}
		FileDescription_File::FileDescription_File() {
			offset = 0;
			flags = 0;
		}
		FileDescription_File::~FileDescription_File() {
			kernel->vfs.releaseDentry(path.dentry);
		}

		AsyncValue<int> FileDescription_HostFD::read(u8* buffer, s64 length) {
			int tmp = ::read(fd, buffer, length);
			if (tmp < 0) return ErrorValue(errno);
			else return tmp;
		}
		AsyncValue<int> FileDescription_HostFD::write(u8* buffer, s64 length) {
			int tmp = ::write(fd, buffer, length);
			if (tmp < 0) return ErrorValue(errno);
			else return tmp;
		}
		AsyncValue<int> FileDescription_HostFD::getdents(FS::linux_dirent64* buffer, int bufSize) {
			return syscall(SYS_getdents64, fd, buffer, bufSize);
		}
		int FileDescription_HostFD::stat(struct stat *buf) {
			return fstat(fd, buf);
		}
		u8* FileDescription_HostFD::mmap(u8* addr, ptr_int length, s64 offset, int prot, int flags) {
			u8* tmp = (u8*) ::mmap(addr, length, prot, flags, fd, offset);
			if (tmp == MAP_FAILED) return TO_MMAP_ERROR(errno);
			else return tmp;
		}
		FileDescription_HostFD::FileDescription_HostFD(int fd, bool noClose) {
			this->fd = fd;
			this->noClose = noClose;
		}
		FileDescription_HostFD::~FileDescription_HostFD() {
			if (!noClose) close(fd);
		}
		FileDescription* KernelInstance::createFD(FS::Path p) {
			FileDescription_File* fd = New<FileDescription_File>();
			fd->path = p;
			fd->kernel = this;
			fd->type = FileDescription::T_File;
			this->vfs.retainDentry(p.dentry);
			return fd;
		}
		WithError<FileDescription*> KernelInstance::open(string path, Path cwd, int flags) {
			auto p = vfs.iterativeLookup(current->fsc->root, cwd, path);
			if (p) {
				if (p.error == 0) return createFD(p());
				else return ErrorValue(p.error);
			} else {
				SyncWaiter<FS::Path> w(p, current->eventQueue);
				if (w.error == 0) return createFD(w.value);
				else return ErrorValue(w.error);
			}
		}
		int KernelInstance::allocateFD(FileDescription* fd) {
			fd->retain();
			return current->fdt->fds.add(fd);
		}
		int KernelInstance::allocateFD(FileDescription* fd, int fdnum) {
			fd->retain();
			void* tmp = current->fdt->fds.find(fdnum);
			if (tmp != nullptr) {
				((FileDescription*) tmp)->release();
				current->fdt->fds.remove(fdnum);
			}
			return current->fdt->fds.add(fdnum, fd) ? 0 : -EEXIST;
		}
		int KernelInstance::mount(string path, FSInstance* fsi) {
			auto tmp = iterativeLookup(path);
			syncWait(tmp, current->eventQueue);
			if (tmp.error != 0) return -tmp.error;
			vfs.mount(tmp(), Path(fsi, &fsi->rootDentry));
			return 0;
		}
		int KernelInstance::callExecutable(string path, int argc, int envc, const char** argv,
				const char** envv) {
			auto tmp = arch::loadProcess(this, path);
			if (!tmp) return -tmp.error;
			int ret = tmp()->call(this, argc, envc, argv, envv);
			Delete(tmp());
			return ret;
		}
	}
	namespace FS
	{
		FSInstance::FSInstance() {
			flags = 0;
		}
		inline static Path VFS_resolveMountPoint(VFS* vfs, Path p) {
			while (p.dentry->flags & DEntry::F_MountPoint)
				p = vfs->mountPoints[p.dentry];
			return p;
		}
		inline static Path VFS_resolveParentMountPoint(VFS* vfs, Path root, Path p) {
			while (p.dentry->flags & DEntry::F_Mount && p.dentry != root.dentry)
				p = vfs->mounts[p.dentry];
			return p;
		}
		AsyncValue<Path> VFS::lookup(Path parentPath, string name) {
			DEntry* parentD = parentPath.dentry;
			Directory* parent = (Directory*) parentD->target;
			auto it = parentD->cachedChildren.find(name);
			if (it == parentD->cachedChildren.end()) {
				aaaaa: auto tmp = parentPath.fsi->lookup(parent, name);
				struct CB
				{
					VFS* This;
					Path parentPath;
					string name;
					Delegate<void(Path, int)> cb;
					Path asdf(Object* o, int err) {
						if (err != 0) return nullptr;
						DEntry* parentD = parentPath.dentry;
						DEntry* childD;
						if (o == nullptr) childD = nullptr;
						else {
							//allocate new dentry, and put it in cache
							childD = New<DEntry>();
							childD->flags = 0;
							childD->name = name;
							childD->parent = parentD;
							childD->target = o;
						}
						parentD->cachedChildren[name].obj = childD;
						if (childD == nullptr) return nullptr;
						return VFS_resolveMountPoint(This, Path { parentPath.fsi, childD });
					}
					void operator()(Object* o, int err) {
						if (err != 0) cb(nullptr, err);
						else cb(asdf(o, 0), 0);
					}
				};
				if (tmp) {
					if (tmp.error != 0) return ErrorValue(tmp.error);
					CB cb { this, parentPath, name };
					return cb.asdf(tmp(), 0);
				} else {
					CB* cb = new CB { this, parentPath, name };
					tmp.wait(cb);
					return Future<Path>(&cb->cb);
				}
			} else {
				if ((*it).second.obj == nullptr) {
					if ((*it).second.id == (u64) -1) return Path(nullptr);
					goto aaaaa;
				}
				return VFS_resolveMountPoint(this, Path { parentPath.fsi, (*it).second.obj });
			}
		}
		struct split
		{
			string s;
			int valuePos;
			int valueLen;
			int pos;
			char delim;
			split() :
					pos(0) {
			}
			bool read() {
				if (pos >= (int) s.length()) return false;
				valuePos = pos;
				u8* tmp = (u8*) memchr(s.data() + pos, delim, s.length() - pos);
				pos = (tmp == nullptr) ? int(s.length()) : int(tmp - (u8*) s.data());
				valueLen = pos - valuePos;
				if (tmp != nullptr) pos++;
				return true;
			}
			string value() {
				return s.substr(valuePos, valueLen);
			}
		};
		struct VFS_iterative_lookup_state
		{
			VFS* vfs;
			split s;
			Path current;
			Path root;
			Delegate<void(Path, int)> cb;
			bool advance(Path lookupResult) {
				current = lookupResult;
				return s.read();
			}
			//returns error code
			int doLoop() {
				while (true) {
					if (s.value() == ".") {
						if (!s.read()) return 0;
						continue;
					}
					if (s.value() == "..") {
						current = VFS_resolveParentMountPoint(vfs, root, current);
						if (current.dentry != root.dentry) {
							current.dentry = current.dentry->parent;
						}
						if (!s.read()) return 0;
						continue;
					}
					auto tmp = vfs->lookup(current, s.value());
					if (tmp) {
						if (tmp.error == 0) {
							if (!advance(tmp())) return 0;
						} else return tmp.error;
					} else {
						tmp.wait( { &VFS_iterative_lookup_state::lookupCB, this });
						return -1;
					}
				}
			}
			void _callCB(int e) {
				Path p = current;
				Delegate<void(Path, int)> cb1 = cb;
				Delete(this);
				cb1(p, e);
			}
			void lookupCB(Path r, int e) {
				if (e == 0) {
					if (advance(r)) {
						e = doLoop();
						if (e != -1) _callCB(e);
					} else _callCB(0);
				} else _callCB(e);
			}
		};
		AsyncValue<Path> VFS::iterativeLookup(Path root, Path cwd, string path) {
			VFS_iterative_lookup_state* _st = New<VFS_iterative_lookup_state>();
			VFS_iterative_lookup_state& st(*_st);
			st.vfs = this;
			st.s.s = path;
			st.s.delim = '/';
			st.s.read();
			string component = st.s.s.substr(st.s.valuePos, st.s.valueLen);
			if (component == ".") {
				st.current = cwd;
				if (!st.s.read()) {
					Delete(_st);
					return cwd;
				}
			} else if (component.length() == 0) {
				st.current = root;
				if (!st.s.read()) {
					Delete(_st);
					return root;
				}
			} else st.current = cwd;
			int e = st.doLoop();
			if (e == -1) {
				return Future<Path>(&st.cb);
			} else if (e == 0) {
				Path p = st.current;
				Delete(_st);
				return p;
			} else {
				Delete(_st);
				return ErrorValue(e);
			}
		}
		AsyncValue<int> VFS::read(Path f, u8* buffer, s64 offset, s64 length) {
			if (f.dentry->target->type != Object::T_File) return ErrorValue(EISDIR);
			return f.fsi->read((File*) f.dentry->target, buffer, offset, length);
		}
		AsyncValue<int> VFS::write(Path f, u8* buffer, s64 offset, s64 length) {
			if (f.dentry->target->type != Object::T_File) return ErrorValue(EISDIR);
			return f.fsi->write((File*) f.dentry->target, buffer, offset, length);
		}
		AsyncValue<int> VFS::readDir(Path d, DirectoryEntry* buf, u64 previousID, int bufLength) {
			if (d.dentry->target->type != Object::T_Directory) return ErrorValue(ENOTDIR);
			return d.fsi->readDir((Directory*) d.dentry->target, buf, previousID, bufLength);
		}
		int VFS::stat(Path p, struct stat& st) {
			st = p.dentry->target->info;
			return 0;
		}
		int VFS::statfs(Path p, struct statfs& st) {
			st = p.fsi->info;
			return 0;
		}
		u8* VFS::mmap(Path f, u8* addr, ptr_int length, s64 offset, int prot, int flags) {
			if (f.fsi->flags & FSInstance::F_SUPPORTS_MMAP) {
				auto tmp = f.fsi->mmap((File*) f.dentry->target, addr, length, offset, prot, flags);
				if (tmp) return tmp();
				else return TO_MMAP_ERROR(tmp.error);
			}
			//XXX: implement mmap emulation by mapping in PROT_NONE pages;
			//when segfault occurs, map anonymous pages on top, then read() to
			//those anonymous pages
			return nullptr;
		}
		void VFS::addRoot(Path root) {
			mountPoints.insert(make_pair(root.dentry, root));
			root.fsi->retain();
			root.dentry->retain();
		}
		void VFS::mount(Path mountPoint, Path newPath) {
			if (mountPoint.dentry->flags & DEntry::F_MountPoint) {
				WARN("attempted to mount to a path that is already a mount point");
				return;
			}
			newPath.fsi->retain();
			newPath.dentry->retain();
			mountPoints.insert(make_pair(mountPoint.dentry, newPath));
			mounts.insert(make_pair(newPath.dentry, mountPoint));
			mountPoint.dentry->flags |= DEntry::F_MountPoint;
			newPath.dentry->flags |= DEntry::F_Mount;
		}
	}
}
