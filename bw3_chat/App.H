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
#include "SharedObjManager.H"
#include "htmlfunc.H"
using namespace Wt;
using namespace xaxaxa;
namespace chat
{

	class App:public WApplication
	{
	public:
		//WLineEdit txt;
		SharedObjPtr<long,GenericServer> s;
		ChatWidget c;
		JSlot clearInput;
		void send()
		{
			//s.Get()->Send(TextMessage(txt.text().toUTF8()));
		}
		App(const WEnvironment& env, SharedObjPtr<long,GenericServer> s)
		:WApplication(env),s(s),c(*s.Get())
		{
			enableUpdates();
			setTitle("Chat");
			useStyleSheet("main.css");
			//txt.setStyleClass("chattxt");
			WVBoxLayout* l=new WVBoxLayout();
			l->setSpacing(2);
			l->setContentsMargins(0,0,0,0);
			l->addWidget(&c,1);
			//l->addWidget(&txt,0);
			root()->setPadding(0);
			root()->setLayout(l);

			/*clearInput.setJavaScript
			      ("function(o, e) { setTimeout(function() {"
			       "" + txt.jsRef() + ".value='';"
			       "}, 0); }");
			txt.enterPressed().connect(this, &App::send);
			txt.enterPressed().connect(clearInput);
			txt.enterPressed().connect(&txt, &WLineEdit::setFocus);*/

		}
		virtual ~App(){}
	};

} /* namespace bw3 */
#endif /* APP_H_ */
