#include "server.C"
#include <cpoll/cpoll.H>
using namespace CP;
using namespace cppsp;
int main() {
	//create server instance
	cppspServer::Server srv("/");
	
	//per-request state machine
	struct handler {
		Request& req; Response& resp; Delegate<void()> cb;
		void process() {
			resp.write("aaaaa");
			//flush content and headers; when writeback is done, flushCB() is called
			resp.flush({&handler::flushCB,this});
		}
		void flushCB(Response&) {
			cb();
		}
	};
	
	//attach a custom request router
	struct {
		void operator()(Request& req, Response& resp, Delegate<void()> cb) {
			//allocate an instance of handler in the StringPool allocator
			req.sp->New<handler>(handler{req,resp,cb})->process();
		}
	} router;
	srv.handleRequest.attach(&router);
	
	//bind, listen, and accept, passing received sockets to the server
	Poll p;
	Socket s;
	s.bind("0.0.0.0","16971",AF_INET,SOCK_STREAM);
	s.listen();
	
	//callback function that's called for every accept()ed connection
	auto acceptCB=[&](Socket* clientSock) {
		//create a new connection handler
		new cppspServer::handler(srv,p,*clientSock);
		
		//release our reference to the socket; the connection handler also holds
		//a reference, which it will automatically release when the connection is finished
		clientSock->release();
	};
	s.repeatAccept(&acceptCB);
	
	//add socket to epoll, and run the event loop
	p.add(s);
	p.loop();
}
