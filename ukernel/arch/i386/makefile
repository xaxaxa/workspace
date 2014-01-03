all: all.a
	
all.a:
	$(CXX) process.C misc.S -c $(CFLAGS1)
	ar rcs all.a process.o misc.o
