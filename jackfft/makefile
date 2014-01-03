PKG_CONFIG=`pkg-config --cflags --libs gtkmm-2.4 glibmm-2.4 gdkmm-2.4 gthread-2.0`
FLAGS=-I../include/ -L../lib/ -g3 --std=c++0x -lcpoll
all: jackfft jackfftc converters
jackfft:
	g++ -g3 -Ofast -pthread jackfft.C -o jackfft --std=c++0x -lrt -lcplib -ljack -lfftw3 $(FLAGS) $(PKG_CONFIG)
ctrltest:
	g++ -g3 -Ofast -pthread ctrltest.C -o ctrltest --std=c++0x -lrt -lcplib $(FLAGS) $(PKG_CONFIG)
jackfftc:
	g++ -g3 -Ofast -pthread jackfftc.C -o jackfftc --std=c++0x -lrt -lcplib -ljack -lfftw3 $(FLAGS)
freqrecord:
	g++ -g3 freqrecord.C -o freqrecord --std=c++0x -lcplib -ljack -lfftw3 $(FLAGS) $(PKG_CONFIG)
freqplay:
	g++ -g3 freqplay.C -o freqplay --std=c++0x -lcplib -ljack -lfftw3 $(FLAGS) $(PKG_CONFIG)
modulate:
	g++ -g3 -Ofast -pthread modulate.C -o modulate --std=c++0x -lrt -ljack -lfftw3 $(FLAGS)
converters: jack_f2i jack_i2f
transform1:
	g++ transform1.C -o transform1
jack_f2i:
	g++ -Ofast jack_f2i.C -o jack_f2i -ljack $(FLAGS)
jack_i2f:
	g++ -Ofast jack_i2f.C -o jack_i2f -ljack $(FLAGS)
clean:
	rm -f jackfft jackfftc jack_f2i jack_i2f
install:
	cp -f jackfft jackfftc jack_i2f jack_f2i main2.ui trollface200.png /usr/local/bin/

