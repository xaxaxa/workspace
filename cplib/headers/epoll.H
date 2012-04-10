#ifndef EPOLL_H
#define EPOLL_H
#include <map>
#include "cplib.hpp"
#include <functional>
using namespace std;
using namespace xaxaxa;
namespace xaxaxa
{
	class EPoll
	{
	public:
		EPoll();
		virtual ~EPoll();
		typedef function<void(FILEDES, uint32_t)> Callback;
		struct taskID
		{
			FILEDES fd;
			uint32_t evt;
			bool operator<(const taskID& other)
			{
				return *((ULong*)this) < *((ULong*)&other);
			}
			bool operator>(const taskID& other)
			{
				return *((ULong*)this) > *((ULong*)&other);
			}
			bool operator==(const taskID& other)
			{
				return *((ULong*)this) == *((ULong*)&other);
			}
		};
		struct taskInfo
		{
			Callback cb;
		};
		map<taskID, taskInfo> tasks;
		set<taskID> removed;
		void EventLoop();
		void AddTask(FILEDES fd, uint32_t event, Callback cb);
		void CancelTask(FILEDES fd, uint32_t event);
	};
};
#endif // EPOLL_H
