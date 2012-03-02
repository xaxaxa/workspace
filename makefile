all: termchat
install: termchat_install
clean:
	rm -rf termchat
termchat:
	g++ -Wall -lncurses --std=c++0x -lpthread -lcplib -Icplib/headers/ -o "termchat" "termchat.cxx"
termchat_install:
	cp -f termchat /usr/local/bin/
