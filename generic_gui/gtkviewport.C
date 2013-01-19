/*
 * gtkviewport.C
 *
 *  Created on: Dec 8, 2012
 *      Author: xaxaxa
 */
#include "include/gtkviewport.H"
namespace GenericGUI
{
	namespace Viewports
	{
		GTKViewport::GTKViewport(int argc, char *argv[]) :
				main(argc, argv) {

		}
		void GTKViewport::run() {
			main.run();
		}
		GTKImplementation::GTKImplementation(Control* control, Viewport* viewport, Gtk::Widget* w) :
				Implementation(control, viewport), widget(w) {

		}
		GTKImplementation::~GTKImplementation() {
			if (unlikely(widget == NULL)) return;
			delete widget;
			widget = NULL;
		}
	}
}

