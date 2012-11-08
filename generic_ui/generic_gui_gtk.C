/*
 * generic_gui_gtk.C
 *
 *  Created on: 2012-11-04
 *      Author: xaxaxa
 */

#include "generic_gui_gtk.H"
using namespace std;
namespace GenericUI
{
	
	GenericGUI_gtk::GenericGUI_gtk() {
		// TODO Auto-generated constructor stub
		controls.insert({"",[](){}});
	}
	
	GenericGUI_gtk::~GenericGUI_gtk() {
		// TODO Auto-generated destructor stub
	}
	
	void GenericGUI_gtk::run() {
	}
	
	void GenericGUI_gtk::parse(xmlNode* node) {
		processNode(node);
	}

	void GenericGUI_gtk::processNode(xmlNode* node) {
		xmlElementType t = node->type;
		if (t != XML_ELEMENT_NODE) return;
		//if (strcmp(node->name, "") == 0) cout << node->name << endl;

		//children
		xmlNode* tmp = node->children;
		while (tmp != NULL) {
			processNode(tmp);
			tmp = tmp->next;
		}
	}

} /* namespace GenericUI */
