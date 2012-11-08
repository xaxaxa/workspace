#include <generic_ui.H>
#include <generic_gui_gtk.H>
#include <stdio.h>
using namespace GenericUI;
int main(int argc, char** argv)
{
	if(argc<2) {
		printf("usage: %s file.xml\n", argv[0]);
		return 1;
	}
	const char* f=argv[1];
	GenericGUI_gtk gui;
	gui.load(f);
	gui.run();
	return 0;
}
