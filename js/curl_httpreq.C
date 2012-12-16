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

#include "curl_httpreq.H"
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/poll.h>
using namespace std;

namespace curl
{
	struct taskInfo
	{
		instance* inst;
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
	void setTaskInfo(instance* inst, taskInfo* t, int s, CURL* c, int act)
	{ //has the side effect of registering or re-registering the event with libevent
		int events =
			(act&CURL_POLL_IN?EV_READ:0)|(act&CURL_POLL_OUT?EV_WRITE:0)|EV_PERSIST;
		if(t->isAdded)
			event_del(&t->ev);
		t->socket=s;
		t->act=act;
		t->c=c;
		t->inst=inst;
		t->isAdded=true;
		event_assign(&t->ev,inst->eb,s,events,cb_event,t);
		event_add(&t->ev, NULL);
	}
	void checkQueue(instance* inst)
	{
		CURLMsg *msg;
		int msgs_left;
		while ((msg = curl_multi_info_read(inst->m, &msgs_left))) {
			if (msg->msg == CURLMSG_DONE) {
				CURL *c=msg->easy_handle;
				curlTaskInfo* t=NULL;
				curl_easy_getinfo(c, CURLINFO_PRIVATE, &t);
				//cout << "t = " << t << endl;
				t->cb(c,msg->data.result);
				curl_multi_remove_handle(inst->m, c);
				curl_easy_cleanup(c);
				delete t;
			}
		}
	}
	void cb_event(int fd, short events, void *userdata)
	{ // an event has been received from libevent
		taskInfo* t=(taskInfo*)userdata;
		instance* inst=t->inst;
		int act =
			(events & EV_READ ? CURL_CSELECT_IN : 0) |
			(events & EV_WRITE ? CURL_CSELECT_OUT : 0);
		int num_transfers;
		curl_multi_socket_action(inst->m, fd, act, &num_transfers);
		checkQueue(inst);
	}

	/* CURLMOPT_SOCKETFUNCTION */ 
	int cb_sock(CURL *c, curl_socket_t s, int what, void *cbp, void *sockp)
	{
		//cout << "cb_sock()" << endl;
		instance* inst=(instance*)cbp;
		taskInfo* t=(taskInfo*)sockp;
		if(what==CURL_POLL_REMOVE && t!=NULL) {
			if(t->isAdded)
				event_del(&t->ev);
			delete t;
		} else {
			if(t==NULL) { //add
				t=new taskInfo();
				t->isAdded=false;
				setTaskInfo(inst,t,s,c,what);
				curl_multi_assign(inst->m,s,t);
			} else { //modify events monitored
				setTaskInfo(inst,t,s,c,what);
			}
		}
		return 0;
	}
	void addCurlTask(instance* inst, CURL* c, const function<void(CURL*,CURLcode)>& cb)
	{
		curlTaskInfo* t=new curlTaskInfo();
		t->c=c;
		t->cb=cb;
		//t->userdata=userdata;
		curl_easy_setopt(c, CURLOPT_PRIVATE, t);
		curl_multi_add_handle(inst->m, c);
	}
	struct transferInfo
	{
		function<bool(const void* data, int len, int state)> cb;
		CURL* c;
	};
	size_t cb_data(void *data, size_t size, size_t nmemb, void *userdata)
	{
		transferInfo* t=(transferInfo*)userdata;
		if(!t->cb(data,size*nmemb,3))return 0;
		return size*nmemb;
	}
	transferInfo* addTransfer(instance* inst, const char* url,
		const function<bool(const void* data, int len, int state)>& cb)
											/*-1:failed 1:connected 2:sent 3:recving 4:closed*/
	{
		CURL* c=curl_easy_init();
		curl_easy_setopt(c, CURLOPT_URL, url);
		transferInfo* t=new transferInfo();
		t->cb=cb;
		t->c=c;
		curl_easy_setopt(c, CURLOPT_WRITEDATA, t);
		curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, cb_data);
		return t;
	}
	void beginTransfer(instance* inst, transferInfo* t) {
		addCurlTask(inst,t->c,[t](CURL* c,CURLcode res)
		{
			if(res==CURLE_OK)
				t->cb(NULL,0,4);
			else if(res!=CURLE_WRITE_ERROR)
				t->cb(NULL,0,-res);
			delete t;
		});
	}
	int cb_curl_timer(CURLM *m, long timeout_ms, void* userdata)
	{ /* Update the event timer after curl_multi library calls */ 
		instance* inst=(instance*)userdata;
		struct timeval timeout;
		timeout.tv_sec = timeout_ms/1000;
		timeout.tv_usec = (timeout_ms%1000)*1000;
		//printf("multi_timer_cb: Setting timeout to %ld ms\n", timeout_ms);
		evtimer_add(&inst->timer_event, &timeout);
		return 0;
	}
	void cb_timer(int fd, short kind, void *userdata)
	{
		instance* inst=(instance*)userdata;
		int num_transfers;
		curl_multi_socket_action(inst->m,
			CURL_SOCKET_TIMEOUT, 0, &num_transfers);
		checkQueue(inst);
	}
	void newInstance(instance* inst)
	{
		inst->eb=event_base_new();
		inst->m=curl_multi_init();
		event_assign(&inst->timer_event,inst->eb,-1,0,cb_timer,inst);
		curl_multi_setopt(inst->m, CURLMOPT_SOCKETFUNCTION, cb_sock);
		curl_multi_setopt(inst->m, CURLMOPT_SOCKETDATA, inst);
		curl_multi_setopt(inst->m, CURLMOPT_TIMERFUNCTION, cb_curl_timer);
		curl_multi_setopt(inst->m, CURLMOPT_TIMERDATA, inst);
	}
	void eventLoop(instance* inst)
	{
		event_base_dispatch(inst->eb);
	}
	void dispose(instance* inst)
	{
		curl_multi_cleanup(inst->m);
	}
	
	void transferManager::addTransfer(const char* url, bool post,
			const function<bool(const void* data, int len, int state)>& cb)
	{
		if(itemsProcessing<concurrency) {
			doTransfer(url, post, cb);
		} else {
			q.push({url, post, cb});
		}
	}
	void transferManager::checkQueue()
	{
		if(itemsProcessing<concurrency && q.size()>0) {
			item& it=q.front();
			doTransfer(it.url.c_str(), it.post,it.cb);
			q.pop();
		}
	}
	void transferManager::doTransfer(const char* url, bool post,
			const function<bool(const void* data, int len, int state)>& cb)
	{
		itemsProcessing++;
		transferInfo* t=curl::addTransfer(&inst,url,[cb,this](const void* data, int len, int state)
		{
			if(state==4) {
				itemsProcessing--;
				checkQueue();
			}
			return cb(data, len, state);
		});
		if(post)
			curl_easy_setopt(t->c, CURLOPT_POST, 1);
		curl::beginTransfer(&inst, t);
	}
}
/*
int main(int argc, char **argv)
{
	curl::instance inst;
	curl::newInstance(&inst);
	curl::addTransfer(&inst,"http://192.168.5.11/",[](const void* data, int len, int state)
	{
		cout << len << endl;
		//if(data!=NULL && len>0)
		//	write(1,data,len);
		return true;
	});
	curl::eventLoop(&inst);
	
	return 0;
} //*/
