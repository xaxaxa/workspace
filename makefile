CFLAGS=-L ~/bin -I ~/include
all: termchat tmp tmp1 tmp2 nc.xaxaxa
install: termchat_install nc.xaxaxa_install
clean:
	rm -rf termchat servertroll tmp tmp1 tmp2 nc.xaxaxa
termchat:
	g++ -o "termchat" "termchat.cxx" -Wall -lncurses --std=c++0x -lpthread -lcplib $(CFLAGS)
termchat_install:
	cp -f termchat /usr/local/bin/
server123:
	g++ server123.C -o server123 --std=c++0x -lcplib -O3 $(CFLAGS)
tmp:
	g++ tmp.C -o tmp --std=c++0x -lcplib -O3 $(CFLAGS)
tmp1:
	g++ tmp1.C -o tmp1 --std=c++0x -lcplib -O3 $(CFLAGS)
tmp2:
	g++ tmp2.C -o tmp2 --std=c++0x -lcplib -O3 $(CFLAGS)
nc.xaxaxa:
	g++ nc.xaxaxa.C -o nc.xaxaxa --std=c++0x -lcplib -O3 $(CFLAGS)
nc.xaxaxa_install:
	cp -f nc.xaxaxa /usr/local/bin/

