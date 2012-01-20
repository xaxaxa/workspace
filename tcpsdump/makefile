all: main rmhttphdr
main:
	g++ main.cxx --std=c++0x -o main -lpcap -lpthread
rmhttphdr:
	g++ rmhttphdr.cxx --std=c++0x -o rmhttphdr -lcplib
clean:
	rm main rmhttphdr

