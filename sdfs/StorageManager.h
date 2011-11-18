/*
 * StorageManager.h
 *
 *  Created on: 2011-11-15
 *      Author: user1
 */

#ifndef STORAGEMANAGER_H_
#define STORAGEMANAGER_H_
#include <string>
#include "IStorage.h"
using namespace std;

namespace sdfs {

class StorageManager {
public:

	StorageManager();
	virtual ~StorageManager();
	CID AllocChunk();
	void AllocChunk(CID id);

};

}

#endif /* STORAGEMANAGER_H_ */
