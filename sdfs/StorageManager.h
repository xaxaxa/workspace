/*
 * StorageManager.h
 *
 *  Created on: 2011-11-15
 *      Author: user1
 */

#ifndef STORAGEMANAGER_H_
#define STORAGEMANAGER_H_
#include <string>
#include <map>
#include <vector>
#include <sys/types.h>
#include <time.h>
#include <cplib/cplib.hpp>
#include "IStorage.h"
#include "CacheManager.h"
#include <boost/random.hpp>
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
	struct Block;
	struct Chunk
	{
		ChunkData Data;
		inline Block FirstBlock();
		inline Chunk()=default;
		inline Chunk(const ChunkData& d):Data(d)
		{}
	};
	struct Block
	{
		//block structure
		//	0       32	 40         	(length)
		//	_____________________________
		//	|length	|type|     data      |
		//	|_______|____|_______________|
		//length includes the size of the length field(32 bits)
		enum class BlockType
:		Byte
		{
			none=0,
			dirindex, //directory index
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
			UInt Size;
			BlockType Type;
			void* GetData()
			{
				return (void*)(this+1);
			}
		};
		struct indexhdr
		{
			CID parent;
			UInt entryoffset; //offset of the parent dirent from the block
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
			CID firstblock; //NULL if it's a directory
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
		ChunkData Data;
		UInt offset;
		inline Block()=default;
		inline Block(const ChunkData& d, UInt o):Data(d),offset(o)
		{
			if(d.Length<(o+sizeof(hdr)) || ((hdr*)d.Data)->Size>(d.Length-sizeof(hdr)-o)) throw OutOfRangeException("FATAL ERROR: corrupt chunk detected");
		}
		inline Block(const ChunkData& d):Data(d),offset(0)
		{
			if(d.Length<sizeof(hdr) || ((hdr*)d.Data)->Size>(d.Length-sizeof(hdr))) throw OutOfRangeException("FATAL ERROR: corrupt chunk detected");
		}
		inline Block(const Chunk& c)
		{
			if(c.Data.Length<sizeof(hdr) || ((hdr*)c.Data.Data)->Size>(c.Data.Length-sizeof(hdr))) throw OutOfRangeException("FATAL ERROR: corrupt chunk detected");
		}

		inline hdr* RawData()
		{
			return (hdr*) (((Byte*) (Data.Data)) + offset);
		}
		inline bool Next()
		{
			hdr* tmp = RawData();
			if (((offset + tmp->Size + sizeof(hdr)) <= (Data.Length)))
			{
				offset += tmp->Size;
				if (offset + RawData()->Size > (Data.Length))
				{
					//throw OutOfRangeException("FATAL ERROR: corrupt chunk detected");
					offset -= tmp->Size;
					return false;
				}
				return true;
			}
			return false;
		}
	};
	Block Chunk::FirstBlock()
	{
		return Data;
	}
	///////////////////////////////
	struct CBlock
	{
		CBlock* Next;
		CBlock* Prev;
		CBlock() :
				Next(NULL), Prev(NULL)
		{
		}
		virtual UInt CalcSize()
		{
			return sizeof(Block::hdr);
		}
		virtual ~CBlock()
		{
		}
	};
	struct CBlock_dirindex: public CBlock
	{
		struct dirent
		{
			Block::dirent hdr;
			//string name;
			inline dirent(const Block::dirent& d) :
					hdr(d)
			{
			}
			inline UInt CalcSize()
			{
				return sizeof(Block::dirent) + hdr.namelen;
			}
		};
		Block::indexhdr hdr;
		//vector<dirent> Entries;
		map<string, dirent> Entries;
		virtual UInt CalcSize()
		{
			UInt result = CBlock::CalcSize() + sizeof(hdr);
			//decltype(Entries.begin()) it;
			for (auto it = Entries.begin(); it != Entries.end(); it++)
				result += (*it).second.CalcSize();
			return result;
		}
	};
	struct CBlock_fileindex: public CBlock
	{
		struct fileent
		{
			Block::indexent hdr;
			inline fileent(Block::indexent e) :
					hdr(e)
			{
			}
			inline UInt CalcSize()
			{
				return sizeof(Block::indexent);
			}
		};
		Block::indexhdr hdr;
		//vector<fileent> Entries;
		map<FILELEN_T, fileent> Entries;
		virtual UInt CalcSize()
		{
			UInt result = CBlock::CalcSize() + sizeof(hdr);
			//size_t i;
			//for (i = 0; i < Entries.size(); i++)
			//for (auto it = Entries.begin(); it != Entries.end(); it++)
			//	result += (*it).second.CalcSize();
			result += sizeof(Block::indexent) * Entries.size();
			return result;
		}
		inline pair<fileent&, bool> GetFileBlock(FILELEN_T offset)
		{
			auto it = Entries.equal_range(offset).first;
			if (it == Entries.end()
					|| offset >= (*it).second.hdr.offset + (*it).second.hdr.len)
				return pair<fileent&, bool> { *((fileent*) NULL), false };
			return pair<fileent&, bool> { (*it).second, true };
		}
	};
	struct CBlock_filedata: public CBlock
	{
		Buffer Data;
		virtual UInt CalcSize()
		{
			return CBlock::CalcSize() + (UInt) Data.Length;
		}
	};
	struct CBlock_xattrlist: public CBlock
	{
		map<string, string> Items;
		virtual UInt CalcSize()
		{
			UInt result = CBlock::CalcSize();
			map<string, string>::iterator it;
			for (it = Items.begin(); it != Items.end(); it++)
				result += (sizeof(Block::xattrent)) + (*it).first.length()
						+ (*it).second.length();
			return result;
		}
	};
	struct CChunk
	{
		//vector<CBlock*> Blocks;
		CBlock* FirstBlock;
		CBlock* LastBlock;
		CChunk() :
				FirstBlock(NULL), LastBlock(NULL)
		{
		}
		UInt CalcSize()
		{
			//size_t i;
			UInt result = 0;
			CBlock* b = FirstBlock;
			while (b != NULL)
			{
				result += b->CalcSize();
				b = b->Next;
			}
			return result;
		}
		~CChunk()
		{
			CBlock* b = FirstBlock;
			while (b != NULL)
			{
				CBlock* next = b->Next;
				delete b;
				b = next;
			}
		}
		void InsertBlock(CBlock* b, CBlock* before = NULL)//ownership of b is transferred to this object
		{
			if (FirstBlock == NULL)
			{
				FirstBlock = LastBlock = b;
				b->Next = b->Prev = NULL;
				return;
			}
			if (before == NULL)
			{
				LastBlock->Next = b;
				b->Prev = LastBlock;
				b->Next = NULL;
				LastBlock = b;
			}
			else
			{
				CBlock* tmp = before->Prev;
				if (tmp == NULL)
					b->Prev = NULL;
				else
				{
					b->Prev = tmp;
					tmp->Next = b;
				}
				before->Prev = b;
				b->Next = before;
				if (before == FirstBlock) FirstBlock = b;
			}
		}
		void RemoveBlock(CBlock* b)
		{
			if (b->Prev) b->Prev->Next = b->Next;
			if (b->Next) b->Next->Prev = b->Prev;
			if (b == FirstBlock) FirstBlock = b->Next;
			if (b == LastBlock) LastBlock = b->Prev;
			delete b;
		}
	};
	class StorageManager
	{
	public:
		enum ReqType
		{
			stat = 1, exists, read, write
		};
		struct ReqInfo
		{
			ReqID id;
			void* dataout;
			string name;
			UInt length;
			ReqType t;
		};
		boost::mt19937 rng;
		typedef unsigned long ReqID;DELEGATE(void,Callback,ReqID);
		vector<IStorage*> Stores;
		StorageManager();
		virtual ~StorageManager();

		CacheManager<CID, CChunk> cache;
		map<ReqID, ReqInfo> curReqInfos;
		multimap<CID, ReqID> curReqs;
		//map<CID, ChunkData> buffers;
		void ParseChunk(const ChunkData& data, CChunk& c);
		ReqID fs_stat(CID id, struct stat& st);
		ReqID fs_exists(CID id, bool& b);
		ReqID fs_lookup(CID parent, string name);
		ReqID fs_read(CID id, void* buf, UInt length);
		ReqID fs_write(CID id, void* buf, UInt length);
	protected:
		inline void requestChunk(CID id);
		void cb(const IStorage::CallbackInfo& inf);
	};

}

#endif /* STORAGEMANAGER_H_ */
