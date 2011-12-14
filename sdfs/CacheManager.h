/*
 * CacheManager.h
 *
 *  Created on: 2011-11-16
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

#ifndef CACHEMANAGER_H_
#define CACHEMANAGER_H_

#include <map>
#include <set>
#include <cplib/cplib.hpp>
using namespace xaxaxa;
namespace sdfs
{
	template<typename tkey, typename tvalue> class CacheManager;
	struct CacheFlags
	{
		static const Byte Initialized = 1;
		static const Byte Dirty = 2;
	};
	template<typename t> struct CacheItem
	{
		t Item;
		Byte refcount;
		Byte flags;
		inline bool Initialized()
		{
			return flags & CacheFlags::Initialized;
		}
		inline bool Dirty()
		{
			return flags & CacheFlags::Dirty;
		}
		//Byte priority;
		//bool initialized;
		//bool dirty;
		inline bool CanPurge()
		{
			return (refcount <= 0) && !Dirty();
		}
	};
	template<typename k, typename v> struct CacheItemPtr
	{
		CacheItem<v>* Item;
		bool destruct;
		CacheManager<k, v>* m;
		typename CacheManager<k, v>::Iter it;
		//typename CacheManager<k, v>::ListIter it1;
		inline v& Get()
		{
			return Item->Item;
		}
		inline CacheItemPtr() :
				Item(NULL)
		{
		}
		/*inline CacheItemPtr(CacheItemPtr<k, v>& x)
		 {
		 Item = x.Item;
		 if (x.destruct) x.Item = NULL;
		 destruct = x.destruct;
		 m = x.m;
		 it = x.it;
		 }*/
		inline CacheItemPtr(const CacheItemPtr<k, v>& x)
		{
			Item = x.Item;
			Item->refcount++;
			destruct = true;
			m = x.m;
			it = x.it;
		}
		inline CacheItemPtr(CacheManager<k, v>* m , typename CacheManager<k, v>::Iter it)
		{
			Item = &(*it).second;
			Item->refcount++;
			destruct = true;
			this->m = m;
			this->it = it;
		}
		/*inline CacheItemPtr<k, v> Clone()
		 {
		 Item->refcount++;
		 return CacheItemPtr<k, v> { Item, true, m, it };
		 }
		 inline CacheItemPtr<k, v> CloneWeak()
		 {
		 return CacheItemPtr<k, v> { Item, false, m, it };
		 }*/
		inline void Reset();
		inline ~CacheItemPtr()
		{
			Reset();
		}
		/*inline CacheItemPtr<k, v>& operator=(CacheItemPtr<k, v>& other)
		 {
		 if (this == &other) return *this; // protect against invalid self-assignment
		 Item = other.Item;
		 //other.destruct = false;
		 if (other.destruct) other.Item = NULL;
		 destruct = other.destruct;
		 m = other.m;
		 it = other.it;
		 // by convention, always return *this
		 return *this;
		 }*/
		inline CacheItemPtr<k, v>& operator=(const CacheItemPtr<k, v>& other)
		{
			if (this == &other) return *this; // protect against invalid self-assignment
			Reset();
			Item = other.Item;
			//other.destruct = false;
			//other.Item = NULL;
			m = other.m;
			it = other.it;
			destruct = true;
			// by convention, always return *this
			return *this;
		}
		inline void SetDirty(bool dirty);
		inline bool IsNull()
		{
			return Item == NULL;
		}
	};
	enum class NotifyType
:	Byte
	{
		nop = 0, dirty, canpurge
	};
	template<typename tkey, typename tvalue> class CacheManager
	{
	public:
		typedef std::map<tkey, CacheItem<tvalue> > Map;
		typedef typename Map::iterator Iter;
		typedef typename std::list<tkey>::iterator ListIter;
		Map items;
		typedef CacheItemPtr<tkey, tvalue> Ptr;
		typename std::list<Iter> items_l;
		typename std::set<tkey> items_dirty;
		//typename std::list<tkey> items_canpurge;
		size_t MaxItems;
		CacheManager();
		virtual ~CacheManager();
		Ptr GetItem(const tkey& k);
		bool GetItem(const tkey& k, Ptr& p);
		//void AddItem(const tkey& k, const tvalue& v);
		void Purge(int n); //purge n items from the cache
		void NotifyItem(Iter it, NotifyType t); //update item info
	}
	;

	template<typename k, typename v> void CacheItemPtr<k, v>::Reset()
	{
		if (destruct && Item)
		{
			Item->refcount--;
			if ((Item->refcount <= 0) && m) m->NotifyItem(it, NotifyType::canpurge);
			Item = NULL;
		}
	}

	template<typename tkey, typename tvalue>
	CacheItemPtr<tkey, tvalue> CacheManager<tkey, tvalue>::GetItem(const tkey & k)
	{
		Iter it = items.find(k);
		if (it == items.end())
		{
			CacheItem<tvalue> tmp;
			tmp.refcount = 1;
			tmp.flags = 0;
			//tmp.initialized = false;
			//tmp.dirty = false;
			//tmp.asdfasdf = true;
			it = items.insert(std::pair<tkey, CacheItem<tvalue> >(k, tmp)).first;
			items_l.push_back(it);
			if (items_l.size() > MaxItems) Purge(MaxItems - items_l.size());
			(*it).second.refcount = 0;
			return CacheItemPtr<tkey, tvalue>(this, it);
		}
		else
		{
			return CacheItemPtr<tkey, tvalue>(this, it);
		}
	}
	template<typename tkey, typename tvalue>
	bool CacheManager<tkey, tvalue>::GetItem(const tkey & k, Ptr& p)
	{
		Iter it = items.find(k);
		if (it == items.end())
			return false;
		else
		{
			p = CacheItemPtr<tkey, tvalue>(this, it);
			return true;
		}
	}

	template<typename tkey, typename tvalue>
	void CacheManager<tkey, tvalue>::Purge(int n)
	{
		typename std::list<Iter>::iterator it;
		it = items_l.begin();
		for (int i; i < n && it != items_l.end(); it++)
		{
			//purge the item
			Iter& x(*it);
			if ((*x).second.CanPurge())
			{
				items.erase(x);
				items_dirty.erase((*x).first);
				items_l.erase(it);
				i++;
			}
		}
	}
	template<typename tkey, typename tvalue>
	CacheManager<tkey, tvalue>::CacheManager()
	{
	}

	template<typename tkey, typename tvalue>
	CacheManager<tkey, tvalue>::~CacheManager()
	{
	}

	template<typename tkey, typename tvalue>
	inline void CacheManager<tkey, tvalue>::NotifyItem(Iter it, NotifyType t)
	{
		switch (t)
		{
			case NotifyType::dirty:
				if ((*it).second.flags & CacheFlags::Dirty)
					items_dirty.insert((*it).first);
				else
					items_dirty.erase((*it).first);
				break;
			case NotifyType::canpurge:
				/*if ((!((*it).second.flags & CacheFlags::flags_dirty))
				 && (*it).second.refcount <= 0)
				 items_canpurge.insert((*it).first);
				 else
				 items_canpurge.erase((*it).first);*/
				break;
			default:
				break;
		}
	}
}

template<typename k, typename v>
inline void sdfs::CacheItemPtr<k, v>::SetDirty(bool dirty)
{
	if (((Item->flags & CacheFlags::Dirty) != 0) ^ dirty)
	{
		if (dirty)
			Item->flags |= CacheFlags::Dirty;
		else
			Item->flags &= ~CacheFlags::Dirty;
		if (m) m->NotifyItem(it, NotifyType::dirty);
	}
}

#endif /* CACHEMANAGER_H_ */
