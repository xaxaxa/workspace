/*
 * generic_gui_gtk.H
 *
 *  Created on: 2012-11-04
 *      Author: xaxaxa
 */

#ifndef GENERIC_GUI_GTK_H_
#define GENERIC_GUI_GTK_H_

#include "generic_gui.H"
#include <gtkmm.h>
#include <iostream>
namespace GenericUI
{
	
	class GenericGUI_gtk: public GenericGUI
	{
	public:

		struct NodeData
		{
			Gtk::Widget* w;
		};
		map<string, function<NodeData*(xmlNode*)> > controls;
		GenericGUI_gtk();
		virtual ~GenericGUI_gtk();
		virtual void run();
		virtual void parse(xmlNode* node);
		void processNode(xmlNode* node);
	};

} /* namespace GenericUI */
#endif /* GENERIC_GUI_GTK_H_ */
