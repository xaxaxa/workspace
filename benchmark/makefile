
CFLAGS := 
CFLAGS1=-L`pwd`/../lib -I`pwd`/../include $(CFLAGS)
CC := gcc $(CFLAGS1)
CXX := g++ $(CFLAGS1)
all: fftbench
clean:
	rm -rf fftbench
fftbench: fftbench.C
	$(CXX) fftbench.C -o fftbench --std=c++0x -Wno-pmf-conversions -lfftw3 -lpthread -Ofast -march=native



