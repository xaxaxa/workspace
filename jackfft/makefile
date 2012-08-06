PKG_CONFIG=`pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0`
all: jackfft jackfftc
jackfft:
	g++ -g3 -Ofast -pthread jackfft.C -o jackfft --std=c++0x -lrt -lcplib -ljack -lfftw3 $(PKG_CONFIG)
jackfftc:
	g++ -g3 -Ofast -pthread jackfftc.C -o jackfftc --std=c++0x -lrt -lcplib -ljack -lfftw3
freqrecord:
	g++ -g3 freqrecord.C -o freqrecord --std=c++0x -lcplib -ljack -lfftw3 $(PKG_CONFIG)
freqplay:
	g++ -g3 freqplay.C -o freqplay --std=c++0x -lcplib -ljack -lfftw3 $(PKG_CONFIG)
clean:
	rm -f jackfft jackfftc
install:
	cp jackfft jackfftc main2.ui trollface200.png /usr/local/bin/
