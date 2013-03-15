#include <iostream>
//#include <cplib/cplib.hpp>
//#include <cplib/asyncsock.hpp>
#include <cpoll.H>
#include <signal.h>

using namespace CP;
int main() {
	Poll p;
	Socket s;
	s.bind("0.0.0.0","7000",AF_INET,SOCK_STREAM);
	s.listen();
	struct {
		Poll& p;
		void operator()(Socket* s) {
			cout << "assssssss" << endl;
			
			struct handler {
				Poll& p;
				StreamReader sr;
				RGC::Ref<Socket> s;
				
				void lineCB(const string& l) {
					cout << "got line: " << l << endl;
					struct header {
						int a;
						void* v;
						char pad[8192];
						int x;
					}* h=(header*)l.data();
					h->pad[h->a]=h->x;
					delete this;
				}
				handler(Poll& p, Socket& s): p(p),sr(s),s(s) {
					sr.readLine(StreamReader::Callback(&handler::lineCB,this));
				}
			} *hdlr=new handler(p,*s);
			p.add(*s);
			s->release();
		}
	} cb {p};
	s.repeatAccept(&cb);
	
	p.add(s);
	p.loop();
}






