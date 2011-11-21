/*
 * IStorage.h
 *
 *  Created on: 2011-11-15
 *      Author: user1
 */

#ifndef ISTORAGE_H_
#define ISTORAGE_H_
#include <string>
#include <cplib/cplib.hpp>

using namespace std;
using namespace xaxaxa;
namespace sdfs {
typedef ULong CID;


struct ChunkData
{
	void* data;
	UInt size;
	ChunkData():data(NULL)
	{};
	~ChunkData()
	{
		if(data!=NULL)free(data);
	}
};


DELEGATE(void,StorageCallback,int);
class IStorage {
public:

	IStorage();
	virtual ~IStorage();
	//virtual Chunk GetChunk(CID id)=0;
	virtual void BeginGetChunk(CID id, ChunkData& dataout)=0;
};

}

#endif /* ISTORAGE_H_ */
