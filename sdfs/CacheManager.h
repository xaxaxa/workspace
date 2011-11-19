/*
 * CacheManager.h
 *
 *  Created on: 2011-11-16
 *      Author: user1
 */

#ifndef CACHEMANAGER_H_
#define CACHEMANAGER_H_
#include <map>
#include <cplib/cplib.hpp>
using namespace xaxaxa;
namespace sdfs {
template<typename t>struct CacheItem
{
	t Item;
	Byte refcount;
	//Byte priority;
	bool initialized;
	bool dirty;
};
template<typename t>struct CacheItemPtr
{
	CacheItem<t>* Item;
	bool destruct;
	inline CacheItem<t>* get()
	{
		return Item;
	}
	inline CacheItemPtr(const CacheItemPtr<t>& x)
	{
		Item=x.Item;
		Item->refcount++;
		destruct=true;
	}
	inline void reset()
	{
		if(destruct && Item)Item->refcount--;
	}
	inline ~CacheItemPtr()
	{
		reset();
	}
	inline CacheItemPtr<t>& operator= (const CacheItemPtr<t>& other)
	{
	   if (this == &other) return *this; // protect against invalid self-assignment
	   other.Item=Item;
	   other.destruct=false;
	   // by convention, always return *this
	   return *this;
	}
};

template<typename tkey, typename tvalue>class CacheManager {
public:
	typedef std::map<tkey,CacheItem<tvalue> > Map;
	typedef Map::iterator Iter;
	Map items;
	std::list<Iter> items_l;
	int MaxItems,NewItems;
	CacheManager();
	virtual ~CacheManager();
	CacheItemPtr<tvalue> GetItem(const tkey& k);
	//void AddItem(const tkey& k, const tvalue& v);
	void Purge(int i);//purge i items from the cache

};

}

#endif /* CACHEMANAGER_H_ */
