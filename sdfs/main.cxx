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
using namespace sdfs;
using namespace std;
using namespace xaxaxa;

int main(int argc, char** argv)
{
	CacheItem<char[1024]> aaaaa;
	cout << sizeof(aaaaa) << endl;
	CacheManager<string, char[1024]> m;
	cout << sizeof(m) << endl;
	cout << sizeof(CacheManager<string, char[1024]>::Iter) << endl;
	m.GetItem("asdf");
	CID tmp=SDFS_PACK_ID(12345678,0xFFFFFFFFFFF);
	cout << "tmp=" << tmp << endl;
	CID cid=SDFS_UNPACK_ID_CID(tmp);
	cout << "cid=" << cid << endl;
	UInt bindex=SDFS_UNPACK_ID_BLOCKINDEX(tmp);
	cout << "bindex=" << bindex << endl;
	cout << "sizeof(StorageManager)=" << sizeof(StorageManager) << endl;
	return 0;
}
