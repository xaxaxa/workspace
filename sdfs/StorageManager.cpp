/*
 * StorageManager.cpp
 *
 *  Created on: 2011-11-15
 *      Author: user1
 */

#include "StorageManager.h"
using namespace std;
using namespace xaxaxa;

namespace sdfs
{
	StorageManager::StorageManager()
	{
		size_t i;
		for (i = 0; i < Stores.size(); i++)
		{
			Stores[i]->CB = IStorage::Callback(&sdfs::StorageManager::cb, this);
		}
	}
	StorageManager::~StorageManager()
	{
		// TODO Auto-generated destructor stub
	}

	void StorageManager::ParseChunk(const ChunkData & data, CChunk & c)
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
						tmp->Entries.insert( { s, *de });
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
						tmp->Entries.insert( { e->offset, *e });
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
						tmp->Items.insert( { n, v });
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

	inline void StorageManager::requestChunk(CID id)
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

	void StorageManager::cb(const IStorage::CallbackInfo & inf)
	{

	}

}
