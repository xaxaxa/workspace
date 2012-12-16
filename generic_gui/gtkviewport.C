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
		GTKImplementation::GTKImplementation(Control* control, Viewport* viewport) :
				Implementation(control, viewport), widget(NULL) {

		}
		GTKImplementation::~GTKImplementation() {
			delete widget;
			widget = NULL;
		}
		void GTKImplementation::initialize() {
			Implementation::initialize();
			createWidget();
		}
	}
}

