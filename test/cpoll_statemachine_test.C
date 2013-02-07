#include <cpoll/statemachines.H>
#include <cpoll/cpoll.H>
#include <unistd.h>

using namespace CP;
int main() {
	
	uint8_t sr_data[streamReader_getSize()+4096];
	streamReader* sr=(streamReader*)sr_data;
	streamReader_init(sr,4096);
	int count=0;
	auto cb1=[sr,&count](uint8_t* buf, int len, bool last) {
		//write(2,buf,len);
		if(last) {
			//const char* ch="\nNEWLINE\n";
			//write(2,ch,strlen(ch));
			count++;
			streamReader_readUntilString(sr, "as");
		}
	};
	streamReader_setCallback(sr, cb1);
	streamReader_readUntilString(sr, "as");
	bool reading=false;
	CP::File f(0);
	
	function<void()> start_read;
	start_read=[&]() {
		if(reading) return;
		uint8_t* buf; int len;
		std::tie(buf,len) = streamReader_beginPutData(sr);
		if(len<=0) return;
		reading=true;
		f.read(buf, len, [sr,&start_read,&reading](int r) {
			reading=false;
			if(r<=0) {
				streamReader_endPutData(sr,0);
				return;
			}
			streamReader_endPutData(sr,r);
			start_read();
		});
	};
	/*start_read();
	CP::Poll p;
	p.add(f);
	p.loop();*/
	
	//synchronous read
	while(true) {
		uint8_t* buf; int len;
		std::tie(buf,len) = streamReader_beginPutData(sr);
		int r=f.read(buf,len);
		if(r<=0) {
			streamReader_endPutData(sr,0);
			break;
		}
		streamReader_endPutData(sr, r);
	}
	
	//force flush the buffered data
	{
		uint8_t* buf; int len;
		std::tie(buf,len) = streamReader_getBufferData(sr);
		cb1(buf, len, true);
		streamReader_reset(sr);
	}
	
	streamReader_deinit(sr);
	cout << count << endl;
}

