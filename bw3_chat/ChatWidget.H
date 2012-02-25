/*
 * ChatWidget.h
 *
 *  Created on: Feb 16, 2012
 *      Author: xaxaxa
 */

#ifndef CHATWIDGET_H_
#define CHATWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WBoxLayout>
#include <Wt/WTextArea>
#include "Server.H"
#include "htmlfunc.H"
#include <string>
#include <boost/shared_ptr.hpp>
using namespace xaxaxa;
using namespace Wt;
using namespace std;
using namespace boost;
namespace chat
{

	class ChatWidget: public WContainerWidget
	{
	public:
		WContainerWidget messages;
		Server& s;
		Client c;
		//WTextArea txt;
		void cb(const boost::shared_ptr<Message> msg, bool flush)
		{
			if (msg)messages.addWidget(msg->ToWidget());
			if (flush)
			{
				/*auto v=messages.children();
				auto it=v.rbegin();
				if(it==v.rend())return;*/
				parseDates(*(c.app));
				string js = "setTimeout(function(){"+messages.jsRef() + ".scrollTop=" + messages.jsRef() + ".scrollHeight;},100);";
				c.app->doJavaScript(js);
				c.app->triggerUpdate();
				/*string js2 = "document.body.onload=function(){" + messages.jsRef()
						+ ".scrollTop=" + messages.jsRef() + ".scrollHeight;};";
				messages.doJavaScript(js2);*/
			}
		}
		ChatWidget(Server& s) :
				s(s)
		{
			//c = new Client();
			c.cb = boost::bind(mem_fn(&ChatWidget::cb), this, _1, _2);
			c.app = WApplication::instance();
			c.sessionId=c.app->sessionId();
			s.Attach(&c);
			setPadding(0);
			messages.setStyleClass("chatbox");
			//txt.setStyleClass("chattxt");
			WBoxLayout *l = new WBoxLayout(WBoxLayout::Direction::TopToBottom);
			l->setContentsMargins(0, 0, 0, 0);
			l->addWidget(&messages, 1);
			//l->addWidget(&txt,0);
			setLayout(l);
		}
		virtual ~ChatWidget()
		{
			//delete c;
			s.Detach(&c);
		}
	};

} /* namespace bw3 */
#endif /* CHATWIDGET_H_ */
