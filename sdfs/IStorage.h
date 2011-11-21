/*
 * IStorage.h
 *
 *  Created on: 2011-11-15
 *      Author: user1
 */

#ifndef ISTORAGE_H_
#define ISTORAGE_H_
#include <string>
#include <set>
#include <map>
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
typedef int ReqID;
class IStorage;


class IStorage {
public:
	enum class CallbackType:Byte
	{
		nop=0,
		init,
		getchunk
	};
	struct CallbackInfo
	{
		IStorage& source;
		Exception& ex;
		CID cid;
		CallbackType type;
		//ChunkData& data;
	};
	DELEGATE(void,Callback,const CallbackInfo&);
	Callback Callback;
	set<CID> Chunks;
	IStorage();
	virtual ~IStorage();
	//virtual Chunk GetChunk(CID id)=0;
	virtual void BeginGetChunk(CID id, ChunkData& dataout)=0;
	virtual void Init(const map<string,string>& config)=0;
};

}

#endif /* ISTORAGE_H_ */
