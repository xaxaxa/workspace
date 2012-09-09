FLAGS=-I../include/ -L../lib/
all: main rmhttphdr
main:
	g++ main.cxx -g3 --std=c++0x -o main -lpcap -lpthread $(FLAGS)
rmhttphdr:
	g++ rmhttphdr.cxx -g3 --std=c++0x -o rmhttphdr -lcplib $(FLAGS)
clean:
	rm -f main rmhttphdr
install:
	cp -f main /usr/local/bin/tcpsdump
	cp -f rmhttphdr /usr/local/bin/
	setcap cap_net_raw=eip /usr/local/bin/tcpsdump

