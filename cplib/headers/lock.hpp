/*
 * Lock.h
 *
 *  Created on: Dec 3, 2011
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

#ifndef LOCK_H_
#define LOCK_H_
#include <pthread.h>
namespace xaxaxa
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
