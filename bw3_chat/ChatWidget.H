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
using namespace Wt;
namespace chat
{

	class ChatWidget: public WContainerWidget
	{
	public:
		WContainerWidget messages;
		Server& s;
		Client* c;
		//WTextArea txt;
		void cb(const Message& msg)
		{
			messages.addWidget(msg.ToWidget());
		}
		ChatWidget(Server& s):s(s)
		{
			c=new Client();
			c->cb=boost::bind(mem_fn(&ChatWidget::cb),this,_1);
			c->app=WApplication::instance();
			s.Attach(c);
			setPadding(0);
			messages.setStyleClass("chatbox");
			//txt.setStyleClass("chattxt");
			WBoxLayout *l = new WBoxLayout(WBoxLayout::Direction::TopToBottom);
			l->setContentsMargins(0,0,0,0);
			l->addWidget(&messages,1);
			//l->addWidget(&txt,0);
			setLayout(l);
		}
		virtual ~ChatWidget(){}
	};

} /* namespace bw3 */
#endif /* CHATWIDGET_H_ */
