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
		void InsertBlock(CBlock* b, CBlock* before = NULL) //ownership of b is transferred to this object
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
	template<class ReqID = ULong, class LockObj = MutexLock> class StorageManager
	{
	public:
		enum class ReqState
:		Byte
		{
			stat = 1, exists, read, write
		};
		struct ReqInfo
		{
			ReqID id;
			void* dataout;
			string name;
			UInt length;
			ReqState s;
		};
		boost::mt19937 rng;
		LockObj l;
		typedef sdfs::Lock<LockObj> Lock;
		//typedef unsigned long ReqID;
		DELEGATE(void,Callback,ReqID);
		EVENT(Callback) CB;
		vector<IStorage*> Stores;
		StorageManager();
		virtual ~StorageManager();

		CacheManager<CID, CChunk> cache;
		typedef CacheManager<CID, CChunk>::Ptr ChunkPtr;
		//map<ReqID, ReqInfo> curReqInfos;
		multimap<CID, ReqInfo> curReqs;
		typedef multimap<CID, ReqInfo>::iterator reqIter;
		//map<CID, ChunkData> buffers;
		void ParseChunk(const ChunkData& data, CChunk& c);
		void fs_stat(const ReqID& rid, CID id, struct stat& st);
		void fs_exists(const ReqID& rid, CID id, bool& b);
		void fs_lookup(const ReqID& rid, CID parent, string name);
		void fs_read(const ReqID& rid, CID id, void* buf, UInt length);
		void fs_write(const ReqID& rid, CID id, void* buf, UInt length);
	protected:
		inline void requestChunk(CID id);
		void cb(const IStorage::CallbackInfo& inf);
		bool tryGetChunk(CID id, const ReqInfo& inf, ChunkPtr& c_out);
		//returns true if retrieved synchronously; false if the operation is pending
	};
	template<class ReqID, class LockObj> StorageManager<ReqID, LockObj>::StorageManager()
	{
		size_t i;
		for (i = 0; i < Stores.size(); i++)
		{
			Stores[i]->CB = IStorage::Callback(&sdfs::StorageManager<ReqID>::cb,
					this);
		}
	}
	template<class ReqID, class LockObj> StorageManager<ReqID, LockObj>::~StorageManager()
	{
		// TODO Auto-generated destructor stub
	}

	template<class ReqID, class LockObj> void StorageManager<ReqID, LockObj>::ParseChunk(
			const ChunkData & data, CChunk & c)
	{
		Chunk ch = data;
		Block b = ch;
		//void* chunkend = ((Byte*) data.Data) + data.Length;

		const Byte warnlevel = 3;
		const char* errmsg = "CORRUPT CHUNK DETECTED. TROUBLE AHEAD.";
		do
		{
			CBlock* bl;
			Block::hdr* h = b.RawData();

			void* blockdata = h->GetData();
			void* blockend = ((Byte*) blockdata) + h->Size;
			/*if (blockend > chunkend)
			 {
			 WARN(warnlevel, errmsg);
			 return;
			 }*/
			switch (h->Type)
			{
				case Block::BlockType::dirindex:
				{
					CBlock_dirindex* tmp;
					bl = tmp = new CBlock_dirindex();
					Block::indexhdr* ihdr = (Block::indexhdr*) blockdata;
					if ((void*) (ihdr + 1) > blockend)
					{
						WARN(warnlevel, errmsg);
						continue;
					}
					tmp->hdr = *ihdr;
					Block::dirent* de = (Block::dirent*) (ihdr + 1);
					while ((void*) (de + 1) <= (blockend))
					{
						char* name = (char*) (void*) (de + 1);
						if ((void*) (name + de->namelen) > blockend)
						{
							WARN(warnlevel, errmsg);
							continue;
						}
						string s(name, de->namelen);
						tmp->Entries.insert(
								std::pair<string, CBlock_dirindex::dirent> { s,
										CBlock_dirindex::dirent(*de) });
						de = (Block::dirent*) (name + de->namelen);
					}
					break;
				}
				case Block::BlockType::fileindex:
				{
					CBlock_fileindex* tmp;
					bl = tmp = new CBlock_fileindex();
					Block::indexhdr* ihdr = (Block::indexhdr*) blockdata;
					if ((void*) (ihdr + 1) > blockend)
					{
						WARN(warnlevel, errmsg);
						continue;
					}
					tmp->hdr = *ihdr;
					Block::indexent* e = (Block::indexent*) (ihdr + 1);
					while ((void*) (e + 1) <= (blockend))
					{
						tmp->Entries.insert(
								std::pair<FILELEN_T, CBlock_fileindex::fileent> {
										e->offset, *e });
						e++;
					}
					break;
				}
				case Block::BlockType::filedata:
				{
					CBlock_filedata* tmp;
					bl = tmp = new CBlock_filedata();
					tmp->Data = Buffer(h->Size);
					memcpy(tmp->Data.Data, blockdata, h->Size);
					break;
				}
				case Block::BlockType::xattr:
				{
					CBlock_xattrlist* tmp;
					bl = tmp = new CBlock_xattrlist();
					Block::xattrent* xe = (Block::xattrent*) blockdata;
					while ((void*) (xe + 1) <= (blockend))
					{
						char* name = (char*) (void*) (xe + 1);
						if ((void*) (name + xe->namelen) > blockend)
						{
							WARN(warnlevel, errmsg);
							continue;
						}
						char* value = (char*) (void*) (name + xe->namelen);
						if ((void*) (value + xe->valuelen) > blockend)
						{
							WARN(warnlevel, errmsg);
							continue;
						}
						string n(name, xe->namelen);
						string v(value, xe->valuelen);
						tmp->Items.insert(std::pair<string, string> { n, v });
						xe = (Block::xattrent*) (value + xe->valuelen);
					}
					break;
				}
				default:
					continue;
			}
			c.InsertBlock(bl);
		}
		while (b.Next());
	}

	template<class ReqID, class LockObj> inline void StorageManager<ReqID, LockObj>::requestChunk(
			CID id)
	{
		size_t i;
		vector<IStorage*> tmp;
		for (i = 0; i < Stores.size(); i++)
		{
			if (Stores[i]->Initialized
					&& (Stores[i]->Chunks.find(id) != Stores[i]->Chunks.end()))
			{
				tmp.push_back(Stores[i]);
			}
		}
		IStorage* stor;
		size_t s = tmp.size();
		if (s <= 0) return;
		if (s == 1)
		{
			stor = tmp[0];
			goto aaaaa;
		}
		{
			boost::uniform_int<> r(0, tmp.size());
			boost::variate_generator<decltype(rng), boost::uniform_int<> > dice(rng,
					r);
			stor = tmp[dice()];
		}
		aaaaa: stor->BeginGetChunk(id);
	}

	template<class ReqID, class LockObj> void StorageManager<ReqID, LockObj>::cb(
			const IStorage::CallbackInfo & inf)
	{

	}
	template<class ReqID, class LockObj> inline bool StorageManager<ReqID, LockObj>::tryGetChunk(
			CID id, const ReqInfo & inf, ChunkPtr& c_out)
	{
		Lock lck(l);
		ChunkPtr ptr = cache.GetItem(id);
		if (ptr.Item->Initialized())
		{
			c_out = ptr;
			return true;
		}
		auto it = this->curReqs.find(id);
		bool requested = it != curReqs.end();
		curReqs.insert( { id, inf });
		if (!requested) requestChunk(id);

	}
}

#endif /* STORAGEMANAGER_H_ */
