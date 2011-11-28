/*
 * main.cxx
 *
 *  Created on: Nov 18, 2011
 *      Author: xaxaxa
 */

#include "CacheManager.h"
#include <string>
#include <cplib/cplib.hpp>
#include "StorageManager.h"
#include <cplib/asyncfile.hpp>
using namespace sdfs;
using namespace std;
using namespace xaxaxa;
using namespace IO;

void test1();
int main(int argc, char** argv)
{
	CacheItem<char[1024]> aaaaa;
	cout << sizeof(aaaaa) << endl;
	CacheManager<string, char[1024]> m;
	cout << sizeof(m) << endl;
	cout << sizeof(CacheManager<string, char[1024]>::Iter) << endl;
	m.GetItem("asdf");
	CID tmp = SDFS_PACK_ID(12345678,0xFFFFFFFFFFF);
	cout << "tmp=" << tmp << endl;
	CID cid = SDFS_UNPACK_ID_CID(tmp);
	cout << "cid=" << cid << endl;
	UInt bindex = SDFS_UNPACK_ID_BLOCKINDEX(tmp);
	cout << "bindex=" << bindex << endl;
	cout << "sizeof(StorageManager)=" << sizeof(StorageManager<>) << endl;
	test1();
	return 0;
}
void test1()
{
	StringBuilder sb;
	Buffer b(4096);
	File f("chunk1", 0);
	while (int br = f.Read(b))
	{
		sb.Append(b.SubBuffer(0, br));
	}
	ChunkData cd { sb.buf, (UInt)sb.Length };
	StorageManager<> m;
	CChunk c;
	m.ParseChunk(cd, c);
	throw Exception("breakpoint");
}

