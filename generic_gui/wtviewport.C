/*
 * WTViewport.C
 *
 *  Created on: Dec 19, 2012
 *      Author: xaxaxa
 */

#include "include/wtviewport.H"

namespace GenericGUI
{
	namespace Viewports
	{
		WTViewport::WTViewport(int argc, char* argv[]) {
		}

		void WTViewport::run() {
			throw logic_error("WTViewport::run() should not be called; use the WT event loop");
		}
		
		WTViewport::~WTViewport() {
		}
		WTImplementation::WTImplementation(Control* control, Viewport* viewport, Wt::WWidget* w) :
				Implementation(control, viewport), widget(w) {
		}

		WTImplementation::~WTImplementation() {
			if (unlikely(widget == NULL)) return;
			delete widget;
			widget = NULL;
		}

	}
} /* namespace GenericGUI */

