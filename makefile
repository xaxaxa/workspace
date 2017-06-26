
CFLAGS := -Ofast -march=native -ftree-vectorize
CFLAGS1=-L`pwd`/lib -I`pwd`/include -Wall -Wno-pmf-conversions -funsigned-char -fwrapv -fno-delete-null-pointer-checks -fno-strict-aliasing \
	-Wno-unused-function -Wno-unused-variable --std=c++0x -fPIC -D_FILE_OFFSET_BITS=64 -D_GLIBCXX_USE_CXX11_ABI=1 $(CFLAGS)
CC := gcc
CXX := g++

all: server123 termchat tmp1 tmp2 nc.xaxaxa email_extract tcpfuck bitflip_proxy \
	generic_ui generic_struct cplib cpoll cppsp cppsp_standalone socketd_cppsp \
	socketd fbdump http_simplebench buffer jackfft dedup benchmark tcpsdump \
	iptsocks_new
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
jackrecord: bin/jackrecord
jackffts: bin/jackffts
convolve: bin/convolve
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
lowlevel_http: bin/lowlevel_http
fbdview_gen_index: bin/fbdview_gen_index
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
bin/nc.xaxaxa: cpoll nc.xaxaxa.C
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
	$(CXX) cppsp_server/socketd_cppsp.C -o bin/socketd_cppsp -lcpoll -lcppsp -ldl -lrt -lpthread $(CFLAGS1)
bin/cppsp_embedded_example: cppsp cpoll
	$(CXX) cppsp_server/example_embedded.C -o bin/cppsp_embedded_example -lcpoll -lcppsp -ldl -lrt $(CFLAGS1)
bin/socketd: cpoll
	$(CXX) socketd/all.C -o bin/socketd -lcpoll -lrt -lpthread $(CFLAGS1)
bin/tcpsdump: tcpsdump/main.cxx tcpsdump/tcpinterpreter.cxx
	$(CXX) tcpsdump/main.cxx -o bin/tcpsdump -lpcap -lpthread $(CFLAGS1)
bin/rmhttphdr: cplib tcpsdump/rmhttphdr.cxx
	$(CXX) tcpsdump/rmhttphdr.cxx -o bin/rmhttphdr -lcplib -lpthread $(CFLAGS1)
bin/jackfft_static: bin/main2.ui
	$(CXX) jackfft/jackfft.C cpoll/cpoll.C cplib/all.C -o bin/jackfft_static -lpthread -ljack -lfftw3 -lSoundTouch \
	`pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0` $(CFLAGS1)
bin/jackfft: cplib bin/main2.ui
	$(CXX) jackfft/jackfft.C -o bin/jackfft -lcplib -lpthread -ljack -lfftw3 -lSoundTouch \
	`pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0` $(CFLAGS1)
bin/jackfftc: jackfft/jackfftc.C
	$(CXX) jackfft/jackfftc.C -o bin/jackfftc -lpthread -ljack -lfftw3 $(CFLAGS1)
bin/jackfft_analyzer:
	cp -af jackfft/jackfft_analyzer.ui bin/
	$(CXX) jackfft/analyzer.C -o bin/jackfft_analyzer -lcpoll -lpthread -ljack -lfftw3 \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4 gdkmm-3.0 gthread-2.0` $(CFLAGS1)
bin/jackfft_getfir:
	$(CXX) jackfft/getfir.C -o bin/jackfft_getfir -lpthread -lfftw3 $(CFLAGS1)
bin/jackrecord:
	$(CXX) jackfft/jackrecord.C -o bin/jackrecord -lpthread -ljack $(CFLAGS1)
bin/convolve:
	$(CXX) jackfft/convolve.C -o bin/convolve -lpthread -ljack -lfftw3 $(CFLAGS1)
bin/dsssreceiver:
	$(CXX) jackfft/dsssreceiver.C -o bin/dsssreceiver -lpthread -ljack -lfftw3 $(CFLAGS1)
bin/dsssgenerator:
	$(CXX) jackfft/dsssgenerator.C -o bin/dsssgenerator -lpthread -lfftw3 $(CFLAGS1)
bin/jackffts: cplib cpoll bin/main2.ui
	$(CXX) jackfft/jackfft.C -o bin/jackffts -lcpoll -lcplib -lpthread -ljack -lSoundTouch \
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
bin/iptsocks_new: cpoll iptsocks_new/all.C iptsocks_new/main.C
	$(CXX) iptsocks_new/all.C -o bin/iptsocks_new -lcpoll -lpthread $(CFLAGS1)
bin/tcpforward: cpoll iptsocks_new/tcpforward.C
	$(CXX) iptsocks_new/tcpforward.C iptsocks_new/joinstream.C -o bin/tcpforward -lcpoll -lpthread $(CFLAGS1)
bin/decode_aaaaa: test/decode_aaaaa.C
	$(CXX) test/decode_aaaaa.C -o bin/decode_aaaaa -lpthread -lcryptopp $(CFLAGS1)
bin/randomread: test/randomread.C
	$(CXX) test/randomread.C -o bin/randomread $(CFLAGS1)
bin/pie_exec: test/pie_exec.C test/handle_syscall.S
	$(CXX) test/pie_exec.C test/handle_syscall.S -o bin/pie_exec $(CFLAGS1)
bin/lowlevel_http: test/lowlevel_http.C
	$(CXX) test/lowlevel_http.C -o bin/lowlevel_http -lpthread -lcpoll $(CFLAGS1)
bin/pg_html_dump: pg_html_dump.C
	$(CXX) pg_html_dump.C -o bin/pg_html_dump -lpq $(CFLAGS1)
bin/fbdview_gen_index: fbdview/gen_index.C
	$(CXX) fbdview/gen_index.C -o bin/fbdview_gen_index -ljsoncpp -lcpoll -lpthread $(CFLAGS1)
dsssgen: bin/dsssgen
bin/dsssgen: test/dsssgen.C
	$(CXX) test/dsssgen.C -o bin/dsssgen -lpthread -lcryptopp -lfftw3 -lfftw3_threads $(CFLAGS1)

bin/vna_test1: vna/test1.C
	$(CXX) vna/test1.C -o bin/vna_test1 -lpthread `pkg-config --cflags --libs gtkmm-3.0`
	cp -a vna/vna.glade bin/

bin/vna_test2: vna/test2.C
	$(CXX) vna/test2.C -o bin/vna_test2 -lpthread `pkg-config --cflags --libs gtkmm-3.0`
	cp -a vna/vna.glade bin/

bin/test_backup: backup_tools/test_backup.C
	$(CXX) backup_tools/test_backup.C -o bin/test_backup -lpthread -lcryptopp $(CFLAGS1)


# library targets
lib/libgeneric_ui.so:
	$(CXX) generic_ui/all.C --shared -o lib/libgeneric_ui.so $(CFLAGS1)
lib/libgeneric_struct.so:
	$(CXX) generic_struct/all.C --shared -o lib/libgeneric_struct.so $(CFLAGS1)
lib/libcplib.so:
	$(CXX) cplib/all.C --shared -o lib/libcplib.so $(CFLAGS1)
	$(CXX) cplib/all.C -c -o lib/libcplib.o $(CFLAGS1)
	ar rcs lib/libcplib.a lib/libcplib.o
lib/libcpoll.so:
	$(CXX) cpoll/*.C --shared -o lib/libcpoll.so $(CFLAGS1) -lcurl
lib/libcppsp.so:
	$(CXX) cppsp/all.C --shared -o lib/libcppsp.so  -lcryptopp $(CFLAGS1)
lib/libcplib.a:
	$(CXX) cplib/all.C -c -o lib/libcplib.o $(CFLAGS1)
	ar rcs lib/libcplib.a lib/libcplib.o
lib/libcpoll.a:
	$(CXX) cpoll/*.C -c -o lib/libcpoll.o $(CFLAGS1)
	ar rcs lib/libcpoll.a lib/libcpoll.o


cpoll/Makefile: cpoll/configure.ac cpoll/Makefile.in
	cd cpoll && autoreconf && ./configure
