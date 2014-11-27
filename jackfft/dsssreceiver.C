#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fftw3.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

typedef double jackfft_float;
typedef jackfft_float jackfft_complex[2];

int srate;

template<class T>
T* jackfft_alloc(int c) {
	T* tmp = (T*)fftw_malloc(c*sizeof(T));
	if(tmp==NULL) throw bad_alloc();
	return tmp;
}
void jackfft_free(void* v) {
	fftw_free(v);
}
string itoa(int i) {
	char buf[32];
	snprintf(buf,sizeof(buf),"%i",i);
	return string(buf);
}

static inline void mult(jackfft_complex& c1, jackfft_complex& c2, double r) {
	jackfft_float r1=c1[0]*r;
	jackfft_float i1=c1[1]*r;
	c1[0]=(r1*c2[0]-i1*c2[1]);
	c1[1]=(r1*c2[1]+i1*c2[0]);
}
struct convolve_c
{
	jackfft_complex* buffer;
	jackfft_complex* filt;
	jackfft_complex* tmp1;
	jackfft_complex* tmp2;
	int os,bs;
	fftw_plan p1,p2;
	void init(int overlapSize, int bufSize) {
		os=overlapSize; bs=bufSize;
		buffer=jackfft_alloc<jackfft_complex>(os+bs);
		filt=jackfft_alloc<jackfft_complex>(os+bs);
		tmp1=jackfft_alloc<jackfft_complex>(os+bs);
		tmp2=jackfft_alloc<jackfft_complex>(os+bs);
		p1=fftw_plan_dft_1d(os+bs,buffer,tmp1,FFTW_FORWARD,FFTW_PRESERVE_INPUT);
		p2=fftw_plan_dft_1d(os+bs,tmp1,tmp2,FFTW_BACKWARD,0);
	}
	void deinit() {
		jackfft_free(buffer);
	}
	jackfft_complex* beginProcess(int sz) {
		memmove(buffer,buffer+sz,(os+bs-sz)*sizeof(jackfft_complex));
		jackfft_complex* buffer1=buffer+os+bs-sz;
		return buffer1;
	}
	jackfft_complex* endProcess(int sz) {
		fftw_execute(p1);
		double aaaaa=1.0/double((os+bs));
		for(int i=0;i<(os+bs);i++) {
			mult(tmp1[i],filt[i],aaaaa);
		}
		fftw_execute(p2);
		return tmp2+os+bs-sz;
	}
	void setWaveform(jackfft_complex* data) {
		jackfft_complex* tmpbuf=jackfft_alloc<jackfft_complex>(os+bs);
		fftw_plan tmpp1=fftw_plan_dft_1d(os+bs,tmpbuf,filt,FFTW_FORWARD,0);
		memcpy(tmpbuf,data,os*sizeof(jackfft_complex));
		for(int i=os;i<bs+os;i++) {
			tmpbuf[i][0]=0;
			tmpbuf[i][1]=0;
		}
		fftw_execute(tmpp1);
		fftw_destroy_plan(tmpp1);
		jackfft_free(tmpbuf);
	}
	void setWaveform_reversed(jackfft_float* data) {
		jackfft_complex* tmpbuf=jackfft_alloc<jackfft_complex>(os+bs);
		fftw_plan tmpp1=fftw_plan_dft_1d(os+bs,tmpbuf,filt,FFTW_FORWARD,0);
		for(int i=0;i<os;i++) {
			tmpbuf[i][0]=data[os-i-1];
			tmpbuf[i][1]=0;
		}
		for(int i=os;i<bs+os;i++) {
			tmpbuf[i][0]=0;
			tmpbuf[i][1]=0;
		}
		fftw_execute(tmpp1);
		fftw_destroy_plan(tmpp1);
		jackfft_free(tmpbuf);
	}
};

double scale_value(double x)
{
	double tmp = x - 1.0;
	if(tmp<0) return 1.0/(-tmp*19.0+1.0);
	else return tmp*19.0+1.0;
}
double scale_value_r(double x)
{
	if(x>1.0)x=(x-1.0)/19.0;
	else x=(-((1/x)-1.0)/19.0);
	if(isnan(x)) return 1.0;
	else return x+1.0;
}
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


convolve_c c;
jack_port_t* port;
jack_port_t* port_out;
void error (const char *desc) {
	fprintf (stderr, "JACK error: %s\n", desc);
}
void jack_shutdown (void *arg) {
	exit(3);
}
double theta=0;
//center frequency
double fc=9000;
int process (jack_nframes_t length, void *arg) {
	jack_default_audio_sample_t *in =
				(jack_default_audio_sample_t *) jack_port_get_buffer (port, length);
	jack_default_audio_sample_t *out =
				(jack_default_audio_sample_t *) jack_port_get_buffer (port_out, length);

	jackfft_complex* buf=c.beginProcess((int)length);
	for(unsigned int i=0;i<length;i++) {
		buf[i][0]=in[i]*cos(theta+i*fc*2*M_PI/srate);
		buf[i][1]=in[i]*sin(theta+i*fc*2*M_PI/srate);
	}
	/*for(unsigned int i=0;i<length;i++) {
		buf[i][0]=in[i];
		buf[i][1]=0;
	}*/
	theta+=length*fc*2*M_PI/srate;
	jackfft_complex* data=c.endProcess((int)length);
	for(int x=0;x<(int)length;x++) out[x]=sqrt(pow(data[x][0],2)+pow(data[x][1],2));
	return 0;
}
int main(int argc, char** argv) {
	if(argc<3) {
		printf("usage: %s codeseq.pcm fractionalbandwidth\nonly 64-bit float pcm files supported\n",argv[0]);
		return 1;
	}
	int fd,ss,fbw;
	{
		struct stat st;
		if(stat(argv[1],&st)!=0) {
			perror("stat"); return 2;
		}
		ss=st.st_size/sizeof(double);
	}
	fd=open(argv[1],O_RDONLY);
	if(fd<0) {
		perror("open"); return 2;
	}
	fbw=atoi(argv[2]);
	
	jack_client_t *client;
	jack_set_error_function(error);
	JackStatus st;
	if ((client = jack_client_open ("dsssreceiver",JackNoStartServer,&st)) == 0) {
		fprintf(stderr, "could not connect to server: status %i\n",st); return 1;
	}
	jack_set_process_callback(client, process, 0);
	jack_on_shutdown(client, jack_shutdown, 0);
	int bs=jack_get_buffer_size(client);
	printf("sample rate: %i\nbuffer size: %i\n", srate=jack_get_sample_rate(client),bs);
	
	jackfft_float* coeff_t=jackfft_alloc<jackfft_float>(ss);
	jackfft_complex* tmpbuf=jackfft_alloc<jackfft_complex>(ss/2+1);
	
	fftw_plan tmpp1=fftw_plan_dft_r2c_1d(ss,coeff_t,tmpbuf,0);
	readAll(fd,coeff_t,ss*sizeof(jackfft_float));
	//upsample
	printf("%f %f %f\n",coeff_t[0],coeff_t[10],coeff_t[100]);
	fftw_execute(tmpp1);
	printf("%f %f %f\n",tmpbuf[0][0],tmpbuf[10][1],tmpbuf[100][0]);
	fftw_destroy_plan(tmpp1);

	jackfft_complex* tmpbuf2=jackfft_alloc<jackfft_complex>(ss*fbw/2+1);
	jackfft_float* coeff2_t=jackfft_alloc<jackfft_float>(ss*fbw);
	
	tmpp1=fftw_plan_dft_c2r_1d(ss*fbw,tmpbuf2,coeff2_t,0);
	//memcpy(tmpbuf2,tmpbuf,sizeof(jackfft_complex)*(ss/2+1));
	for(int i=0;i<(ss/2+1);i++) {
		tmpbuf2[i][0]=tmpbuf[i][0];
		tmpbuf2[i][1]=tmpbuf[i][1];
	}
	for(int i=ss/2+1;i<(ss*fbw/2+1);i++) {
		tmpbuf2[i][0]=0;
		tmpbuf2[i][1]=0;
	}
	fftw_execute(tmpp1);
	fftw_destroy_plan(tmpp1);
	jackfft_free(coeff_t);
	jackfft_free(tmpbuf);
	jackfft_free(tmpbuf2);
	for(int i=0;i<ss*fbw/2;i++) {
		double tmp=coeff2_t[i];
		coeff2_t[i]=coeff2_t[ss*fbw/2+i];
		coeff2_t[ss*fbw/2+i]=tmp;
	}
	double aaaaa=1.0/sqrt(ss*fbw*ss);
	for(int i=0;i<ss*fbw;i++) {
		coeff2_t[i]*=aaaaa;
	}
	
	port=jack_port_register(client, "in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	port_out=jack_port_register(client, "baseband", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	c.init(ss*fbw,bs);
	c.setWaveform_reversed(coeff2_t);
	
	if (jack_activate(client)) {
		fprintf(stderr, "cannot activate client");
		return 1;
	}
	while(true)sleep(1000);
	jack_client_close(client);
}
