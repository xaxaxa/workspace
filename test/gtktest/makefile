PKG_CONFIG=`pkg-config --cflags --libs gtkmm-2.4`
FLAGS=-I../../include/ -L../../lib/ --std=c++0x -lgeneric_gui -lwt -lwthttp -g3
a:
	g++ a.C -o a $(FLAGS) $(PKG_CONFIG)
gui_test1:
	g++ gui_test1.C -o gui_test1 $(FLAGS) $(PKG_CONFIG)

