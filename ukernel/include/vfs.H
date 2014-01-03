/*
 * vfs.H
 *
 *  Created on: Nov 2, 2013
 *      Author: xaxaxa
 */

#ifndef VFS_H_
#define VFS_H_
#include "core.H"
#include "path.H"
#include <sys/statfs.h>
#include <dirent.h>

namespace ukernel
{
	namespace FS
	{
		using namespace core;
		typedef u64 ID;
		struct Directory;
		struct FSInstance;
		struct Object;
		struct DEntry;

		//represents a file or directory
		//same role as struct inode in linux
		struct Object: public RefCount, public ListItemHeader
		{
			ID id;
			enum
			{
				T_File = 1, T_Directory, T_Link, T_Special
			} type;
			struct stat info;
			Object() {
			}
			virtual ~Object() {
			}
		};
		//represents all fs objects except directories
		struct File: public Object
		{
			virtual ~File() {
			}
		};
		struct Directory: public Object
		{
			virtual ~Directory() {
			}
		};
		struct DEntry: public RefCount, public ListItemHeader
		{
			string name;
			//if target is NULL, this is a negative entry
			Object* target;
			DEntry* parent;
			struct cacheEntry
			{
				u64 id; //may be (u64)-1, which means there is no cached directory listing
				DEntry* obj;
				cacheEntry() :
						id((u64) -1), obj(nullptr) {
				}
			};
			//the map may contain NULL entries, also representing negative entries, unless id
			//is (u64)-1
			map<string, cacheEntry> cachedChildren;
			u32 flags;
			enum
			{
				F_MountPoint = 1, F_Mount //this is the root of a mount
			};
		};
		//not the same thing as DEntry
		struct DirectoryEntry
		{
			u64 id;
			string name;
		};
		struct FSInstance: public RefCount
		{
			//used by vfs
			DEntry rootDentry;
			//set by fs-driver to the root inode at mount time
			Object* root;
			struct statfs info;
			void initRootDentry() {
				rootDentry.target = root;
			}
			s32 flags;
			enum
			{
				F_SUPPORTS_MMAP = 1 //mmap passthrough
			};
			virtual AsyncValue<Object*> lookup(Directory* parent, string name)=0;
			//FILESYSTEM IMPLEMENTATIONS MUST NEVER DO PARTIAL READS/WRITES UNLESS AN
			//EXCEPTIONAL SITUATION IS ENCOUNTERED (i/o error, eof, etc)
			virtual AsyncValue<int> read(File* f, u8* buffer, s64 offset, s64 length)=0;
			virtual AsyncValue<int> write(File* f, u8* buffer, s64 offset, s64 length)=0;
			virtual WithError<u8*> mmap(File* f, u8* addr, ptr_int length, s64 offset, int prot,
					int flags) {
				return ErrorValue(ENOSYS);
			}
			virtual AsyncValue<int> readDir(Directory* d, DirectoryEntry* buf, u64 previousID,
					int bufLength)=0;
			FSInstance();
			virtual ~FSInstance() {
			}
		};
		struct Filesystem
		{
			virtual AsyncValue<FSInstance*> mount(string dev, string opts)=0;
			virtual ~Filesystem() {
			}
		};
		//handles mountpoint management, caching, etc
		struct VFS
		{
			List<Object> unreferencedObjects;
			List<DEntry> unreferencedDentries;
			map<DEntry*, Path> mountPoints;
			map<DEntry*, Path> mounts;
			DEntry _dentrySentinel[0]; //if any DEntry::cachedChildren entries point
			//to this, then it is a partial cache entry (only contains name)
			AsyncValue<Path> lookup(Path d, string name);
			AsyncValue<Path> iterativeLookup(Path root, Path cwd, string path);
			AsyncValue<int> read(Path f, u8* buffer, s64 offset, s64 length);
			AsyncValue<int> write(Path f, u8* buffer, s64 offset, s64 length);
			//reads directory entries after previousID; specify 0 to read from beginning
			//excludes the entry at previousID
			AsyncValue<int> readDir(Path d, DirectoryEntry* buf, u64 previousID, int bufLength);
			int stat(Path p, struct stat& st);
			int statfs(Path p, struct statfs& st);
			u8* mmap(Path f, u8* addr, ptr_int length, s64 offset, int prot, int flags);
			void retainObject(Object* o) {
				if (o->retain() == 2) {
					unreferencedObjects.remove(o);
				}
			}
			void releaseObject(Object* o) {
				if (o->release() == 1) {
					unreferencedObjects.append(o);
				}
			}
			void addExistingObject(Object* o) {
			}
			void retainDentry(DEntry* d) {
				if (d->retain() == 2) {
					unreferencedDentries.remove(d);
					retainObject(d->target);
				}
			}
			void releaseDentry(DEntry* d) {
				if (d->release() == 1) {
					unreferencedDentries.append(d);
					releaseObject(d->target);
				}
			}
			void addRoot(Path root);
			void mount(Path mountPoint, Path newPath);
		};
		struct linux_dirent64
		{
			u64 d_ino;
			s64 d_off;
			u16 d_reclen;
			u8 d_type;
			char d_name[];
		};
	}
}

#endif /* VFS_H_ */
