all: termchat
install: termchat_install
clean:
	rm -rf termchat
termchat:
	g++ -o "termchat" "termchat.cxx" -Wall -lncurses --std=c++0x -lpthread -lcplib -Icplib/headers/
termchat_install:
	cp -f termchat /usr/local/bin/
