#include <cpoll.H>
#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>

using namespace std;

struct handlerItem {
	string pattern;
	string file;
};
vector<handlerItem> handlers;
handlerItem defaultHandler;
map<string,string> mime={{"htm","text/html"},{"mp3","audio/mp3"}};
int reqs=0;
int total=0;
void inc_req()
{
	//cerr << "++requests processing = " << (++reqs) << endl;
}
void dec_req()
{
	//cerr << "--requests processing = " << (--reqs) << endl;
}
string getExt(string s)
{
	int i;
	if((i=s.rfind('.'))>=0) return s.substr(i+1);
	return "";
}
void sendReply(const handlerItem& h, CP::Socket& s, CP::Poll& p)
{
	struct stat st;
	if(stat(h.file.c_str(), &st)!=0) return;
	
	string mimeType="text/html";
	auto it=mime.find(getExt(h.file));
	if(it!=mime.end()) mimeType=(*it).second;
	
	s.retain();
	struct struc {
		CP::Callback fileRead;		//read callback
		CP::Callback socketSend;	//send callback
		//function<void()> destruct;
		char buf[4096];
		string str;
	};
	struc *cbs=new struc();
	stringstream ss;
	ss << "HTTP/1.1 200 OK\r\nContent-Length: " << st.st_size
		<< "\r\nContent-Type: " << mimeType << "\r\nConnection: close\r\n\r\n";
	cbs->str=ss.str();
	s.sendAll(cbs->str.data(), cbs->str.length(), 0, [&h, &s, &p, cbs](int br) {
		if(br<=0) {
			dec_req();
			s.release();
			delete cbs;
			return;
		}
		CP::File* f;
		try {
			f=new CP::File(open(h.file.c_str(), O_RDONLY));
		} catch(exception& ex) {
			dec_req();
			s.release();
			delete cbs;
			return;
		}
		p.add(*f);
		
		cbs->fileRead=[cbs, &s, f](int br) {
			if(br<=0) {
				s.shutdown(SHUT_WR);
				s.repeatRecv(cbs->buf, 4096, 0, [&s, f, cbs](int br) {
					if(br<=0) {
						dec_req();
						s.release();
						f->release();
						delete cbs;
					}
				});
				return;
			}
			s.sendAll(cbs->buf, br, 0, cbs->socketSend);
		};
		cbs->socketSend=[cbs, &s, f](int bw) {
			if(bw<=0) {
				dec_req();
				s.release();
				f->release();
				delete cbs;
				return;
			}
			f->read(cbs->buf, 4096, cbs->fileRead);
		};
		f->read(cbs->buf, 4096, cbs->fileRead);
	});
}
//const char* httphdr="HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";
//int httphdr_len=strlen(httphdr);
int main(int argc, char** argv)
{
	const char* listenAddr="0.0.0.0";
	int port=80;
	if(argc>1)defaultHandler.file=argv[1];
	else {
		cout << "usage: " << argv[0] << " defaultpage [listenaddress] [port]"
		<< " [pattern1] [page1] [patternN] [pageN] ..." << endl;
		return 1;
	}
	if(argc>2)listenAddr=argv[2];
	if(argc>3)port=atoi(argv[3]);
	for(int i=4;(i+1)<argc;i+=2) {
		handlers.push_back({argv[i], argv[i+1]});
	}
	CP::Poll poll;
	CP::Socket s;
	poll.add(s);
	s.bind(CP::IPEndPoint(CP::IPAddress(listenAddr), port));
	s.listen();
	s.repeatAccept([&poll](CP::Socket* new_s) {
		//cerr << "request #" << (++total) << " received" << endl;
		inc_req();
		poll.add(*new_s);
		string* tmps=new string();
		tmps->resize(4096);
		
		//assumes that the request uri is in the first packet and that
		//it is within the first 4096 bytes. until cpoll gets proper
		//HTTP support, this will do.
		new_s->recv((char*)tmps->data(),4096,0,[tmps,new_s,&poll](int br) {
			if(br<=0) return; //socket will now be closed because there
								//are no more data transfers and we release()ed it
			//cerr << "request read" << endl;
			tmps->resize(br);
			//match all patterns against request
			for(int i=0;i<handlers.size();i++)
				if(tmps->find(handlers[i].pattern)!=string::npos) {
					//found; now send the reply
					sendReply(handlers[i], *new_s, poll);
					return;
				}
			sendReply(defaultHandler, *new_s, poll);
		});
		//release it now, but it won't be destructed until all data transfers
		//have completed
		new_s->release();
	});
	poll.loop();
}
