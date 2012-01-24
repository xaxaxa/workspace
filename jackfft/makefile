PKG_CONFIG=`pkg-config --libs --cflags gtkmm-2.4`
all: jackfft jackfftc
jackfft:
	g++ $(PKG_CONFIG) jackfft.C -o jackfft --std=c++0x -lcplib -ljack -lfftw
jackfftc:
	g++ jackfftc.C -o jackfftc --std=c++0x -lcplib -ljack -lfftw3

