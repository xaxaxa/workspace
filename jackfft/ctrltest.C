/*
 * ctrltest.C
 * 
 * Copyright 2012  <xaxaxa@xaxaxa-mac>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include "eqcontrol.C"
using namespace xaxaxa;

int main(int argc, char **argv)
{
	Gtk::Main kit(argc, argv);
	Glib::RefPtr<Gtk::Builder> b;
	b = Gtk::Builder::create_from_file("container.glade");
	Gtk::Window* w;
	b->get_widget("window1",w);
	EQControl c(50);
	w->add(c);
	c.set_hexpand(true);
	c.set_vexpand(true);
	c.show();
	kit.run(*w);
	return 0;
}

