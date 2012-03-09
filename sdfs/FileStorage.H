/*
 * FileStorage.H
 *
 *  Created on: Mar 6, 2012
 *      Author: xaxaxa
 */

#ifndef FILESTORAGE_H_
#define FILESTORAGE_H_

#include "IStorage.h"

namespace sdfs
{

	class FileStorage: public IStorage
	{
	public:

		FileStorage()
		{

		}
		virtual ~FileStorage()
		{
		}
		virtual void BeginGetChunk(CID id)
		{
		}
		virtual void BeginPutChunk(CID id, Buffer b)
		{
		}
		virtual void BeginRemoveChunk(CID id)
		{
		}
		virtual void Init(const map<string, string>& config)
		{

		}
	};

} /* namespace sdfs */
#endif /* FILESTORAGE_H_ */
