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

anonfunc_test:
	g++ -Wno-pmf-conversions anonfunc_test.C -o anonfunc_test --std=c++0x -g3 -I../include -L../lib
delegate_perf.so:
	g++ -Wno-pmf-conversions delegate_perf.C -o delegate_perf.so --shared --std=c++0x -O3 -I../include -L../lib
delegate_perftest: delegate_perf.so
	g++ -Wno-pmf-conversions delegate_perftest.C delegate_perf.so -o delegate_perftest --std=c++0x -O3 -I../include -L../lib
streamwriter_test:
	g++ streamwriter_test.C -o streamwriter_test --std=c++0x -O3 -I../include -L../lib -lcpoll -Wno-pmf-conversions
streambuffer_test:
	g++ streambuffer_test.C -o streambuffer_test --std=c++0x -O3 -I../include -L../lib -lcpoll -Wno-pmf-conversions
streamreader_test:
	g++ streamreader_test.C -o streamreader_test --std=c++0x -O3 -I../include -L../lib -lcpoll -Wno-pmf-conversions
vuln:
	g++ vuln.C -o vuln --std=c++0x -O3 -I../include -L../lib -lcpoll -Wno-pmf-conversions
image_steg:
	g++ image_steg.C -o image_steg -lMagick++
cppsp_test: cppsp_test.C
	g++ cppsp_test.C -o cppsp_test --std=c++0x -g3 -I../include -L../lib -lcpoll -lcppsp -ldl -Wno-pmf-conversions
socketd_cppsp: socketd_cppsp.C
	g++ socketd_cppsp.C -o socketd_cppsp --std=c++0x -g3 -I../include -L../lib -lcpoll -lcppsp -ldl -Wno-pmf-conversions
ldtest1:
	g++ ldtest1.C -o ldtest1.so --shared -g3 -I../include -L../lib
ldtest:
	g++ ldtest.C -o ldtest -g3 -I../include -L../lib -ldl
urlparser_test: urlparser_test.C
	g++ urlparser_test.C -o urlparser_test -g3 -I../include -L../lib --std=c++0x -lcpoll -lcppsp -ldl
httptest: httptest.C
	g++ httptest.C -o httptest -g3 -I../include -L../lib --std=c++0x -lcpoll -ldl -lcurl
simplehttp: simplehttp.C
	g++ simplehttp.C -o simplehttp --std=c++0x -g3 -I../include -L../lib -lcpoll -ldl -Wno-pmf-conversions
streamtest:
	g++ streamtest.C -o streamtest --std=c++0x -g3 -I../include -L../lib -lcpoll -Wno-pmf-conversions
keepalive_test: keepalive_test.C
	g++ keepalive_test.C -o keepalive_test --std=c++0x -g3 -I../include -L../lib -lcpoll -Wno-pmf-conversions
so_vuln:
	g++ so_vuln.C -o so_vuln --std=c++0x -g3 -I../include -L../lib -lcpoll -Wno-pmf-conversions

