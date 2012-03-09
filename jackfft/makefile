PKG_CONFIG=`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4 gdkmm-3.0`
all: jackfft jackfftc
jackfft:
	g++ -g3 jackfft.C -o jackfft --std=c++0x -lrt -lcplib -ljack -lfftw3 $(PKG_CONFIG)
jackfftc:
	g++ -g3 jackfftc.C -o jackfftc --std=c++0x -lrt -lcplib -ljack -lfftw3
freqrecord:
	 g++ -g3 freqrecord.C -o freqrecord --std=c++0x -lcplib -ljack -lfftw3 $(PKG_CONFIG)
clean:
	rm -f jackfft jackfftc

