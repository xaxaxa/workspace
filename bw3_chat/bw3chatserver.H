/*
 * bw3chatserver.h
 *
 *  Created on: Feb 16, 2012
 *      Author: xaxaxa
 */

#ifndef BW3CHATSERVER_H_
#define BW3CHATSERVER_H_

#include "Server.H"
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/Postgres>
#include <vector>
using namespace std;
namespace dbo = Wt::Dbo;
namespace chat
{
	class Reply: public Message
	{
	public:
		string nickname;
		string body;
		virtual WWidget* ToWidget() const
		{
			return new WText("<b>" + nickname + "</b><br />" + body + "<br />");
		}
		template<class Action>
		void persist(Action& a)
		{
			dbo::field(a, nickname, "nickname");
			dbo::field(a, body, "body");
		}
	};
}
namespace Wt
{
	namespace Dbo
	{
		template<>
		struct dbo_traits<chat::Reply> : public dbo_default_traits
		{
			static const char *versionField()
			{
				return 0;
			}
		};
	}
}
namespace chat
{
	class bw3_chatserver: public GenericServer
	{
	public:
		bw3_chatserver(Wt::WServer& wserver) :
				GenericServer(wserver)
		{

		}
		virtual void GetMessages(Client* c)
		{
			dbo::backend::Postgres conn(
					"hostaddr='192.168.5.62' dbname='fb_app' user='fb_app' password='passw0rd!'");
			dbo::Session session;
			session.setConnection(conn);
			session.mapClass<Reply>("bw3_replies");
			typedef dbo::collection<dbo::ptr<Reply> > replylist;
			dbo::Transaction transaction(session);
			replylist l = session.find<Reply>().orderBy("createdate desc nulls last").limit(20);

			vector<dbo::ptr<Reply> > replies;
			for (auto i = l.begin(); i != l.end(); ++i)
			{
				replies.push_back(*i);
			}
			for (auto i = replies.rbegin(); i != replies.rend(); ++i)
				Notify(c, **i);
			transaction.rollback();
		}
		virtual ~bw3_chatserver()
		{
		}
	};

} /* namespace chat */
#endif /* BW3CHATSERVER_H_ */
