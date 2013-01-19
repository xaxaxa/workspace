/*
 * WTViewport.H
 *
 *  Created on: Dec 19, 2012
 *      Author: xaxaxa
 */

#ifndef WTVIEWPORT_H_
#define WTVIEWPORT_H_
#include "control.H"
#include <Wt/WApplication>
namespace GenericGUI
{
	namespace Viewports
	{
		class WTViewport: public Viewport
		{
		public:
			Wt::WApplication* app;
			WTViewport(int argc, char *argv[]);
			void run();
			virtual ~WTViewport();
		};
		class WTApplication: public Wt::WApplication
		{
		public:
			WTViewport viewport;
			WTApplication(const Wt::WEnvironment& env) :
					Wt::WApplication(env), viewport(0, NULL) {
				viewport.app = this;
			}
		};
		class WTImplementation: public Implementation
		{
		public:
			Wt::WWidget* widget;
			WTImplementation(Control* control, Viewport* viewport, Wt::WWidget* w);
			~WTImplementation();
			Wt::WWidget* getWidget() {
				return widget;
			}
		};
	}
} /* namespace GenericGUI */
#endif /* WTVIEWPORT_H_ */
