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
namespace sdfs {
template<typename t>struct CacheItem
{
	t Item;
	Byte refcount;
	Byte priority;
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
}
template<typename tkey, typename tvalue>class CacheManager {
public:
	typedef std::map<tkey,CacheItem<tvalue> > Map;
	typedef Map::iterator Iter;
	Map items;
	int MaxItems,NewItems;
	CacheManager();
	virtual ~CacheManager();
	bool GetItem(tkey k);
	void AddItem(tkey k, tvalue v);

};

}

#endif /* CACHEMANAGER_H_ */
