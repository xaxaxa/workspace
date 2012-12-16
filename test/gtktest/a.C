#include <gtkmm.h>

int main(int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    Gtk::Window window;
    Gtk::Window w;
	//window.add(w);
	w.show();
	window.show();
    Gtk::Main::run(window);
    
    return 0;
}

