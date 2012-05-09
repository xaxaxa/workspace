all: termchat tmp tmp1 tmp2 nc.xaxaxa
install: termchat_install nc.xaxaxa_install
clean:
	rm -rf termchat servertroll tmp tmp1 tmp2 nc.xaxaxa
termchat:
	g++ -o "termchat" "termchat.cxx" -Wall -lncurses --std=c++0x -lpthread -lcplib -Icplib/headers/
termchat_install:
	cp -f termchat /usr/local/bin/
tmp:
	g++ tmp.C -o tmp --std=c++0x -lcplib -Ofast
tmp1:
	g++ tmp1.C -o tmp1 --std=c++0x -lcplib -Ofast
tmp2:
	g++ tmp2.C -o tmp2 --std=c++0x -lcplib -Ofast
nc.xaxaxa:
	g++ nc.xaxaxa.C -o nc.xaxaxa --std=c++0x -lcplib -Ofast
nc.xaxaxa_install:
	cp -f nc.xaxaxa /usr/local/bin/

