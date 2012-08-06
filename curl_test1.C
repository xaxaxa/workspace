/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/poll.h>
#include <curl/curl.h>
#include <event.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <functional>

using std::function;
using namespace std;
event_base* eb;
CURLM *m;
event timer_event;
struct taskInfo
{
	int socket;
	int act; //from libcurl
	CURL* c;
	event ev; //libevent struct is embedded within
	bool isAdded; //whether this is registered with libevent
};
struct curlTaskInfo
{
	CURL *c;
	function<void(CURL*, CURLcode)> cb;
	//void* userdata;
};
void cb_event(int fd, short events, void *userp);
void setTaskInfo(taskInfo* t, int s, CURL* c, int act)
{ //has the side effect of registering or re-registering the event with libevent
	int events =
		(act&CURL_POLL_IN?EV_READ:0)|(act&CURL_POLL_OUT?EV_WRITE:0)|EV_PERSIST;
	if(t->isAdded)
		event_del(&t->ev);
    t->socket=s;
    t->act=act;
    t->c=c;
    t->isAdded=true;
    event_assign(&t->ev,eb,s,events,cb_event,t);
    event_add(&t->ev, NULL);
}
void checkQueue()
{
	CURLMsg *msg;
	int msgs_left;
	while ((msg = curl_multi_info_read(m, &msgs_left))) {
		if (msg->msg == CURLMSG_DONE) {
			CURL *c=msg->easy_handle;
			curlTaskInfo* t=NULL;
			curl_easy_getinfo(c, CURLINFO_PRIVATE, &t);
			//cout << "t = " << t << endl;
			t->cb(c,msg->data.result);
			curl_multi_remove_handle(m, c);
			curl_easy_cleanup(c);
			delete t;
		}
	}
}
void cb_event(int fd, short events, void *userp)
{ // an event has been received from libevent
	int act =
		(events & EV_READ ? CURL_CSELECT_IN : 0) |
		(events & EV_WRITE ? CURL_CSELECT_OUT : 0);
	int num_transfers;
	curl_multi_socket_action(m, fd, act, &num_transfers);
	checkQueue();
}

/* CURLMOPT_SOCKETFUNCTION */ 
int cb_sock(CURL *c, curl_socket_t s, int what, void *cbp, void *sockp)
{
	//cout << "cb_sock()" << endl;
	taskInfo* t=(taskInfo*)sockp;
	if(what==CURL_POLL_REMOVE && t!=NULL) {
		if(t->isAdded)
			event_del(&t->ev);
		delete t;
	} else {
		if(t==NULL) { //add
			t=new taskInfo();
			t->isAdded=false;
			setTaskInfo(t,s,c,what);
			curl_multi_assign(m,s,t);
		} else { //modify events monitored
			setTaskInfo(t,s,c,what);
		}
	}
	return 0;
}
void addCurlTask(CURL* c, const function<void(CURL*,CURLcode)>& cb)
{
	curlTaskInfo* t=new curlTaskInfo();
	t->c=c;
	t->cb=cb;
	//t->userdata=userdata;
	curl_easy_setopt(c, CURLOPT_PRIVATE, t);
	curl_multi_add_handle(m, c);
}
struct transferInfo
{
	function<bool(const void* data, int len, int state)> cb;
	
};
size_t cb_data(void *data, size_t size, size_t nmemb, void *userdata)
{
	transferInfo* t=(transferInfo*)userdata;
	t->cb(data,size*nmemb,3);
	return size;
}
void addTransfer(const char* url,
	const function<bool(const void* data, int len, int state)>& cb)
										/*-1:failed 1:connected 2:sent 3:recving 4:closed*/
{
	CURL* c=curl_easy_init();
	curl_easy_setopt(c, CURLOPT_URL, url);
	transferInfo* t=new transferInfo();
	t->cb=cb;
	curl_easy_setopt(c, CURLOPT_WRITEDATA, t);
	curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, cb_data);
	addCurlTask(c,[t](CURL* c,CURLcode res)
	{
		t->cb(NULL,0,4);
		delete t;
	});
}
int cb_curl_timer(CURLM *m, long timeout_ms, void* userdata)
{ /* Update the event timer after curl_multi library calls */ 
	struct timeval timeout;
 
	timeout.tv_sec = timeout_ms/1000;
	timeout.tv_usec = (timeout_ms%1000)*1000;
	//printf("multi_timer_cb: Setting timeout to %ld ms\n", timeout_ms);
	evtimer_add(&timer_event, &timeout);
	return 0;
}
void cb_timer(int fd, short kind, void *userp)
{
	int num_transfers;
	curl_multi_socket_action(m,
		CURL_SOCKET_TIMEOUT, 0, &num_transfers);
	checkQueue();
}
int main(int argc, char **argv)
{
	eb=event_base_new();
	m=curl_multi_init();
	
	event_assign(&timer_event,eb,-1,0,cb_timer,NULL);
	curl_multi_setopt(m, CURLMOPT_SOCKETFUNCTION, cb_sock);
	curl_multi_setopt(m, CURLMOPT_TIMERFUNCTION, cb_curl_timer);
	
	addTransfer("https://graph.facebook.com/4",[](const void* data, int len, int state)
	{
		//cout << len << endl;
		if(data!=NULL && len>0)
			write(1,data,len);
		return true;
	});
	//sleep(5);
	event_base_dispatch(eb);
	curl_multi_cleanup(m);
	return 0;
}
