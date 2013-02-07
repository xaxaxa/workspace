#include <cpoll/statemachines.H>
#include <cpoll/cpoll.H>
#include <unistd.h>
#include <cpoll/sendfd.H>
#include <iostream>
#include <socketd.H>
using namespace std;
using namespace socketd;

int main() {
	cout << "started child #" << getpid() << endl;
	CP::Socket s(3, AF_UNIX, SOCK_STREAM, 0);
	protocolHeader ph;
	int reqs=0;
	while(s.read(&ph, sizeof(ph))>0) {
		if(ph.type==protocolHeader::handleConnection) {
			cout << "protocolHeader::handleConnection" << endl;
			prot_handleConnection ph1;
			if(s.read(&ph1,sizeof(ph1))<=0) return 1;
			//cout << "read prot_handleConnection" << endl;
			int fd=recvfd(s.handle);
			//cout << "received fd " << fd << endl;
			const char* resp="HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n"
			"Connection: close\r\nContent-Length: 9\r\n\r\ntrollolol";
			write(fd,resp,strlen(resp));
			if((++reqs)>5) {
				//recycle
				return 0;
			}
		}
	}
	
}
