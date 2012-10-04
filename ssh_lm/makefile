CF=--std=c++0x `pkg-config --cflags --libs gthread-2.0 gtkmm-2.4 glibmm-2.4`
all: ssh_lm
ssh_lm:
	g++ ssh_lm.C -o ssh_lm $(CF)
clean:
	rm -f ssh_lm

