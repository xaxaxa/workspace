#include <iostream>
#include <cpoll/cpoll.H>
#include <signal.h>
#include <list>

using namespace CP;
using namespace std;
using namespace RGC;

struct bufferManager
{
	Buffer curBuf;
	int curIndex=0;
	Buffer beginGet() {
		if(curBuf==nullptr) {
			int bs=1024*16;
			return curBuf=Buffer(bs);
		}
		else return curBuf;
	}
	void endGet(int usedLen) {
		curBuf.clip(usedLen);
		if(curBuf.length()<=0) curBuf.release();
	}
};
int main() {
	StandardStream ss;
	struct handler {
		Ref<Stream> s1,s2;
		list<Buffer> queue;
		list<Buffer>::iterator last_written;
		bufferManager bm;
		Buffer rbuf;
		vector<iovec> iovecs;
		bool writing=false;
		bool shouldExit=false;
		void startR1() {
			rbuf=bm.beginGet();
			s1->read(rbuf,{&handler::cbR1,this});
		}
		void cbR1(int r) {
			if(r<=0) {
				if(writing) shouldExit=true;
				else exit(0);
			}
			bm.endGet(r);
			rbuf.clip(0,r);
			queue.push_back(rbuf);
			rbuf.release();
			startW2();
			startR1();
		}
		
		void startW2() {
			if(writing) return;
			if(queue.size()<=0) {
				if(shouldExit)exit(0);
				return;
			}
			writing=true;
			iovecs.resize(queue.size());
			auto it=queue.begin();
			for(int i=0;i<iovecs.size();i++) {
				iovecs[i].iov_base=(*it).data();
				iovecs[i].iov_len=(*it).length();
				it++;
			}
			last_written=it;
			s2->writevAll(&iovecs[0],iovecs.size(),{&handler::cbW2,this});
		}
		void cbW2(int r) {
			//printf("cbW2: %i\n",r);
			if(r<=0) exit(0);
			queue.erase(queue.begin(),last_written);
			writing=false;
			startW2();
		}
		handler(Stream& s1, Stream& s2):s1(&s1),s2(&s2) {
			startR1();
		}
	} h(ss,ss);
	Poll p;
	ss.addToPoll(p);
	ss.setBlocking(false);
	p.loop();
}
