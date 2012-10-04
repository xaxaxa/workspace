/*
 * cpoll_internal.H
 *
 *  Created on: 2012-09-14
 *      Author: xaxaxa
 */

#ifndef CPOLL_INTERNAL_H_
#define CPOLL_INTERNAL_H_
#include <signal.h>
namespace CP
{
	static inline int eventToIndex(Events event) {
		if (event == Events::in) return 0;
		if (event == Events::out) return 1;
		if (event == Events::other) return 2;
		return -1;
	}
	static inline short eventToPoll(Events event) {
		switch (event) {
			case Events::in:
				return POLLIN | POLLPRI | POLLRDHUP;
			case Events::out:
				return POLLOUT | POLLHUP | POLLERR | POLLNVAL;
			default:
				break;
		}
		return 0;
	}
	static inline short eventsToPoll(Events events) {
		short ret = 0;
		if ((event_t) events & (event_t) Events::in) ret |= POLLIN | POLLPRI | POLLRDHUP;
		if ((event_t) events & (event_t) Events::out) ret |= POLLOUT | POLLHUP | POLLERR | POLLNVAL;
		return ret;
	}
	static inline Events pollToEvents(short events) {
		event_t ret = 0;
		if (events & (POLLIN | POLLPRI | POLLRDHUP)) ret |= (event_t) Events::in;
		if (events & (POLLOUT | POLLHUP | POLLERR | POLLNVAL)) ret |= (event_t) Events::out;
		return (Events) ret;
	}
	static inline uint32_t eventToEPoll(Events event) {
		switch (event) {
			case Events::in:
				return EPOLLIN | EPOLLPRI | EPOLLRDHUP;
			case Events::out:
				return EPOLLOUT | EPOLLHUP | EPOLLERR;
			default:
				break;
		}
		return 0;
	}
	static inline uint32_t eventsToEPoll(Events events) {
		uint32_t ret = 0;
		if ((event_t) events & (event_t) Events::in) ret |= EPOLLIN | EPOLLPRI | EPOLLRDHUP;
		if ((event_t) events & (event_t) Events::out) ret |= EPOLLOUT | EPOLLHUP | EPOLLERR;
		return ret;
	}
	static inline Events ePollToEvents(uint32_t events) {
		event_t ret = 0;
		//if EPOLLHUP is in events, return both Events::in and Event::out
		if (events & (EPOLLHUP | EPOLLIN | EPOLLPRI | EPOLLRDHUP)) ret |= (event_t) Events::in;
		if (events & (EPOLLHUP | EPOLLOUT | EPOLLERR)) ret |= (event_t) Events::out;
		return (Events) ret;
	}

	static inline Events indexToEvent(int index) {
		return (Events) (1 << index);
	}
	static inline void onError(int err_no) {
		throw CPollException(err_no);
	}
	static inline int checkError(int err) {
		if (err < 0 && errno != EINTR && errno != EINPROGRESS) throw CPollException();
		return err;
	}
	//disable the retarded SIGHUP and SIGPIPE
	static void disableSignals(){
		struct sigaction sa;
			sa.sa_handler = SIG_IGN;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = SA_RESTART; /* Restart system calls if
			 interrupted by handler */
			sigaction(SIGHUP, &sa, NULL);
			sigaction(SIGPIPE, &sa, NULL);
			sa.sa_handler = SIG_DFL;
			//cout<<sigaction(SIGSTOP, &sa, NULL)<<endl;
			//cout<<errno<<endl;
			sigaction(SIGCONT, &sa, NULL);
			sigaction(SIGTSTP, &sa, NULL);
			sigaction(SIGTTIN, &sa, NULL);
			sigaction(SIGTTOU, &sa, NULL);
	}
}

#endif /* CPOLL_INTERNAL_H_ */
