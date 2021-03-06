#include "fftfilter.C"
#include "jfftfile.H"

#include <alsa/asoundlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <arpa/inet.h>

using namespace std;
using namespace xaxaxa;

bool jackfftPrintWarnings=true;

typedef int16_t sample_t;
int sample_scale = 16383;


int readAll(int fd, void* b, int len) {
	char* buf=(char*)b;
	int br=0;
	while(br<len) {
		int r=read(fd,buf+br,len-br);
		if(r<=0) break;
		br+=r;
	}
	return br;
}

int main(int argc, char** argv) {
	const char *device = "default";
	int channels = 2;
	int srate = 44100;
	
	int filterBufSize = 1024;
	int filterOverlap = 2;
	int filterPeriod = 8;
	int filterPaddingL = 4;
	int filterPaddingR = 0;
	int minBuffers = filterPeriod + filterPaddingL + filterPaddingR - 1;


	int filterInBuffers = minBuffers+20;
	int filterOutBuffers = filterInBuffers;
	int filterFFTSize = 1024*16;
	// if the source is sending data faster than it is being played and
	// the buffer fills up, rewind by this many buffers to restore low latency
	int filterOverrunRewindBuffers = 20;
	int complexSize = filterFFTSize / 2 + 1;
	
	int stdinBufSize = 1024*channels;
	
	
	int err;
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;
	
	
	
	
	if(argc<2) {
		cerr << "usage: " << argv[0] << " file.jfft [device]" << endl;
		return 1;
	}
	ifstream jfftStream(argv[1]);
	stringstream jfftBuffer;
	jfftBuffer << jfftStream.rdbuf();
	string jfft = jfftBuffer.str();
	
	if(argc>2) {
		device = argv[2];
	}
	
	
	if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		fprintf(stderr, "Playback open error: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}

	snd_pcm_format_t fmt;
	if(sizeof(jackfft_float) == 8) fmt = SND_PCM_FORMAT_FLOAT64_LE;
	else fmt = SND_PCM_FORMAT_FLOAT_LE;
	if ((err = snd_pcm_set_params(handle,
								fmt,
								SND_PCM_ACCESS_RW_NONINTERLEAVED,
								channels,
								srate,
								1,
								500000)) < 0) {   /* 0.5sec */
		fprintf(stderr, "Playback open error: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
	if ((err = snd_pcm_nonblock(handle, 1)) < 0) {
		fprintf(stderr, "set pcm nonblocking: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
	
	FFTFilter<sample_t>* filters[channels];
	for(int i=0; i<channels; i++) {
		filters[i] = new FFTFilter<sample_t>(filterBufSize, filterInBuffers,
			filterOutBuffers, filterOverlap, filterPeriod, filterPaddingL,
			filterPaddingR, filterFFTSize);
		loadjfft((uint8_t*)jfft.data(), jfft.length(), filters[i]->coefficients,complexSize, srate);
	}
	
	
	while(true) {
		sample_t buf[stdinBufSize];
		
		// read interleaved data into buf
		if(readAll(0,buf,sizeof(buf))<=0) break;
		
		printf("read %d samples\n", stdinBufSize);
		
		// de-interleave buf and append to filters' ring buffer
		for(int i=0; i<channels; i++) {
			int appended = filters[i]->inBuffer.append([&](jackfft_float* dst, int off, int len) {
				int tmp=0;
				for(int j=off*channels+i; j<stdinBufSize; j+=channels) {
					dst[tmp++]=jackfft_float(buf[j])/sample_scale;
				}
			}, stdinBufSize/channels);
			filters[i]->inBuffer.commit(appended);
			if(appended != stdinBufSize/channels) {
				// buffer full; drop some data to restore low latency
				filters[i]->inBuffer.uncommit(filterOverrunRewindBuffers*filterBufSize);
			}
		}
		
		// run filter and write to alsa until alsa queue is full
		int len=0;
		void* bufs[channels];
	repeat:
		for(int i=0; i<channels; i++) {
			if(filters[i]->outBuffer.length()<=0)
				filters[i]->processAvailableData();
			jackfft_float* tmp = filters[i]->outBuffer.ncpeek(len);
			if(len<=0) goto cont;
			bufs[i] = tmp;
		}
		{
			int bw = snd_pcm_writen(handle, bufs, len);
			if(bw < 0) bw = snd_pcm_recover(handle, bw, 0);
			if(bw < 0 && bw!=-EAGAIN) {
				fprintf(stderr, "snd_pcm_writen failed: %s\n", snd_strerror(bw));
				return 1;
			}
			if(bw > 0) {
				printf("wrote %d samples\n", bw);
				for(int i=0; i<channels; i++)
					filters[i]->outBuffer.consume(bw);
				goto repeat;
			}
		}
	cont:;
	}
}
