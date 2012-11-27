#include "ui_client.H"
#define BUFSIZE 4096;

struct client {
	RGC::Ref<Socket> s;
	//int8_t buffer[BUFSIZE];
	UIClient uic;
	Instance inst;
	void 
	client(Socket& s): s(s) {
		uic.allow_module_loading=true;
		uic.inst=&inst;
		uic.doListen();
		s.repeatRecv(buffer, BUFSIZE, [this]() {
			
		});
	}
};

int main(int argc, char** argv) {
	Poll p;
	Socket s(AF_INET,SOCK_STREAM);
	s.bind(IPEndPoint(IPAddress("0.0.0.0"),16969));
	s.listen();
	s.repeatAccept([&p](Socket* s1) {
		p.add(*s1);
		new client(*s1);
		s1->release();
	});
	p.add(s);
	p.loop();
}
