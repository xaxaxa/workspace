/*
 * proxyviewport.C
 *
 *  Created on: Apr 13, 2013
 *      Author: xaxaxa
 */

#include "include/webviewport.H"

namespace GenericGUI
{
	namespace Viewports
	{
		static void WebViewport::registerImplementations() {

		}
		WebViewport::WebViewport():tmpw(nullptr) {

		}
		void WebViewport::run() {
		}
		WebImplementation::WebImplementation(Control* control, Viewport* viewport) :
				Implementation(control, viewport) {
			WebViewport* vp=(WebViewport*)viewport;
		}
		WebImplementation::~WebImplementation() {
		}
		void WebViewport::registerScript(const char* s, int l) {
		}
		void WebViewport::generateEventCall(const char* expr, int exprLen) {
		}
		void WebViewport::render(CP::StreamWriter& w) {
		}
		void WebImplementation::render(CP::StreamWriter& w) {
		}
		void WebImplementation::renderChildren(CP::StreamWriter& w) {
		}
		void WebImplementation::notifyClientEvent(WebClientEvent& evt) {
		}
	
	}
}

