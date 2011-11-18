/*
 * IStorage.h
 *
 *  Created on: 2011-11-15
 *      Author: user1
 */

#ifndef ISTORAGE_H_
#define ISTORAGE_H_
#include <string>
using namespace std;

namespace sdfs {
typedef string CID;

class Chunk
{
	CID id;
	int size;
};
class IStorage {
public:
	IStorage();
	virtual ~IStorage();
	virtual Chunk GetChunk(CID id)=0;

};

}

#endif /* ISTORAGE_H_ */
