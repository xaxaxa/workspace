ARGS=-g3 -fPIC -march=native --std=c++0x `pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0` -rdynamic -I../include/ -L../lib -lv8 -pthread -lcurl -levent -lssl -lcplib -lgeneric_ui -lgeneric_struct
all: v8_test fbdump fbdump_cui.so
clean:
	rm -f v8_test fbdump *.so *.o *.gch
v8_test:
	g++ v8_test.C -o v8_test $(ARGS)
fbdump: curl_httpreq.o
	g++ fbdump.C curl_httpreq.o -o fbdump -fPIC -fPIE -pie $(ARGS)
fbdump_cui.so:
	g++ -shared -fPIC fbdump_cui.C -o fbdump_cui.so -Wl,--unresolved-symbols=ignore-all $(ARGS)
urdl.o:
	g++ -c -fPIC ../../urdl*/src/urdl.cpp -o ./urdl.o  -I../../include/ -pthread -lboost_system -lboost_date_time -lssl
curl_httpreq.o:
	g++ -c -fPIC curl_httpreq.C -o curl_httpreq.o --std=c++0x


//-Wl,--unresolved-symbols=ignore-all
