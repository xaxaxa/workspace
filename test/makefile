all:

execd:
	g++ -Wall -o execd execd.C --std=c++0x -I../include -L../lib -lgeneric_ui -lgeneric_struct -pthread -ldl -lcpoll -g3
sha_name_brute:
	g++ sha_name_brute.C -o sha_name_brute --std=c++0x -pthread -lcryptopp -Ofast -march=native
fcgi_test:
	g++ fcgi_test.C -g3 --std=c++0x -o fcgi_test -lfcgi
cpoll_statemachine_test:
	g++ cpoll_statemachine_test.C -O3 --std=c++0x -o cpoll_statemachine_test -lcpoll -I../include -L../lib
a:
	g++ a.C -o a -O3 -I../include -L../lib -lcplib --std=c++0x
socketd_test:
	g++ socketd_test.C -o socketd_test -g3 -I../include -L../lib -lcpoll --std=c++0x
socketd_proxy.so:
	g++ socketd_proxy.C -o socketd_proxy.so -O3 -I../include -L../lib -lcpoll -ldl -pthread --std=c++0x --shared -fPIC


