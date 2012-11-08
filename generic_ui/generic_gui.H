/*
 * generic_gui.H
 *
 *  Created on: 2012-11-04
 *      Author: xaxaxa
 */

#ifndef GENERIC_GUI_H_
#define GENERIC_GUI_H_
#include "generic_ui.H"
#include <functional>
#include <sys/types.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
using namespace std;
namespace GenericUI
{
class GenericGUI
{
public:
	virtual void load(const function<int(int8_t*, int)>& cb);
	virtual void parse(xmlNode* node)=0;
	virtual void load(const char* file);
	virtual void run()=0;
};

}
#endif /* GENERIC_GUI_H_ */
