
CFLAGS := -Ofast -march=native
CFLAGS1 = --std=c++0x -Wno-pmf-conversions -L`pwd`/../lib -I`pwd`/../include $(CFLAGS)
LIBS := -lrt -lpthread
CC := gcc $(CFLAGS1)
CXX := g++ $(CFLAGS1)
all: fftbench
clean:
	rm -rf fftbench
fftbench: fftbench.C
	$(CXX) fftbench.C -o fftbench -lfftw3 $(LIBS)
fibbench: fibbench.C
	$(CXX) fibbench.C -o fibbench $(LIBS)

