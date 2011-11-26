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
		// TODO Auto-generated constructor stub

	}
	StorageManager::~StorageManager()
	{
		// TODO Auto-generated destructor stub
	}

	void StorageManager::ParseChunk(const ChunkData & data, CChunk & c)
	{
		Chunk ch = data;
		Block b = ch;
		void* chunkend = ((Byte*) data.Data) + data.Length;
		const Byte warnlevel = 3;
		const char* errmsg = "CORRUPT CHUNK DETECTED. TROUBLE AHEAD.";
		do
		{
			CBlock* bl;
			Block::hdr* h = b.RawData();
			void* blockdata = h->GetData();
			switch (h->Type)
			{
				case Block::BlockType::dirindex:
				{
					CBlock_dirindex* tmp;
					bl = tmp = new CBlock_dirindex();
					Block::indexhdr* ihdr = (Block::indexhdr*) blockdata;
					if ((void*) (ihdr + 1) > chunkend)
					{
						WARN(warnlevel, errmsg);
						continue;
					}
					tmp->hdr = *ihdr;
					Block::dirent* de = (Block::dirent*) (ihdr + 1);
					while ((void*) (de + 1) <= (chunkend))
					{
						char* name = (char*) (void*) (de + 1);
						if ((void*) ((Byte*) (de + 1) + de->namelen) > chunkend)
						{
							WARN(warnlevel, errmsg);
							continue;
						}
						string s(name, de->namelen);
						tmp->Entries.insert( { s, *de });
						de++;
					}
					break;
				}
				case Block::BlockType::fileindex:
				{
					CBlock_fileindex* tmp;
					bl = tmp = new CBlock_fileindex();
					Block::indexhdr* ihdr = (Block::indexhdr*) blockdata;
					if ((void*) (ihdr + 1) > chunkend)
					{
						WARN(warnlevel, errmsg);
						continue;
					}
					tmp->hdr = *ihdr;
					Block::indexent* e = (Block::indexent*) (ihdr + 1);
					while ((void*) (e + 1) <= (chunkend))
					{
						tmp->Entries.insert({e->offset,*e});
						e++;
					}
					break;
				}
			}
			c.Blocks.push_back(bl);
		}
		while (b.Next());
	}

}
