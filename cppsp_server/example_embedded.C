#include "server.C"
#include <cpoll/cpoll.H>
using namespace CP;
using namespace cppsp;
int main() {
	Poll p;
	//create server instance
	cppspEmbedded::Server srv(&p,"/");
	
	//per-request state machine
	struct handler: public HandlerBase {
		void process() {
			response->write("aaaaa");
			//flush content and headers; when writeback is done, flushCB() is called
			response->finalize({&handler::flushCB,this});
		}
		void flushCB(Response&) {
			cb();
		}
	};
	
	//attach a custom request router
	srv.attachHandler(makeHandler<handler>());
	
	//bind, listen, and accept, passing received sockets to the server
	Socket s;
	s.bind("0.0.0.0","16971",AF_INET,SOCK_STREAM);
	s.listen();
	srv.listen(s);
	//add socket to epoll, and run the event loop
	p.add(s);
	p.loop();
}
