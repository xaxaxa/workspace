/*
 * main.cxx
 *
 *  Created on: Nov 18, 2011
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
void cb(void* a, const ULong& id, int retv)
{
	cout << "cb("<<id<<", "<<retv<<");"<<endl;
}
struct stat st;
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
	//test1();
	StorageManager<> asdf;
	asdf.CB+=StorageManager<>::Callback(&cb,NULL);
	asdf.fs_stat(123,5,st);
	return 0;
}
void test1()
{
	StringBuilder sb;
	Buffer b(4096);
	File f("/home/xaxaxa/chunk1", 0);
	while (int br = f.Read(b))
	{
		sb.Append(b.SubBuffer(0, br));
	}
	ChunkData cd { sb.buf, (UInt)sb.length };
	StorageManager<> m;
	CChunk c;
	m.ParseChunk(cd, c);
	//throw Exception("breakpoint");
}

