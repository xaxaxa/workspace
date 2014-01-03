CFLAGS := -g3 -march=native 
CFLAGS1=-L`pwd`/lib -I`pwd`/include -Wall -Wno-pmf-conversions \
	-Wno-unused-function -Wno-unused-variable --std=c++0x -fPIC -D_FILE_OFFSET_BITS=64 $(CFLAGS)
CC := gcc
CXX := g++
ARCH := i386
all: libukernel.so ukernel_test
	
install: termchat_install nc.xaxaxa_install
clean:
	rm -f bin/* arch/include/*
libukernel.so: bin/libukernel.so
	
ukernel_test: bin/ukernel_test
	
bin/libukernel.so: arch_includes bin/arch.a
	$(CXX) all.C bin/arch.a --shared -o $@ $(CFLAGS1) -lpthread
bin/ukernel_test: bin/libukernel.so ukernel_test.C
	$(CXX) ukernel_test.C -o $@ $(CFLAGS1) -L./bin -lukernel

arch_includes: arch/include/process.H arch/include/misc.H
arch/include/process.H:
	echo "#include \"../$(ARCH)/process.H\"" > $@
arch/include/misc.H:
	echo "#include \"../$(ARCH)/misc.H\"" > $@
bin/arch.a:
	$(MAKE) -C arch/$(ARCH) CFLAGS1="$(CFLAGS1)"
	mv arch/$(ARCH)/all.a bin/arch.a
