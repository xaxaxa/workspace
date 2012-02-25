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
#include <Wt/WDateTime>
#include <Wt/WContainerWidget>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <Wt/Dbo/WtSqlTraits>
#include <cplib/cplib.hpp>
#include <boost/shared_ptr.hpp>
#include "htmlfunc.H"
using namespace std;
namespace dbo = Wt::Dbo;
using namespace xaxaxa;
using namespace boost;
namespace chat
{
	string formatDateTime(const WDateTime& dt)
	{
		return string("<span class=\"datetime\">") + lexical_cast<string>(dt.toTime_t() * 1000)
				+ string("</span>");
	}
	class Reply: public Message
	{
	public:
		bool disown, deleted, admindeleted, postbyadmin, postbymoderator, isverified, replybyposter;
		int number;
		WDateTime createdate;

		long id;
		string nickname;
		string body;
		virtual WWidget* ToWidget() const
		{
			WContainerWidget* w = new WContainerWidget();
			StringBuilder sb;
			sb << "<div class=\"header";
			if (postbyadmin || postbymoderator)
				sb << " postbyadmin";
			sb << "\">#" << number;
			if (replybyposter)
				sb << " <b>reply by poster</b>";
			sb << "<br /><span class=\"nickname\">" << ((nickname == string()) ? "someone" :
			HTMLEscape1(nickname));
			if (isverified)
				sb << "<img src=\"http://ictp.us.to:12480/bw3/checked.gif\" />";
			sb << "</span> wrote at " << formatDateTime(createdate) << "</div>";
			w->addWidget(new WText(WString::fromUTF8(sb.ToCString(), false)));
			WText* c = new WText(WString::fromUTF8(body, false));
			c->setTextFormat(TextFormat::PlainText);
			c->setStyleClass("content");
			if(replybyposter)
				c->addStyleClass("replybyposter");
			w->addWidget(c);
			//WText* t=new WText(WString::fromUTF8("<b>" + nickname + "</b><br />" + body + "<br />", false));
			//t->setStyleClass("");

			return w;
		}
		template<class Action>
		void persist(Action& a)
		{
			dbo::field(a, id, "id");
			dbo::field(a, number, "number");
			dbo::field(a, disown, "disown");
			dbo::field(a, deleted, "deleted");
			dbo::field(a, admindeleted, "admindeleted");
			dbo::field(a, postbyadmin, "postbyadmin");
			dbo::field(a, postbymoderator, "postbymoderator");
			dbo::field(a, isverified, "isverified");
			dbo::field(a, replybyposter, "replybyposter");
			dbo::field(a, createdate, "createdate");
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
	const string conn_str =
			"hostaddr='192.168.5.62' dbname='fb_app' user='fb_app' password='passw0rd!'";
	class bw3_chatserver: public GenericServer
	{
	public:
		long postid;
		long last_id;
		bw3_chatserver(Wt::WServer& wserver, long postid) :
				GenericServer(wserver), postid(postid), last_id(0)
		{

		}
		virtual void GetMessages(Client* c)
		{
			dbo::backend::Postgres conn(conn_str);
			dbo::Session session;
			session.setConnection(conn);
			session.mapClass<Reply>("bw3_replies_view");
			typedef dbo::collection<dbo::ptr<Reply> > replylist;
			dbo::Transaction transaction(session);
			//replylist l = session.query<dbo::ptr<Reply> >("select *, (select (bw3_users.nickname=bw3_replies.nickname and bw3_replies.nickname != '') from bw3_users where bw3_users.id=bw3_replies.uid) as isverified from bw3_replies").where("bw3_replies.post_id=?").bind(postid).orderBy(
			//		"bw3_replies.createdate desc nulls last").limit(20);
			replylist l = session.find<Reply>().where("post_id=?").bind(postid).orderBy(
					"createdate desc nulls last").limit(20);
			vector<dbo::ptr<Reply> > replies;
			for (auto i = l.begin(); i != l.end(); ++i)
			{
				replies.push_back(*i);
			}
			for (auto i = replies.rbegin(); i != replies.rend(); ++i)
			{
				auto msg=boost::shared_ptr<Message>(new Reply(**i));
				Notify(c, msg, false);
				last_id = (*i)->id;
			}
			Notify(c, boost::shared_ptr<Message>(), true);
			transaction.rollback();
		}
		virtual void Update()
		{
			recursive_mutex::scoped_lock lock(mutex);
			dbo::backend::Postgres conn(conn_str);
			dbo::Session session;
			session.setConnection(conn);
			session.mapClass<Reply>("bw3_replies_view");
			typedef dbo::collection<dbo::ptr<Reply> > replylist;
			dbo::Transaction transaction(session);
			replylist l = session.find<Reply>().where("post_id=? and id>?").bind(postid).bind(
					last_id).orderBy("createdate");

			vector<dbo::ptr<Reply> > replies;
			for (auto i = l.begin(); i != l.end(); ++i)
			{
				auto msg=boost::shared_ptr<Message>(new Reply(**i));
				Send(msg,false);
				last_id = (*i)->id;
			}
			Send(boost::shared_ptr<Message>(), true);
			transaction.rollback();
		}
		virtual ~bw3_chatserver()
		{
		}
	};

} /* namespace chat */
#endif /* BW3CHATSERVER_H_ */
