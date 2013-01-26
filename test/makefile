all:

execd:
	g++ -Wall -o execd execd.C --std=c++0x -I../include -L../lib -lgeneric_ui -lgeneric_struct -pthread -ldl -lcpoll -g3
sha_name_brute:
	g++ sha_name_brute.C -o sha_name_brute --std=c++0x -pthread -lcryptopp -Ofast -march=native
fcgi_test:
	g++ fcgi_test.C -g3 --std=c++0x -o fcgi_test -lfcgi


