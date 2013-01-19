/*
 * gtkviewport.H
 *
 *  Created on: Dec 8, 2012
 *      Author: xaxaxa
 */

#ifndef GTKVIEWPORT_H_
#define GTKVIEWPORT_H_
#include "control.H"
#include <gtkmm.h>
namespace GenericGUI
{
	namespace Viewports
	{
		class GTKViewport: public Viewport
		{
		public:
			Gtk::Main main;
			GTKViewport(int argc, char *argv[]);
			void run();
		};
		class GTKImplementation: public Implementation
		{
		public:
			Gtk::Widget* widget;
			GTKImplementation(Control* control, Viewport* viewport, Gtk::Widget* w);
			~GTKImplementation();
			Gtk::Widget* getWidget() {
				return widget;
			}
		};
	}
}

#endif /* GTKVIEWPORT_H_ */
