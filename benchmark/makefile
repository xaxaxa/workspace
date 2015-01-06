
CFLAGS := -O3 -march=native -ftree-vectorize -msse3 -mfpmath=sse
CFLAGS1 = --std=c++0x -Wno-pmf-conversions -L`pwd`/../lib -I`pwd`/../include $(CFLAGS)
LIBS := -lrt -lpthread
CC := gcc $(CFLAGS1)
CXX := g++ $(CFLAGS1)
all: fftbench
clean:
	rm -rf fftbench fibbench memcpybench simplefftbench
fftbench: fftbench.C
	$(CXX) fftbench.C -o fftbench -lfftw3 $(LIBS)
simplefftbench: simplefftbench.C
	$(CXX) simplefftbench.C -o simplefftbench $(LIBS)
fibbench: fibbench.C
	$(CXX) fibbench.C -o fibbench $(LIBS)
memcpybench: memcpybench.C
	$(CXX) memcpybench.C -o memcpybench $(LIBS)
