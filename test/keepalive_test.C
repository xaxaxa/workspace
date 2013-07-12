#include <cpoll/cpoll.H>
using namespace CP;
String data;
int conns=0;
int maxconns;
int closed=0;
struct connectionHandler
{
	Socket s;
	String buf;
	void operator()(int i) {
		if(i!=0) printf("connection failed\n");
		else {
			conns++;
			s.writeAll(data.data(),data.length(),{&connectionHandler::writeCB,this});
		}
	}
	void writeCB(int i) {
		if(i<=0) {
			printf("%p: write error\n",this);
			end(); return;
		}
		if((buf.d=(char*)malloc(4096))==nullptr) {
			perror("malloc");
			end(); return;
		}
		buf.len=4096;
		s.repeatRead(buf.data(),buf.length(),{&connectionHandler::readCB,this});
	}
	void readCB(int i) {
		if(i<=0) {
			printf("%p: connection closed\n",this);
			closed++;
			end(); return;
		}
	}
	void end() {
		delete this;
	}
};
int main(int argc, char** argv) {
	if(argc<5) {
		printf("usage: %s host port uri connections\n",argv[0]);
		return 1;
	}
	MemoryStream ms;
	StreamWriter sw(ms);
	sw.writeF("GET %s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\n\r\n",argv[3],argv[1]);
	sw.flush();
	data=ms.String();
	
	Poll p;
	EndPoint* ep=nullptr;
	maxconns=atoi(argv[4]);
	
	for(int i=0;i<maxconns;i++) {
		connectionHandler* h=new connectionHandler();
		if(ep==nullptr) {
			h->s.connect(argv[1],argv[2],AF_INET,SOCK_STREAM);
			ep=h->s.getRemoteEndPoint();
			p.add(h->s);
			(*h)(0);
		} else {
			h->s.init(ep->addressFamily,SOCK_STREAM,0);
			p.add(h->s);
			h->s.connect(*ep,h);
		}
	}
	Timer t((uint64_t)500);
	struct {
		Timer& t;
		int lastC;
		bool connected;
		void operator()(int i) {
			if(!connected) {
				if(conns>lastC) {
					if(conns>=maxconns) {
						printf("%i connections\n",conns);
						connected=true;
						lastC=0;
						return;
					} else printf("%i connections so far\n",conns);
					lastC=conns;
				}
			} else {
				if(closed>lastC) {
					printf("%i connections left\n",conns-closed);
					lastC=closed;
				}
			}
		}
	} tcb {t,0,false};
	t.setCallback(&tcb);
	p.add(t);
	p.loop();
}
