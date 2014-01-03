/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */

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
