/*
 * SharedObjManager.H
 *
 *  Created on: Feb 17, 2012
 *      Author: xaxaxa
 */

#ifndef SHAREDOBJMANAGER_H_
#define SHAREDOBJMANAGER_H_
#include <map>
#include <iostream>
#include <boost/thread/recursive_mutex.hpp>
using namespace boost;
using namespace std;
namespace chat
{
	static int objs = 0;
	class counterObj
	{
	public:
		counterObj()
		{
			objs++;
			cout << objs << " counterObjects total" << endl;
		}
		~counterObj()
		{
			objs--;
			cout << objs << " counterObjects total" << endl;
		}
	};
	template<class Tkey, class Tvalue> class SharedObjPtr;
	template<class T> class SharedObjInfo
	{
	public:
		T* Object;
		int refcount;
		//recursive_mutex mutex;
		SharedObjInfo(T* obj) :
				Object(obj), refcount(0)
		{

		}
		void Delete()
		{
			delete Object;
		}
		inline void inc()
		{
			++refcount;
		}
		inline void dec()
		{
			--refcount;
		}
	};
	template<class Tkey, class Tvalue> class SharedObjManager;
	template<class Tkey, class Tvalue> class SharedObjPtr
	{
	public:
		typedef typename map<Tkey, SharedObjInfo<Tvalue> >::iterator iterator;
		iterator it;
		SharedObjManager<Tkey, Tvalue>* m;
		SharedObjPtr(SharedObjManager<Tkey, Tvalue>& m);
		inline SharedObjPtr(SharedObjManager<Tkey, Tvalue>& m, iterator it);
		inline void Release();
		inline bool Valid();

		//returned pointer should not be used after deletion of this SharedObjPtr
		//or after Release() is called
		inline Tvalue* Get()
		{
			if (Valid())
				return (*it).second.Object;
			return NULL;
		}
		SharedObjPtr(const SharedObjPtr<Tkey, Tvalue>& other);
		inline SharedObjPtr<Tkey, Tvalue>& operator=(const SharedObjPtr<Tkey, Tvalue>& other)
		{
			Release();
			it = other.it;
			m = other.m;
			if (Valid())
				inc();
			return *this;
		}
		inline void inc();
		inline void dec();
		~SharedObjPtr()
		{
			Release();
		}
	};
	template<class Tkey, class Tvalue> class SharedObjManager
	{
	public:
		map<Tkey, SharedObjInfo<Tvalue> > m;
		recursive_mutex mutex;
		typedef typename map<Tkey, SharedObjInfo<Tvalue> >::iterator iterator;
		inline void TryDelete(iterator it)
		{
			//recursive_mutex::scoped_lock lock(mutex);
			//{
			//recursive_mutex::scoped_lock lock1((*it).second.mutex);
			if ((*it).second.refcount > 0)
				return;
			(*it).second.Delete();
			//}
			m.erase(it);
		}
		SharedObjPtr<Tkey, Tvalue> Get(const Tkey& k)
		{
			recursive_mutex::scoped_lock lock(mutex);
			iterator it = m.find(k);
			return SharedObjPtr<Tkey, Tvalue>(*this, it);
		}
		SharedObjPtr<Tkey, Tvalue> Add(const Tkey& k, Tvalue* v)
		{
			recursive_mutex::scoped_lock lock(mutex);
			auto tmp = m.insert(std::pair<Tkey, SharedObjInfo<Tvalue> >
			{ k, SharedObjInfo<Tvalue>
			{ v } });
			return SharedObjPtr<Tkey, Tvalue>(*this, tmp.first);
		}
		SharedObjManager()
		{

		}
		virtual ~SharedObjManager()
		{
		}
	};
	template<class Tkey, class Tvalue> SharedObjPtr<Tkey, Tvalue>::SharedObjPtr(
			SharedObjManager<Tkey, Tvalue>& m) :
			it(m.m.end())
	{

	}
	template<class Tkey, class Tvalue> SharedObjPtr<Tkey, Tvalue>::SharedObjPtr(
			SharedObjManager<Tkey, Tvalue>& m, iterator it) :
			it(it), m(&m)
	{
		recursive_mutex::scoped_lock lock(m.mutex);
		if (Valid())
			inc();
	}
	template<class Tkey, class Tvalue> SharedObjPtr<Tkey, Tvalue>::SharedObjPtr(
			const SharedObjPtr<Tkey, Tvalue>& other):it(other.it),m(other.m)
	{
		recursive_mutex::scoped_lock lock(m->mutex);
		if (Valid())
			inc();
	}
	template<class Tkey, class Tvalue> bool SharedObjPtr<Tkey, Tvalue>::Valid()
	{
		return it != m->m.end();
	}
	template<class Tkey, class Tvalue> void SharedObjPtr<Tkey, Tvalue>::inc()
	{
		//recursive_mutex::scoped_lock lock(m->mutex);
		(*it).second.inc();
	}
	template<class Tkey, class Tvalue> void SharedObjPtr<Tkey, Tvalue>::dec()
	{
		//recursive_mutex::scoped_lock lock(m->mutex);
		(*it).second.dec();
		m->TryDelete(it);
	}
	template<class Tkey, class Tvalue> void SharedObjPtr<Tkey, Tvalue>::Release()
	{
		recursive_mutex::scoped_lock lock(m->mutex);
		if (Valid())
			dec();
		it = m->m.end();
	}

} /* namespace chat */
#endif /* SHAREDOBJMANAGER_H_ */
