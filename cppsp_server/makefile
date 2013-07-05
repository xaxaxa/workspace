OPTFLAGS := -Ofast -march=native -flto
CXXFLAGS := $(OPTFLAGS) -Wall --std=c++0x -Wno-pmf-conversions -I../include
CXX := g++


cppsp_standalone: cppsp_standalone.C
	g++ cppsp_standalone.C -Wall -o cppsp_standalone --std=c++0x -g3 -I../include -L../lib -lcpoll -lcppsp -ldl -lrt -Wno-pmf-conversions
example_embedded: example_embedded.C
	g++ example_embedded.C -Wall -o example_embedded --std=c++0x -g3 -I../include -L../lib -lcpoll -lcppsp -ldl -lrt -Wno-pmf-conversions
socketd_cppsp: socketd_cppsp.C
	g++ socketd_cppsp.C -o socketd_cppsp --std=c++0x -g3 -I../include -L../lib -lcpoll -lcppsp -ldl -Wno-pmf-conversions

cpoll.o:
	$(CXX) ../cpoll/all.C -c -o cpoll.o $(CXXFLAGS) -fPIC
cppsp.o:
	$(CXX) ../cppsp/all.C -c -o cppsp.o $(CXXFLAGS) -fPIC


cppsp_standalone_r: cppsp_standalone.C cpoll.o cppsp.o
	$(CXX) cppsp_standalone.C ../cpoll/all.C ../cppsp/all.C -o cppsp_standalone_r -lpthread -ldl -lrt $(CXXFLAGS)


