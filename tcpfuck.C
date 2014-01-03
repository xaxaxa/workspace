#include <cpoll/cpoll.H>
#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
	const char* listenAddr="0.0.0.0";
	int port=80;
	if(argc>1)listenAddr=argv[1];
	if(argc>2)port=atoi(argv[2]);
	
	CP::Poll poll;
	CP::Socket s;
	poll.add(s);
	s.bind(CP::IPEndPoint(CP::IPAddress(listenAddr), port));
	s.listen();
	auto cb=[&poll](CP::Socket* new_s) {
		
	};
	s.repeatAccept(&cb);
	poll.loop();
}
