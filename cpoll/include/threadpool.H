/*
 * threadpool.H
 *
 *  Created on: May 17, 2013
 *      Author: xaxaxa
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <delegate.H>
#include <sys/types.h>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include "cpoll.H"
using namespace std;
namespace CP
{
	struct IDLETHREAD
	{
		Delegate<void()> func;
		Delegate<void(IDLETHREAD*)> cb;
		timespec lastPut { 0, 0 }; //CLOCK_MONOTONIC
		pthread_t thread;
		int32_t efd;
		enum
		{
			none = 0, invoke, kill
		} op;
		IDLETHREAD() :
				efd(eventfd(0, EFD_SEMAPHORE)), op(none) {
		}
		~IDLETHREAD() {
			close(efd);
		}
		void signal() {
			eventfd_t i = 1;
			eventfd_write(efd, i);
		}
	};
	void* idleThread(void* v) {
		while (true) {
			{
				eventfd_t i;
				eventfd_read(((IDLETHREAD*) v)->efd, &i);
			}
			switch (((IDLETHREAD*) v)->op) {
				case IDLETHREAD::invoke:
					try {
						((IDLETHREAD*) v)->func();
					} catch (...) {
					}
					((IDLETHREAD*) v)->cb((IDLETHREAD*) v);
					break;
				case IDLETHREAD::kill:
					delete (IDLETHREAD*) v;
					return NULL;
				default:
					break;
			}
		}
	}

	class ThreadPool: public RGC::Object
	{
	public:
		vector<IDLETHREAD*> threads;
		PThreadMutex mutex;
		int32_t max;
		pthread_attr_t _attr;
		ThreadPool(int32_t max = 8) :
				max(max) {
			pthread_attr_init(&_attr);
			pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_DETACHED);
		}
		~ThreadPool() {
			for (size_t i = 0; i < threads.size(); i++) {
				threads[i]->op = IDLETHREAD::kill;
				threads[i]->signal();
			}
			pthread_attr_destroy(&_attr);
		}
		IDLETHREAD* create() {
			IDLETHREAD* tmp = new IDLETHREAD();
			if (pthread_create(&tmp->thread, &_attr, idleThread, (void*) tmp) == 0) return tmp;
			else {
				throw runtime_error(strerror(errno));
			}
		}
		IDLETHREAD* get() {
			size_t threadcount;
			{
				ScopeLock l(mutex);
				threadcount = threads.size();
				if (threadcount > 0) {
					IDLETHREAD* t = threads[threads.size() - 1];
					threads.resize(threads.size() - 1);
					return t;
				}
			}
			return create();
		}
		void put(IDLETHREAD* t) {
			bool b;
			{
				ScopeLock l(mutex);
				if ((b = ((int32_t) threads.size() < max))) threads.push_back(t);
			}
			if (!b) {
				t->op = IDLETHREAD::kill;
				t->signal();
			}
		}
		void invoke(const Delegate<void()>& func) {
			IDLETHREAD* t = get();
			t->cb = {&ThreadPool::put,this};
			t->func = func;
			t->op = IDLETHREAD::invoke;
			t->signal();
		}
	};
}

#endif /* THREADPOOL_H_ */
