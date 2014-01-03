#include <cpoll/cpoll.H>
#include <unistd.h>

using namespace CP;
class myStream: public Stream
{
public:
	Timer f;
	Callback _cb;
	myStream() {
		
	}
	int32_t read(void* buf, int32_t len) override {
		
	}
	int32_t write(const void* buf, int32_t len) override {
		
	}
	void read(void* buf, int32_t len, const Callback& cb, bool repeat = false) override {
		_cb=cb;
		memcpy(buf,"asdfg",len<5?len:5);
		f.setCallback({&myStream::timerCB,this});
		f.setInterval(200);
	}
	void timerCB(int n) {
		f.setInterval(0);
		_cb(5);
	}
	void write(const void* buf, int32_t len, const Callback& cb, bool repeat = false) override {
		
	}
	//sync
	void close() override {}
	void flush() override {}

	//async
	void close(const Callback& cb) override {}
	void flush(const Callback& cb) override {}
	
	void cancelRead() override {}
	void cancelWrite() override {}
};
int main(int argc, char** argv) {
	Poll p;
	myStream s;
	Buffer b(50);
	auto cb=[&](int i) {
		write(2,b.data(),b.length());
	};
	s.readAll(b,&cb);
	
	p.add(s.f);
	p.loop();
}
