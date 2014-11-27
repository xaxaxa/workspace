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
	return (T*)fftw_malloc(c*sizeof(T));
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
	jackfft_float r1=c1[0]/r;
	jackfft_float i1=c1[1]/r;
	c1[0]=(r1*c2[0]-i1*c2[1]);
	c1[1]=(r1*c2[1]+i1*c2[0]);
}
struct convolve
{
	jackfft_float* buffer;
	jackfft_complex* filt;
	jackfft_complex* tmp1;
	jackfft_float* tmp2;
	int os,bs;
	fftw_plan p1,p2;
	void init(int overlapSize, int bufSize) {
		os=overlapSize; bs=bufSize;
		buffer=jackfft_alloc<jackfft_float>(os+bs);
		filt=jackfft_alloc<jackfft_complex>((os+bs)/2+1);
		tmp1=jackfft_alloc<jackfft_complex>((os+bs)/2+1);
		tmp2=jackfft_alloc<jackfft_float>(os+bs);
		p1=fftw_plan_dft_r2c_1d(os+bs,buffer,tmp1,FFTW_PRESERVE_INPUT);
		p2=fftw_plan_dft_c2r_1d(os+bs,tmp1,tmp2,0);
	}
	void deinit() {
		jackfft_free(buffer);
	}
	template<class F>
	jackfft_float* process(F* data, int sz) {
		memmove(buffer,buffer+sz,(os+bs-sz)*sizeof(jackfft_float));
		jackfft_float* buffer1=buffer+os+bs-sz;
		for(int i=0;i<sz;i++) buffer1[i]=jackfft_float(data[i]);
		fftw_execute(p1);
		for(int i=0;i<(os+bs)/2+1;i++) {
			mult(tmp1[i],filt[i],os+bs);
		}
		fftw_execute(p2);
		return tmp2+os+bs-sz;
	}
	void setWaveform(jackfft_float* data) {
		jackfft_float* tmpbuf=jackfft_alloc<jackfft_float>(os+bs);
		fftw_plan tmpp1=fftw_plan_dft_r2c_1d(os+bs,tmpbuf,filt,0);
		memcpy(tmpbuf,data,os*sizeof(jackfft_float));
		for(int i=os;i<bs+os;i++) tmpbuf[i]=0;
		//tmpbuf[0]=1;
		//tmpbuf[1]=1;
		fftw_execute(tmpp1);
		fftw_destroy_plan(tmpp1);
		jackfft_free(tmpbuf);
		//for(int i=0;i<(os+bs)/2+1;i++) if(filt[i][1]!=0) printf("%i %lf\n",i,filt[i][1]);
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
void load_jfft(int file, double* coeff, int coeffs) {
	struct {
		double freq; double val;
	} buf[1024];
	int i1=0,br;
	double last_v=0.5;
	while((br=read(file,buf,sizeof(buf)))>0) {
		br/=sizeof(*buf);
		for(int x=0;x<br;x++) {
			int i2=(int)round(((double)buf[x].freq/(srate/2))*coeffs);
			if(i2>coeffs) goto break1;
			for(int i=i1;i<i2;i++)
				coeff[i]=last_v*(i2-i)/(i2-i1)+buf[x].val*(i-i1)/(i2-i1);
			i1=i2;
			last_v=buf[x].val;
		}
	}
break1:
	for(int i=i1;i<coeffs;i++) coeff[i]=last_v;
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


convolve* instances;
int channels=2;
vector<jack_port_t*> inputs;
vector<jack_port_t*> outputs;


void error (const char *desc) {
	fprintf (stderr, "JACK error: %s\n", desc);
}
void jack_shutdown (void *arg) {
	exit(3);
}
int process (jack_nframes_t length, void *arg) {
	for(size_t i=0;i<inputs.size();i++) {
		jack_default_audio_sample_t *out =
					(jack_default_audio_sample_t *) jack_port_get_buffer (outputs[i], length);
		jack_default_audio_sample_t *in =
					(jack_default_audio_sample_t *) jack_port_get_buffer (inputs[i], length);
		
		jackfft_float* data=instances[i].process(in,(int)length);
		for(int x=0;x<(int)length;x++) out[x]=data[x];
	}
	return 0;
}
int main(int argc, char** argv) {
	if(argc<2) {
		printf("usage: %s size file.jfft\nor: %s file.pcm\nonly 64-bit float pcm files supported\n",argv[0],argv[0]);
		return 1;
	}
	int os,fd;
	if(argc==2) {
		fd=open(argv[1],O_RDONLY);
		struct stat st;
		if(stat(argv[1],&st)!=0) {
			perror("stat"); return 2;
		}
		os=st.st_size/sizeof(double);
	} else {
		os=atoi(argv[1]);
		fd=open(argv[2],O_RDONLY);
	}
	if(fd<0) {
		perror("open"); return 2;
	}
	jack_client_t *client;
	jack_set_error_function(error);
	JackStatus st;
	if ((client = jack_client_open ("convolve",JackNoStartServer,&st)) == 0) {
		fprintf(stderr, "could not connect to server: status %i\n",st); return 1;
	}
	jack_set_process_callback(client, process, 0);
	jack_on_shutdown(client, jack_shutdown, 0);
	int bs=jack_get_buffer_size(client);
	printf("sample rate: %i\nbuffer size: %i\n", srate=jack_get_sample_rate(client),bs);
	
	jackfft_float* coeff_t=jackfft_alloc<jackfft_float>(os);
	if(argc==2) {
		readAll(fd,coeff_t,os*sizeof(jackfft_float));
	} else {
		double* coeff=jackfft_alloc<double>((os)/2+1);
		jackfft_complex* coeff1=jackfft_alloc<jackfft_complex>((os)/2+1);
		load_jfft(fd,coeff,(os)/2+1);
		close(fd);
		
		
		fftw_plan tmpp1=fftw_plan_dft_c2r_1d(os,coeff1,coeff_t,0);
		for(int i=0;i<(os)/2+1;i++) {
			coeff1[i][0]=coeff[i]/(os);
			coeff1[i][1]=0;
		}
		jackfft_free(coeff);
		fftw_execute(tmpp1);
		fftw_destroy_plan(tmpp1);
		jackfft_free(coeff1);
		for(int i=0;i<os/2;i++) {
			double tmp=coeff_t[i];
			coeff_t[i]=coeff_t[os/2+i];
			coeff_t[os/2+i]=tmp;
		}
	}
	
	
	instances=new convolve[channels];
	string names[channels][2];
	for(int i=0;i<channels;i++)
	{
		names[i][0]="input_"+itoa(i);
		inputs.push_back(jack_port_register(client, names[i][0].c_str(),
						 JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
		names[i][1]="output_"+itoa(i);
		outputs.push_back(jack_port_register(client, names[i][1].c_str(),
						 JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
		instances[i].init(os,bs);
		instances[i].setWaveform(coeff_t);
	}
	
	if (jack_activate(client)) {
		fprintf(stderr, "cannot activate client");
		return 1;
	}
	while(true)sleep(1000);
	jack_client_close(client);
}
