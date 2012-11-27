#include <iostream>
#include <pthread.h>
#include <stdlib.h>

using namespace std;

pthread_mutex_t mutex;
/*#define LOCK(m) pthread_mutex_lock(&m);try
#define UNLOCK(m) finally{pthread_mutex_unlock(&m);}*/
struct scopeLock
{
	pthread_mutex_t& mutex;
	scopeLock(pthread_mutex_t& m):mutex(m)
	{pthread_mutex_lock(&mutex);}
	~scopeLock()
	{pthread_mutex_unlock(&mutex);}
};
void* th(void* v)
{
	int i(0);
	while(1)
	{
		{
			scopeLock l(mutex);
			cout << "thread " << (long int)v << ": " << i << endl;
		}
		sleep(((double)rand())/RAND_MAX*3);
		i++;
	}
	return NULL;
}

int main()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); 
	pthread_mutex_init(&mutex,&attr);
	int x;
	pthread_t t[10];
	for(x=0;x<10;x++)
	{
		pthread_create(&t[x],NULL,th,(void*)x);
	}
	//return 0;
	while(1)sleep(10);
}

