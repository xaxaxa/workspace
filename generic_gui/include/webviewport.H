/*
 * proxyviewport.H
 *
 *  Created on: Apr 13, 2013
 *      Author: xaxaxa
 */

#ifndef PROXYVIEWPORT_H_
#define PROXYVIEWPORT_H_

#include "control.H"
#include <cpoll/cpoll.H>

namespace GenericGUI
{
	namespace Viewports
	{
		struct WebClientEvent
		{
			const char* data;
			int dataLen;
		};
		class WebViewport: public Viewport
		{
		public:
			static void registerImplementations();
			CP::StreamWriter* tmpw;
			vector<Control*> topLevelControls;
			WebViewport();
			void run();
			void registerScript(const char* s, int l);
			void registerScript(const char* s) {
				registerScript(s, strlen(s));
			}
			//generate javascript code to raise a clientEvent on the server side
			//expr is a javascript expression whose value is also sent to the server
			void generateEventCall(const char* expr, int exprLen);
			void generateEventCall(const char* expr) {
				generateEventCall(expr, strlen(expr));
			}
			void render(CP::StreamWriter& w);
		};
		class WebImplementation: public Implementation
		{
		public:
			WebImplementation(Control* control, Viewport* viewport);
			~WebImplementation();
			virtual void render(CP::StreamWriter& w);
			virtual void renderChildren(CP::StreamWriter& w);
			virtual void notifyClientEvent(WebClientEvent& evt);
		};
	}
}

#endif /* PROXYVIEWPORT_H_ */
