/*
 * Server.H
 *
 *  Created on: Feb 16, 2012
 *      Author: xaxaxa
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <Wt/WApplication>
#include <Wt/WWidget>
#include <functional>
#include <set>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/function.hpp>
#include <cplib/cplib.hpp>
#include <Wt/WText>
using namespace Wt;
using namespace std;
using namespace boost;
using namespace xaxaxa;

namespace chat
{
	class Message
	{
	public:
		virtual ~Message()
		{
		}
		virtual WWidget* ToWidget() const=0;
	};
	class TextMessage: public Message
	{
	public:
		string s;
		TextMessage(string s) :
				s(s)
		{
		}
		virtual ~TextMessage()
		{
		}
		virtual WWidget* ToWidget() const
		{
			return new WText(s+"<br />");
		}
	};
	struct Client
	{
		typedef boost::function<void(const Message&,bool)> Callback;
		Callback cb;
		WApplication* app;
	};
	class Server
	{
	public:
		Server()
		{
		}
		virtual void Attach(Client* cb, bool getmsgs = true)=0;
		virtual void Detach(Client* cb)=0;
		virtual ~Server()
		{
		}
	};

	class GenericServer: public Server
	{
	public:
		virtual ~GenericServer()
		{
		}
		//CircularQueue<Message*> log;
		Wt::WServer& wserver;
		int msgs;
		recursive_mutex mutex;
		set<Client*> cbs;
		GenericServer(Wt::WServer& wserver) :
				wserver(wserver)
		{
		}
		virtual void GetMessages(Client* c)
		{
			//to be implemented by subclass
		}
		virtual void Attach(Client* cb, bool getmsgs = true)
		{
			recursive_mutex::scoped_lock lock(mutex);
			if(cbs.insert(cb).second && getmsgs)
				GetMessages(cb);
		}
		virtual void Detach(Client* cb)
		{
			recursive_mutex::scoped_lock lock(mutex);
			cbs.erase(cb);
		}
		virtual void Notify(Client* cb, const Message& msg, bool flush=true)
		{
			recursive_mutex::scoped_lock lock(mutex);
			WApplication *app = WApplication::instance();
			if (app && app->sessionId() == cb->app->sessionId())
				cb->cb(msg,flush);
			else
			{
				//wserver.post(cb->sessionID, boost::bind(cb->cb, boost::ref(msg)));
				WApplication::UpdateLock lock2(cb->app);
				cb->cb(msg,flush);
				if(flush)cb->app->triggerUpdate();
			}
		}
		virtual void Send(const Message& evt, bool flush=true)
		{
			recursive_mutex::scoped_lock lock(mutex);

			for (auto it = cbs.begin(); it != cbs.end(); it++)
				Notify(*it, evt, flush);
		}

	};

} /* namespace bw3 */
#endif /* SERVER_H_ */
