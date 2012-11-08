#include <iostream>
#include <string>
#include <functional>
#include "generic_ui.H"
#include "cout_object.H"
using namespace GenericUI;
using namespace GenericUI::Aliases;

using namespace std;
uint32_t cnt=0;
bool cb(EventTypes e, const EventData& ed)
{
	switch(e) {
		case EventTypes::newInterface:
			fprintf(stderr, "debug_ui: newInterface: %s\n", ed.name.c_str());
			ed.interface->listen(&cb);
			break;
		case EventTypes::newSlot:
			fprintf(stderr, "debug_ui: newSlot: %s.%s\n", ed.interface->name.c_str(), ed.name.c_str());
			ed.slot->listen(&cb);
			break;
		case EventTypes::newAttachment:
			fprintf(stderr, "debug_ui: newAttachment: %s.%s\n", ed.interface->name.c_str(), ed.name.c_str());
			break;
		case EventTypes::invocation:
			fprintf(stderr, "debug_ui: invocation %i: %s.%s; arg=", (++cnt), ed.interface->name.c_str(), ed.name.c_str());
			*(uint32_t*)&ed.scope = cnt;
			cerr << ed.o << endl;
			break;
		case EventTypes::ret:
			fprintf(stderr, "debug_ui: ret %i: %s.%s; val=", *(uint32_t*)&ed.scope, ed.interface->name.c_str(), ed.name.c_str());
			cerr << ed.o << endl;
			break;
		case EventTypes::loadUI:
			fprintf(stderr, "debug_ui: loadUI: %s\n", ed.name.c_str());
			break;
		default:
			fprintf(stderr, "debug_ui: unknown event %i\n", (int)e);
	}
	return true;
}

__attribute__((constructor)) void init()
{
	listen(cb);
}
