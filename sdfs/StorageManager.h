/*
 * StorageManager.h
 *
 *  Created on: 2011-11-15
 *      Author: xaxaxa
 *
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include "Lock.h"
#include <boost/random.hpp>
#define FUSE_USE_VERSION 27
#include <fuse/fuse_lowlevel.h>
using namespace std;
using namespace xaxaxa;
#define SDFS_PACK_ID(chunkid,blockindex) (((CID)(chunkid)&0x0000FFFFFFFFFFFF)|((CID)(blockindex)<<(64-16)))
#define SDFS_UNPACK_ID_CID(id) ((id)&0x0000FFFFFFFFFFFF)
#define SDFS_UNPACK_ID_BLOCKINDEX(id) ((UInt)(((id)&0xFFFF000000000000)>>(64-16)))
#ifdef _64BITFILELENGTH
typedef ULong FILELEN_T;
#else
typedef UInt FILELEN_T;
#endif
namespace sdfs
{
	struct Block;
	typedef UShort BlockID;
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
		//	0    16   24     56         	(length)
		//	________________________________
		//	| id |type|length|    data      |
		//	|____|____|______|______________|
		//length includes the size of the length field(32 bits)
		//***IMPORTANT***: if id is zero, the length field is omitted and it is assumed that
		//the block fills the chunk. this is for space optimization reasons.
		//However, when the chunk is parsed into memory, it's assigned an ID of 1.
		//StorageManager::SerializeChunk() should check if a chunk contains only one
		//block and if it does, apply this optimization.
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

		//block header
		struct blockhdr
		{
			BlockID ID;
			BlockType Type;
			UInt Size;
			static const int minsize=sizeof(BlockID)+sizeof(BlockType); //constexpr
			inline void* GetData()
			{
				return (ID==0)?(void*)(((Byte*)this)+sizeof(ID)+sizeof(Type)):(void*)(this+1);
			}
		};
		struct indexhdr
		{
			CID parent;
			//UInt entryoffset; //offset of the parent dirent from the block
			//entry offset removed: the parent will be searched for the inode #.
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
			inodehdr inode;
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
		inline blockhdr* RawData()
		{
			return (blockhdr*) (((Byte*) (Data.Data)) + offset);
		}
		inline UInt GetSize()
		{
			blockhdr* tmp=RawData();
			return tmp->ID==0?(Data.Length-blockhdr::minsize):tmp->Size;
		}
		inline bool CheckBounds()
		{
			if(Data.Length>=(offset+sizeof(BlockID)+sizeof(BlockType)) && RawData()->ID==0)return true;
			if(Data.Length<(offset+sizeof(blockhdr)) || RawData()->Size>(Data.Length-offset-sizeof(blockhdr)))return false;
			return true;
		}
		inline Block()=default;
		inline Block(const ChunkData& d, UInt o):Data(d),offset(o)
		{
			if(!CheckBounds())throw OutOfRangeException("FATAL ERROR: corrupt chunk detected");
		}
		inline Block(const ChunkData& d):Data(d),offset(0)
		{
			if(!CheckBounds())throw OutOfRangeException("FATAL ERROR: corrupt chunk detected");
		}
		/*inline Block(const Chunk& c):Block(c.Data)
		 {
		 if(!CheckBounds())throw OutOfRangeException("FATAL ERROR: corrupt chunk detected");
		 }*/

		inline bool Next()
		{
			blockhdr* tmp = RawData();
			if(tmp->ID==0)return false;
			if (((offset + tmp->Size + sizeof(blockhdr)) <= (Data.Length)))
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
		//CBlock* Next;
		//CBlock* Prev;
		BlockID ID;
		Block::BlockType Type;
		CBlock()
		{
		}
		virtual UInt CalcSize()
		{
			return ID == 0 ? Block::blockhdr::minsize : sizeof(Block::blockhdr);
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
		map<CID, dirent> Entries;
		map<string, CID> Index;
		inline dirent* GetEntry(CID child)
		{
			auto it = Entries.find(child);
			if (it == Entries.end()) return NULL;
			return &(*it).second;
		}
		inline dirent* GetEntry(string name)
		{
			auto it = Index.find(name);
			if (it == Index.end()) return NULL;
			return GetEntry((*it).second);
		}
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
			const pair<fileent&, bool> nullret = { *((fileent*) NULL), false };
			auto it = Entries.lower_bound(offset);
			if ((it == Entries.end()) || ((*it).second.hdr.offset > offset))
			{
				if (it == Entries.begin()) return nullret;
				it--;
			}
			if (it == Entries.end() || offset >= (*it).second.hdr.offset + (*it).second.hdr.len)
				return nullret;
			return pair<fileent&, bool> { (*it).second, true };
			//return_null: return pair<fileent&, bool> { *((fileent*) NULL), false };
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
				result += (sizeof(Block::xattrent)) + (*it).first.length() + (*it).second.length();
			return result;
		}
	};
	struct CChunk
	{
		//vector<CBlock*> Blocks;
		/*CBlock* FirstBlock;
		 CBlock* LastBlock;*/
		map<BlockID, CBlock*> Blocks;
		bool contig_id; //whether the block ID's are verified to be contiguous.
		CChunk()
		{
		}
		UInt CalcSize()
		{
			//size_t i;
			UInt result = 0;
			/*CBlock* b = FirstBlock;
			 while (b != NULL)
			 {
			 result += b->CalcSize();
			 b = b->Next;
			 }*/
			for (auto it = Blocks.begin(); it != Blocks.end(); it++)
			{
				result += (*it).second->CalcSize();
			}
			return result;
		}
		~CChunk()
		{
			/*CBlock* b = FirstBlock;
			 while (b != NULL)
			 {
			 CBlock* next = b->Next;
			 delete b;
			 b = next;
			 }*/
			for (auto it = Blocks.begin(); it != Blocks.end(); it++)
			{
				delete (*it).second;
			}
		}
		/*void InsertBlock(CBlock* b, CBlock* before = NULL) //ownership of b is transferred to this object
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
		 void Clear()
		 {
		 CBlock* b = FirstBlock;
		 while (b != NULL)
		 {
		 CBlock* next = b->Next;
		 delete b;
		 b = next;
		 }
		 FirstBlock = LastBlock = NULL;
		 }*/
		BlockID GetNextID()
		{
			auto it = Blocks.begin();
			if (it == Blocks.end())
			{
				contig_id = true;
				return 1;
			}
			if (contig_id)
			{
				auto it = Blocks.end();
				it--;
				return (*it).first + 1;
			}
			BlockID prev_id = (*it).first;
			it++;
			if (it == Blocks.end())
			{
				contig_id = true;
				return prev_id + 1;
			}
			BlockID id;
			do
			{
				id = (*it).first;
				if (id > (prev_id + 1))
				{
					return prev_id + 1;
				}

				prev_id = id;
				it++;
			}
			while (it != Blocks.end());
			contig_id = true;
			id++;
			if (id == 0) id = 1; //in case of integer wrap-around
			return id;
		}
		inline BlockID InsertBlock(CBlock* b) //ownership of b is transferred to this object
		{
			BlockID id = b->ID == 0 ? GetNextID() : b->ID;
			Blocks.insert( { id, b });
			return id;
		}
		inline CBlock* GetBlock(BlockID id)
		{
			auto it = Blocks.find(id);
			if (it == Blocks.end()) return NULL;
			return (*it).second;
		}
		inline void RemoveBlock(BlockID id)
		{
			auto it = Blocks.find(id);
			if (it == Blocks.end()) return;
			delete (*it).second;
			Blocks.erase(it);
		}
		void Clear()
		{
			for (auto it = Blocks.begin(); it != Blocks.end(); it++)
			{
				delete (*it).second;
			}
			Blocks.clear();
		}
	};
	template<typename ReqID = ULong, typename LockObj = MutexLock> class StorageManager
	{
	public:
		/*enum class ReqState
		 :		Byte
		 {
		 stat = 1, exists, read, write
		 };*/
		CacheManager<CID, CChunk> cache;
		typedef CacheManager<CID, CChunk>::Ptr ChunkPtr;
		struct ReqInfo;DELEGATE(void, ReqState, ReqInfo&, CID, ChunkPtr);
		//typedef void(*ReqState)(ReqInfo& inf);
		struct ReqInfo
		{
			ReqID id;
			ReqState s;
			union
			{
				struct
				{
					struct stat* st;
					CID cid;
					BlockID dir_bid;
				} stat_info;
				/*struct
				 {
				 struct stat* st;
				 UInt parentoffset;
				 } stat_info2;*/
				struct
				{
					void* dataout;
					UInt length;
				} rw_info;
				struct
				{
					void* name;
					Byte namelen;
				} lookup_info;
			};
			//string name;
			bool success;
		};
		boost::mt19937 rng;
		LockObj l;
		typedef sdfs::Lock<LockObj> Lock;
		//typedef unsigned long ReqID;
		DELEGATE(void,Callback,ReqID,int);EVENT(Callback) CB;
		void Reply(ReqID rid, int retv);
		//retv: return value. negative if error occured.
		//positive or zero if successful.
		//for read() and write(), returns bytes read/written.
		//otherwise, returns zero.

		vector<IStorage*> Stores;
		StorageManager();
		virtual ~StorageManager();

		//map<ReqID, ReqInfo> curReqInfos;
		multimap<CID, ReqInfo> curReqs;
		typedef typename multimap<CID, ReqInfo>::iterator reqIter;
		//map<CID, ChunkData> buffers;
		void ParseChunk(const ChunkData& data, CChunk& c);

	protected:
		inline void requestChunk(CID id);
		void cb(const IStorage::CallbackInfo& inf);
		inline void beginGetChunk(CID id, const ReqInfo& inf);
		//begins the chunk request and invokes the correct function based on the ReqState
		//when the operation completes. cache is queried first.

		//callbacks.
		void stat_cb2(ReqInfo& inf, CID id, ChunkPtr cptr)
		{
			if (!inf.success)
			{
				Reply(inf.id, -ENOENT);
				return;
			}
			CBlock* bl = cptr.Get().GetBlock(inf.stat_info.dir_bid);
			if (bl->Type != Block::BlockType::dirindex)
			{
				Reply(inf.id, -ENOENT);
				return;
			}
			CBlock_dirindex* dir = (CBlock_dirindex*) bl;
			Block::inodehdr* inode = dir->GetEntry(inf.stat_info.cid)->hdr.inode;
			struct stat* st = (struct stat*) inf.stat_info.st;
			memset(st, 0, sizeof(st));
			st->st_atim = inode->s_atime;
			st->st_ctim = inode->s_ctime;
			st->st_mtim = inode->s_mtime;
			st->st_gid = inode->s_gid;
			st->st_uid = inode->s_uid;
			st->st_mode = inode->s_mode;
			st->st_size = inode->s_size;
			Reply(inf.id, 0);
		}
		void stat_cb1(ReqInfo& inf, CID id, ChunkPtr cptr)
		{
			if (!inf.success)
			{
				Reply(inf.id, -ENOENT);
				return;
			}
			//struct stat* st = (struct stat*) inf.dataout;
			CBlock* bl = cptr.Get().GetBlock(SDFS_UNPACK_ID_BLOCKINDEX(inf.stat_info.cid));
			Block::indexhdr* ihdr;
			switch (bl->Type)
			{
				case Block::BlockType::dirindex:
					ihdr = &((CBlock_dirindex*) bl)->hdr;
					break;
				case Block::BlockType::fileindex:
					ihdr = &((CBlock_fileindex*) bl)->hdr;
					break;
				default:
					Reply(inf.id, -ENOENT);
					return;
			}
			inf.s = ReqState(&stat_cb2, this);
			inf.stat_info.dir_bid = SDFS_UNPACK_ID_BLOCKINDEX(ihdr->parent);
			beginGetChunk(SDFS_UNPACK_ID_CID(ihdr->parent), inf);
			//inf.stat_info2.parentoffset = ihdr->entryoffset;
		}
	public:
		//filesystem methods.
		void fs_stat(const ReqID& rid, CID id, struct stat& st)
		{
			ReqInfo inf;
			inf.stat_info.st = &st;
			inf.s = ReqState(&stat_cb1, this);
			inf.stat_info.cid = id;
			beginGetChunk(SDFS_UNPACK_ID_CID(id), inf);
		}
		void fs_exists(const ReqID& rid, CID id, bool& b);
		void fs_lookup(const ReqID& rid, CID parent, string name);
		void fs_read(const ReqID& rid, CID id, void* buf, UInt length);
		void fs_write(const ReqID& rid, CID id, void* buf, UInt length);
	};
	template<class ReqID, class LockObj> StorageManager<ReqID, LockObj>::StorageManager()
	{
		size_t i;
		for (i = 0; i < Stores.size(); i++)
		{
			Stores[i]->CB = IStorage::Callback(&sdfs::StorageManager<ReqID>::cb, this);
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
		Block b = ch.FirstBlock();
		//void* chunkend = ((Byte*) data.Data) + data.Length;

		const Byte warnlevel = 3;
		const char* errmsg = "CORRUPT CHUNK DETECTED. TROUBLE AHEAD.";
		do
		{
			CBlock* bl;
			Block::blockhdr* h = b.RawData();

			void* blockdata = h->GetData();
			void* blockend = ((Byte*) blockdata) + b.GetSize();
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
						tmp->Entries.insert(pair<CID, CBlock_dirindex::dirent> { de->indexblock,
								CBlock_dirindex::dirent(*de) });
						tmp->Index.insert(pair<string, CID> { s, de->indexblock });
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
								std::pair<FILELEN_T, CBlock_fileindex::fileent> { e->offset, *e });
						e++;
					}
					break;
				}
				case Block::BlockType::filedata:
				{
					CBlock_filedata* tmp;
					bl = tmp = new CBlock_filedata();
					tmp->Data = Buffer(b.GetSize());
					memcpy(tmp->Data.Data, blockdata, b.GetSize());
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
			bl->ID = h->ID;
			bl->Type = h->Type;
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
			if (Stores[i]->Initialized && (Stores[i]->Chunks.find(id) != Stores[i]->Chunks.end()))
			{
				tmp.push_back(Stores[i]);
			}
		}
		IStorage* stor;
		size_t s = tmp.size();
		if (s <= 0)
		{
			auto range = curReqs.equal_range(id);

			for (decltype(range.first) it(range.first); it != range.second; it++)
			{
				ReqInfo& inf1((*it).second);
				inf1.success = false;
				CALL(inf1.s, inf1, id, ChunkPtr());
			}
			curReqs.erase(id);
		}
		if (s == 1)
		{
			stor = tmp[0];
			goto aaaaa;
		}
		{
			boost::uniform_int<> r(0, tmp.size());
			boost::variate_generator<decltype(rng), boost::uniform_int<> > dice(rng, r);
			stor = tmp[dice()];
		}
		aaaaa: stor->BeginGetChunk(id);
	}

	template<class ReqID, class LockObj> void StorageManager<ReqID, LockObj>::cb(
			const IStorage::CallbackInfo & inf)
	{
		Lock lck(l);
		//put into cache

		auto range = curReqs.equal_range(inf.cid);

		if (!inf.success)
		{
			for (decltype(range.first) it(range.first); it != range.second; it++)
			{
				ReqInfo& inf1((*it).second);
				inf1.success = false;
				CALL(inf1.s, inf1, inf.cid, ChunkPtr());
			}
			curReqs.erase(inf.cid);
		}
		ChunkPtr ptr = cache.GetItem(inf.cid);
		ptr.Item->flags |= CacheFlags::Initialized;
		ptr.Get().Clear();
		ParseChunk( { inf.b.Data, inf.b.Length }, ptr.Get());
		for (decltype(range.first) it(range.first); it != range.second; it++)
		{
			ReqInfo& inf1((*it).second);
			inf1.success = true;
			CALL(inf1.s, inf1, inf.cid, ptr);
		}
		curReqs.erase(inf.cid);
	}
	template<class ReqID, class LockObj> inline void StorageManager<ReqID, LockObj>::beginGetChunk(
			CID id, const ReqInfo & inf)
	{
		Lock lck(l);
		ChunkPtr ptr;
		if (cache.GetItem(id, ptr))
		{
			CALL(inf.s, inf, id, ptr);
			return;
		}
		auto it = this->curReqs.find(id);
		bool requested = it != curReqs.end();
		curReqs.insert( { id, inf });
		if (!requested) requestChunk(id);
	}
}

#endif /* STORAGEMANAGER_H_ */
