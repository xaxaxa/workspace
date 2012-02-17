/*
 * App.h
 *
 *  Created on: Feb 16, 2012
 *      Author: xaxaxa
 */



#ifndef APP_H_
#define APP_H_
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include <Wt/WPushButton>
#include <Wt/WServer>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WLineEdit>
#include "ChatWidget.H"
using namespace Wt;

namespace chat
{

	class App:public WApplication
	{
	public:
		WLineEdit txt;
		ChatWidget c;
		GenericServer& s;
		JSlot clearInput;
		void send()
		{
			s.Send(TextMessage(txt.text().toUTF8()));
		}
		App(const WEnvironment& env, GenericServer& s)
		:WApplication(env),c(s),s(s)
		{
			enableUpdates();
			setTitle("Chat");
			useStyleSheet("main.css");
			txt.setStyleClass("chattxt");
			WVBoxLayout* l=new WVBoxLayout();
			l->setSpacing(2);
			l->setContentsMargins(0,0,0,0);
			l->addWidget(&c,1);
			l->addWidget(&txt,0);
			root()->setPadding(0);
			root()->setLayout(l);

			clearInput.setJavaScript
			      ("function(o, e) { setTimeout(function() {"
			       "" + txt.jsRef() + ".value='';"
			       "}, 0); }");
			txt.enterPressed().connect(this, &App::send);
			txt.enterPressed().connect(clearInput);
			txt.enterPressed().connect(&txt, &WLineEdit::setFocus);

		}
		virtual ~App(){}
	};

} /* namespace bw3 */
#endif /* APP_H_ */
