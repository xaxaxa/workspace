#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string>
using namespace std;
static const int threshold=30;	// ~ -60dB
static const int silenceCount=441;	//10ms @ 44.1ksps
static const int audioCount=44;		//~1ms @ 44.1ksps
typedef struct __attribute__ ((__packed__)) {
	int16_t l,r;
} Sample;


Sample* hasSilence(Sample* buf, int len, int& state) {
	Sample* end=buf+len;
	int& count=state;
	for(Sample* i=buf;i<end;i++) {
		if(abs(i->l)<threshold && abs(i->r)<threshold) {
			if(++count>silenceCount) return i-count+1;
		} else count=0;
	}
	return NULL;
}
Sample* hasAudio(Sample* buf, int len, int& state) {
	Sample* end=buf+len;
	int& count=state;
	for(Sample* i=buf;i<end;i++) {
		if(abs(i->l)>=threshold && abs(i->r)>=threshold) {
			if(++count>audioCount) return i-count+1;
		} else count=0;
	}
	return NULL;
}
int readAll(int fd, Sample* buf, int len) {
	uint8_t* buf_=(uint8_t*)buf;
	len=len*sizeof(Sample);
	int br;
	while(len>0) {
		if((br=read(fd,buf_,len))<=0) break;
		buf_+=br;
		len-=br;
	}
	return (buf_-(uint8_t*)buf)/sizeof(Sample);
}
int writeAll(int fd, Sample* buf, int len) {
	uint8_t* buf_=(uint8_t*)buf;
	len=len*sizeof(Sample);
	int br;
	while(len>0) {
		if((br=write(fd,buf_,len))<=0) break;
		buf_+=br;
		len-=br;
	}
	return (buf_-(uint8_t*)buf)/sizeof(Sample);
}
//reads S16_LE interleaved 2-channel samples from stdin; 
//removes silence from the beginning and end of the stream
int main() {
	int n=44100;	//1 second
	Sample buf_[n*2];
	Sample* buf=buf_+n;
	int count1=0,count2=0;
	string tmpdata;
	int br,br_;
	
	while(true) {
		br=br_=readAll(0,buf_+n,n);
		if(br_<=0) return 0;
		Sample* i;
		if((i=hasAudio(buf,br,count1))!=NULL) {
			fprintf(stderr,"found audio; starting operation\n");
			br=buf+br-i;
			buf=i;
			count1=0;
			goto aaaaa;
		}
		memcpy(buf_+n-br_,buf_+n,br_*sizeof(Sample));
	}

	while(true) {
		memcpy(buf_+n-br_,buf_+n,br_*sizeof(Sample));
		br=br_=readAll(0,buf_,n);
		buf=buf_;
		if(br<=0) break;
		Sample* i;
	aaaaa:
		if(tmpdata.length()==0) {
			if((i=hasSilence(buf,br,count1))!=NULL && hasAudio(i,buf+br-i,count2)==NULL) {
				fprintf(stderr,"found silence; buffering all data for now\n");
				tmpdata.append((char*)(i),(buf+br-i)*sizeof(Sample));
				writeAll(1,buf,i-buf);
			} else {
				writeAll(1,buf,br);
				count2=0;
			}
		} else {
			if((i=hasAudio(buf,br,count2))!=NULL) {
				fprintf(stderr,"found audio; writing buffer and resuming operation\n");
				writeAll(1,(Sample*)tmpdata.data(),tmpdata.length()/sizeof(Sample));
				tmpdata.clear();
				count2=0;
				writeAll(1,buf,i-buf);
				br=buf+br-i;
				buf=i;
				goto aaaaa;
			} else {
				tmpdata.append((char*)(buf),br*sizeof(Sample));
			}
		}
	}
	return 0;
}
