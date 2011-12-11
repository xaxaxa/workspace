/*
 * IStorage.h
 *
 *  Created on: 2011-11-15
 *      Author: user1
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

#ifndef ISTORAGE_H_
#define ISTORAGE_H_
#include <string>
#include <set>
#include <map>
#include <cplib/cplib.hpp>

using namespace std;
using namespace xaxaxa;
namespace sdfs
{
	typedef ULong CID;

	struct ChunkData
	{
		void* Data;
		UInt Length;
		/*
		 ~ChunkData()
		 {
		 if (data != NULL) free(data);
		 }*/
	};
	//typedef Buffer ChunkData;
	typedef int ReqID;
	class IStorage;

	class IStorage
	{
	public:
		//typedef ULong State;
		enum class CallbackType
:		Byte
		{
			nop=0,
			init,
			chunk
		};
		struct CallbackInfo
		{
			IStorage& source;
			bool success;
			CID cid;
			CallbackType type;
			Buffer b;
			//State state;
			//ChunkData& data;
		};
		DELEGATE(void,Callback,const CallbackInfo&);
		Callback CB;
		set<CID> Chunks;
		bool Initialized;
		int Priority;
		IStorage();
		virtual ~IStorage();
		//virtual Chunk GetChunk(CID id)=0;
		virtual void BeginGetChunk(CID id)=0;
		virtual void BeginPutChunk(CID id, Buffer b)=0;
		virtual void BeginRemoveChunk(CID id)=0;
		virtual void Init(const map<string,string>& config)=0;
	};

}

#endif /* ISTORAGE_H_ */
