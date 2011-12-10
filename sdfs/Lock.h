/*
 * Lock.h
 *
 *  Created on: Dec 3, 2011
 *      Author: xaxaxa
 */

#ifndef LOCK_H_
#define LOCK_H_
#include <pthread.h>
namespace sdfs
{
	class MutexLock
	{
	public:
		pthread_mutex_t mutex;
		inline MutexLock()
		{
			pthread_mutexattr_t attr;
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&mutex, &attr);
		}
		inline ~MutexLock()
		{
			pthread_mutex_destroy(&mutex);
		}
		inline void Lock()
		{
			pthread_mutex_lock(&mutex);
		}
		inline void Unlock()
		{
			pthread_mutex_unlock(&mutex);
		}
	};
	template<typename l = MutexLock> struct Lock
	{
		l _lock;
		inline Lock(l lck) :
				_lock(lck)
		{
			lck.Lock();
		}
		inline ~Lock()
		{
			_lock.Unlock();
		}
	};
} /* namespace sdfs */
#endif /* LOCK_H_ */
