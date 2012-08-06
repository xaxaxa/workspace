ARGS=-g3 --std=c++0x -rdynamic -I../../include/ -L../../l/ -lv8_base -lv8_snapshot -pthread -lcurl -levent -lssl -lcplib
all: v8_test fbdump
clean:
	rm -f v8_test fbdump *.o *.gch
v8_test:
	g++ v8_test.C -o v8_test $(ARGS)
fbdump: curl_httpreq.o
	g++ fbdump.C curl_httpreq.o -o fbdump $(ARGS)
urdl.o:
	g++ -c ../../urdl*/src/urdl.cpp -o ./urdl.o  -I../../include/ -pthread -lboost_system -lboost_date_time -lssl
curl_httpreq.o:
	g++ -c curl_httpreq.C -o curl_httpreq.o --std=c++0x
