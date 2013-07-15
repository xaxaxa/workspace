
CFLAGS := -Ofast -march=native
CFLAGS1=-L`pwd`/lib -I`pwd`/include -Wall -Wno-pmf-conversions --std=c++0x $(CFLAGS)
CC := gcc
CXX := g++
all: server123 termchat tmp1 tmp2 nc.xaxaxa email_extract tcpfuck bitflip_proxy \
	generic_ui generic_struct cplib cpoll cppsp cppsp_standalone socketd_cppsp \
	socketd fbdump http_simplebench buffer
install: termchat_install nc.xaxaxa_install
clean:
	rm -rf termchat servertroll tmp tmp1 tmp2 nc.xaxaxa server123 lib/* bin/*
# aliases
generic_ui: lib/libgeneric_ui.so
generic_struct: lib/libgeneric_struct.so
cplib: lib/libcplib.so
cpoll: lib/libcpoll.so
cppsp: lib/libcppsp.so
cppsp_standalone: bin/cppsp_standalone
socketd_cppsp: bin/socketd_cppsp
socketd: bin/socketd
fbdump: bin/fbdump lib/fbdump_cui.so
email_extract: bin/email_extract
	
termchat: bin/termchat
	
server123: bin/server123
	
tmp: bin/tmp
	
tmp1: bin/tmp1
	
tmp2: bin/tmp2
	
nc.xaxaxa: bin/nc.xaxaxa
	
buffer: bin/buffer
	
http_simplebench: bin/http_simplebench
	
tcpfuck: bin/tcpfuck
	
bitflip_proxy: bin/bitflip_proxy
	
# binary targets
bin/email_extract: email_extract.C cplib
	$(CXX) email_extract.C -o bin/email_extract -lcplib $(CFLAGS1)
bin/termchat: termchat.cxx cplib
	$(CXX) -o bin/termchat termchat.cxx -lncurses -lpthread -lcplib $(CFLAGS1)
bin/server123: cplib
	$(CXX) server123.C -o bin/server123 -lcplib $(CFLAGS1)
bin/tmp: tmp.C cplib
	$(CXX) tmp.C -o bin/tmp -lcplib $(CFLAGS1)
bin/tmp1: tmp1.C cplib
	$(CXX) tmp1.C -o bin/tmp1 -lcplib $(CFLAGS1)
bin/tmp2: tmp2.C cplib
	$(CXX) tmp2.C -o bin/tmp2 -lcplib $(CFLAGS1)
bin/nc.xaxaxa: cpoll
	$(CXX) nc.xaxaxa.C -o bin/nc.xaxaxa -lcpoll -lpthread $(CFLAGS1)
bin/buffer: buffer.C
	$(CXX) buffer.C -o bin/buffer -lcpoll -lpthread $(CFLAGS1)
bin/http_simplebench: http_simplebench.C
	$(CXX) http_simplebench.C -o bin/http_simplebench -lcpoll -lpthread $(CFLAGS1)
bin/tcpfuck: tcpfuck.C cpoll
	$(CXX) tcpfuck.C -o bin/tcpfuck -lcpoll -lpthread $(CFLAGS1)
bin/bitflip_proxy: bitflip_proxy.C cpoll
	$(CXX) bitflip_proxy.C -o bin/bitflip_proxy -lcpoll -lpthread $(CFLAGS1)

bin/fbdump: generic_ui generic_struct cplib
	$(CXX) js/fbdump.C js/curl_httpreq.C -o bin/fbdump -lgeneric_ui -lgeneric_struct\
		-lpthread -lv8 -lcplib -ldl -lcurl -levent $(CFLAGS1)
lib/fbdump_cui.so:
	$(CXX) js/fbdump_cui.C --shared -o lib/fbdump_cui.so \
		`pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0` $(CFLAGS1)
bin/cppsp_standalone: cppsp
	$(CXX) cppsp_server/cppsp_standalone.C -o bin/cppsp_standalone -lcpoll -lcppsp -ldl -lrt $(CFLAGS1)
bin/socketd_cppsp: cppsp
	$(CXX) cppsp_server/socketd_cppsp.C -o bin/socketd_cppsp -lcpoll -lcppsp -ldl -lrt $(CFLAGS1)
bin/socketd:
	$(CXX) socketd/all.C -o bin/socketd -lcpoll -lrt $(CFLAGS1)
# library targets
lib/libgeneric_ui.so:
	$(CXX) generic_ui/all.C --shared -o lib/libgeneric_ui.so $(CFLAGS1)
lib/libgeneric_struct.so:
	$(CXX) generic_struct/all.C --shared -o lib/libgeneric_struct.so $(CFLAGS1)
lib/libcplib.so:
	$(CXX) cplib/all.C --shared -o lib/libcplib.so $(CFLAGS1)
lib/libcpoll.so:
	$(CXX) cpoll/all.C --shared -o lib/libcpoll.so $(CFLAGS1)
lib/libcppsp.so:
	$(CXX) cppsp/all.C --shared -o lib/libcppsp.so $(CFLAGS1)

