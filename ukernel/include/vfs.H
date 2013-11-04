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
namespace ukernel
{
	namespace FS
	{
		using namespace core;
		typedef u64 ID;
		struct Directory;
		struct FSInstance;
		struct Object;
		struct DirectoryEntry;

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
		struct DirectoryEntry: public RefCount, public ListItemHeader
		{
			string name;
			//if target is NULL, this is a negative entry
			Object* target;
			DirectoryEntry* parent;
			//the map may contain NULL entries, also representing negative entries
			map<string, DirectoryEntry*> cachedChildren;
			u32 flags;
			enum
			{
				F_MountPoint = 1
			};
		};
		struct FSInstance: public RefCount
		{
			//used by vfs
			DirectoryEntry rootDentry;
			//set by fs-driver to the root inode at mount time
			Object* root;
			void initRootDentry() {
				rootDentry.target = root;
			}
			virtual AsyncValue<Object*> lookup(Directory* parent, string name)=0;
			virtual AsyncValue<int> read(File* f, u8* buffer, s64 offset, s64 length)=0;
			virtual AsyncValue<int> write(File* f, u8* buffer, s64 offset, s64 length)=0;
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
			List<DirectoryEntry> unreferencedDentries;
			map<DirectoryEntry*, Path> mountPoints;
			AsyncValue<Path> lookup(Path d, string name);
			AsyncValue<int> read(Path f, u8* buffer, s64 offset, s64 length);
			AsyncValue<int> write(Path f, u8* buffer, s64 offset, s64 length);
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
			void retainDentry(DirectoryEntry* d) {
				if (d->retain() == 2) {
					unreferencedDentries.remove(d);
					retainObject(d->target);
				}
			}
			void releaseDentry(DirectoryEntry* d) {
				if (d->release() == 1) {
					unreferencedDentries.append(d);
					releaseObject(d->target);
				}
			}
			void addRoot(Path root);
			void mount(Path mountPoint, Path newPath);
		};
	}
}

#endif /* VFS_H_ */
