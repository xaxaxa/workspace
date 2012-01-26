all: main rmhttphdr
main:
	g++ main.cxx -g3 --std=c++0x -o main -lpcap -lpthread
rmhttphdr:
	g++ rmhttphdr.cxx -g3 --std=c++0x -o rmhttphdr -lcplib
clean:
	rm -f main rmhttphdr

