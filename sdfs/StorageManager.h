/*
 * StorageManager.h
 *
 *  Created on: 2011-11-15
 *      Author: user1
 */

#ifndef STORAGEMANAGER_H_
#define STORAGEMANAGER_H_
#include <string>
#include <sys/types.h>
#include <time.h>
#include <cplib/cplib.hpp>
#include "IStorage.h"
#include "CacheManager.h"
#define FUSE_USE_VERSION 27
#include <fuse/fuse_lowlevel.h>
using namespace std;
using namespace xaxaxa;
#define SDFS_PACK_ID(chunkid,blockindex) (((CID)(chunkid)&0x000000FFFFFFFFFF)|((CID)(blockindex)<<(64-24)))
#define SDFS_UNPACK_ID_CID(id) ((id)&0x000000FFFFFFFFFF)
#define SDFS_UNPACK_ID_BLOCKINDEX(id) ((UInt)(((id)&0xFFFFFF0000000000)>>(64-24)))
#ifdef _64BITFILELENGTH
typedef ULong FILELEN_T;
#else
typedef UInt FILELEN_T;
#endif
namespace sdfs
{

	struct Block
	{
		//block structure
		//	0       32			(length)
		//	_____________________
		//	|length	|data        |
		//	|_______|____________|
		//length includes the size of the length field(32 bits)
		enum class BlockType
:		Byte
		{
			none=0,
			dirindex,	//directory index
			//	0            |(sizeof(dirent)+namelen)|	                                  (block.length)
			//	__________________________________________________________________________
			//	| indexhdr | dirent:[....namelen][name] |         more entries...         |
			//	|__________|____________________________|_________________________________|
			//
			fileindex,//index of file blocks
			//	0                                                       (block.length)
			//	________________________________________________________
			//	| indexhdr | indexent |         more entries...         |
			//	|__________|__________|_________________________________|
			//
			filedata,//a block of data
			//	0                (block.length)
			//	_________________
			//	|      data      |
			//	|________________|
			//
			xattr//extended attributes(ACL's, ...)
			//	0                          (sizeof(xattrent)+namelen+valuelen)	       (block.length)
			//	_______________________________________________________________________
			//	|namelen,valuelen,name,value|              more entries...             |
			//	|___________________________|__________________________________________|
			//
		};
		//files and directories do not have "inodes". all stat() info about a file
		//is entirely contained within its parent's directory index.
		//the "inode number" of a file is the block ID of its index.

		struct hdr//block header
		{
			UInt size;
			BlockType type;
			void* getdata()
			{
				return (void*)(this+1);
			}
		};
		struct indexhdr
		{
			CID parent;
			UInt entryoffset;	//offset of the parent dirent from the block
		};
		struct inodehdr
		{
			mode_t s_mode;
			uid_t s_uid;
			gid_t s_gid;
			FILELEN_T s_size;
			time_t s_atime;
			time_t s_mtime;
			time_t s_ctime;
		};
		struct dirent
		{
			struct inodehdr;
			CID indexblock;
			CID firstblock;		//NULL if it's a directory
			Byte namelen;
		};
		struct indexent
		{
			FILELEN_T offset;
			CID id;
			UInt len;
		};
		struct xattrent
		{
			Byte namelen;
			UShort valuelen;
		};
		CacheItemPtr<CID, ChunkData> Data;
		UInt offset;
		inline hdr* RawData()
		{
			return (hdr*) (((Byte*) (Data.Item->Item.data)) + offset);
		}
		inline bool Next()
		{
			hdr* tmp = RawData();
			if (((offset + tmp->size + sizeof(hdr)) <= (Data.Item->Item.size)))
			{
				offset += tmp->size;
				if (offset + RawData()->size > (Data.Item->Item.size))
				{
					//throw OutOfRangeException("FATAL ERROR: corrupt chunk detected");
					offset -= tmp->size;
					return false;
				}
				return true;
			}
			return false;
		}
	};
	struct Chunk
	{
		CacheItemPtr<CID, ChunkData> Data;
		inline Block FirstBlock()
		{
			return Block { Data, 0 };
		}
	};
	class StorageManager
	{
	public:
		ArrayList<IStorage*> stores;
		StorageManager();
		virtual ~StorageManager();
		CacheManager<CID,ChunkData> cache;

	};

}

#endif /* STORAGEMANAGER_H_ */
