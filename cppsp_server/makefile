cppsp_standalone: cppsp_standalone.C
	g++ cppsp_standalone.C -Wall -o cppsp_standalone --std=c++0x -g3 -I../include -L../lib -lcpoll -lcppsp -ldl -lrt -Wno-pmf-conversions
socketd_cppsp: socketd_cppsp.C
	g++ socketd_cppsp.C -o socketd_cppsp --std=c++0x -g3 -I../include -L../lib -lcpoll -lcppsp -ldl -Wno-pmf-conversions
cppsp_standalone_r: cppsp_standalone.C
	g++ cppsp_standalone.C -Wall -o cppsp_standalone --std=c++0x -I../include -L../lib -lcpoll -lcppsp -ldl -lrt -Wno-pmf-conversions -Ofast -march=native -flto 
