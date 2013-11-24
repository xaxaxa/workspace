
CFLAGS := -Ofast -march=native 
CFLAGS1=-L`pwd`/lib -I`pwd`/include -Wall -Wno-pmf-conversions \
	-Wno-unused-function -Wno-unused-variable --std=c++0x -fPIC -D_FILE_OFFSET_BITS=64 $(CFLAGS)
CC := gcc
CXX := g++
all: server123 termchat tmp1 tmp2 nc.xaxaxa email_extract tcpfuck bitflip_proxy \
	generic_ui generic_struct cplib cpoll cppsp cppsp_standalone socketd_cppsp \
	socketd fbdump http_simplebench buffer jackfft dedup benchmark tcpsdump
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
	
tcpsdump: bin/tcpsdump bin/rmhttphdr
jackfft: bin/jackfft
jackffts: bin/jackffts
dedup: bin/dedup
benchmark: fftbench fibbench
fftbench: bin/fftbench
fftfbench: bin/fftfbench
fftsbench: bin/fftsbench
fibbench: bin/fibbench
iptsocks_new: bin/iptsocks_new
cppsp_embedded_example: bin/cppsp_embedded_example
decode_aaaaa: bin/decode_aaaaa
randomread: bin/randomread
pie_exec: bin/pie_exec
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
bin/buffer: buffer.C cpoll
	$(CXX) buffer.C -o bin/buffer -lcpoll -lpthread $(CFLAGS1)
bin/http_simplebench: http_simplebench.C cpoll
	$(CXX) http_simplebench.C -o bin/http_simplebench -lcpoll -lpthread $(CFLAGS1)
bin/tcpfuck: tcpfuck.C cpoll
	$(CXX) tcpfuck.C -o bin/tcpfuck -lcpoll -lpthread $(CFLAGS1)
bin/bitflip_proxy: bitflip_proxy.C cpoll
	$(CXX) bitflip_proxy.C -o bin/bitflip_proxy -lcpoll -lpthread $(CFLAGS1)

bin/fbdump: generic_ui generic_struct cplib
	$(CXX) js/fbdump.C js/curl_httpreq.C -o bin/fbdump -lgeneric_ui -lgeneric_struct\
		-lpthread -lv8 -lcplib -ldl -lcurl -levent $(CFLAGS1)
	cp -f js/fbdump.js bin/
lib/fbdump_cui.so:
	$(CXX) js/fbdump_cui.C --shared -o lib/fbdump_cui.so \
		`pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0` $(CFLAGS1)
bin/cppsp_standalone: cppsp cpoll
	$(CXX) cppsp_server/cppsp_standalone.C -o bin/cppsp_standalone -lcpoll -lcppsp -ldl -lrt -lpthread $(CFLAGS1)
bin/socketd_cppsp: cppsp cpoll
	$(CXX) cppsp_server/socketd_cppsp.C -o bin/socketd_cppsp -lcpoll -lcppsp -ldl -lrt $(CFLAGS1)
bin/cppsp_embedded_example: cppsp cpoll
	$(CXX) cppsp_server/example_embedded.C -o bin/cppsp_embedded_example -lcpoll -lcppsp -ldl -lrt $(CFLAGS1)
bin/socketd: cpoll
	$(CXX) socketd/all.C -o bin/socketd -lcpoll -lrt $(CFLAGS1)
bin/tcpsdump:
	$(CXX) tcpsdump/main.cxx -o bin/tcpsdump -lpcap -lpthread $(CFLAGS1)
bin/rmhttphdr: cplib
	$(CXX) tcpsdump/rmhttphdr.cxx -o bin/rmhttphdr -lcplib -lpthread $(CFLAGS1)
bin/jackfft: cplib cpoll bin/main2.ui
	$(CXX) jackfft/jackfft.C -o bin/jackfft -lcpoll -lcplib -lpthread -ljack -lfftw3f -lSoundTouch \
	`pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0` -DJACKFFT_USE_FLOAT $(CFLAGS1)
bin/jackffts: cplib cpoll bin/main2.ui
	$(CXX) jackfft/jackfft.C -o bin/jackfft -lcpoll -lcplib -lpthread -ljack -lSoundTouch \
	`pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0` -DJACKFFT_USE_FLOAT \
	/usr/local/lib/libffts.a -DJACKFFT_USE_FFTS $(CFLAGS1)
bin/main2.ui:
	cp -f jackfft/main2.ui bin/
	cp -f trollface200.png bin/
bin/dedup:
	$(CXX) dedup/dedup.C -o bin/dedup -lpthread $(CFLAGS1)
bin/fftbench:
	$(CXX) benchmark/fftbench.C -o bin/fftbench -lpthread -lfftw3 $(CFLAGS1)
bin/fftfbench:
	$(CXX) benchmark/fftbench.C -o bin/fftfbench -lpthread -lfftw3f -DUSE_FFTWF $(CFLAGS1)
bin/fftsbench:
	$(CXX) benchmark/fftsbench.C -o bin/fftsbench -lpthread /usr/local/lib/libffts.a $(CFLAGS1)
bin/fibbench:
	$(CXX) benchmark/fibbench.C -o bin/fibbench -lpthread $(CFLAGS1)
bin/iptsocks_new: cpoll iptsocks_new/all.C
	$(CXX) iptsocks_new/all.C -o bin/iptsocks_new -lcpoll -lpthread $(CFLAGS1)
bin/decode_aaaaa: test/decode_aaaaa.C
	$(CXX) test/decode_aaaaa.C -o bin/decode_aaaaa -lpthread -lcryptopp $(CFLAGS1)
bin/randomread: test/randomread.C
	$(CXX) test/randomread.C -o bin/randomread $(CFLAGS1)
bin/pie_exec: test/pie_exec.C test/handle_syscall.S
	$(CXX) test/pie_exec.C test/handle_syscall.S -o bin/pie_exec $(CFLAGS1)
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
	$(CXX) cppsp/all.C --shared -o lib/libcppsp.so  -lcryptopp $(CFLAGS1)
lib/libcplib.a:
	$(CXX) cplib/all.C -c -o lib/libcplib.o $(CFLAGS1)
	ar rcs lib/libcplib.a lib/libcplib.o
lib/libcpoll.a:
	$(CXX) cpoll/all.C -c -o lib/libcpoll.o $(CFLAGS1)
	ar rcs lib/libcpoll.a lib/libcpoll.o

