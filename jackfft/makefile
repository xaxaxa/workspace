PKG_CONFIG=`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4 gdkmm-3.0`
jackfft:
	g++ jackfft.C -o jackfft --std=c++0x -lcplib -ljack -lfftw3 $(PKG_CONFIG)
jackfftc:
	g++ jackfftc.C -o jackfftc --std=c++0x -lcplib -ljack -lfftw3
clean:
	rm -f jackfft jackfftc

