#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fftfilter.C"
#include <cplib/cplib.hpp>

using namespace std;
vector<jack_port_t *> inputs;
vector<jack_port_t *> outputs;
#define CHANNELS 2
#define CONCAT(X) (((stringstream&)(stringstream()<<X)).str())
double cur_index=0;
int srate;

using namespace xaxaxa;
using namespace std;


Filter<jack_default_audio_sample_t>* filt[CHANNELS];

int process (jack_nframes_t length, void *arg)
{
	for(size_t i=0;i<inputs.size();i++)
	{
		jack_default_audio_sample_t *out = 
					(jack_default_audio_sample_t *) 
					jack_port_get_buffer (outputs[i], length);
		jack_default_audio_sample_t *in = 
					(jack_default_audio_sample_t *) 
					jack_port_get_buffer (inputs[i], length);

		filt[i]->PutData(in, length);
		filt[i]->GetData(out, length);
		/*Int a;
		while((a = ((FFTFilter<jack_default_audio_sample_t>*)filt[i])->OutBuffer.BeginDequeue()) >= 0)
		{
			//cout << a << endl;
			((FFTFilter<jack_default_audio_sample_t>*)filt[i])->OutBuffer.EndDequeue(a);
		}*/
		//for(UInt i=0;i<length;i++)
		//	out[i] = in[i];
	}
	return 0;
}
void error (const char *desc)
{
	fprintf (stderr, "JACK error: %s\n", desc);
}
void jack_shutdown (void *arg)
{
	exit (1);
}
inline double scale_freq(double x)
{
	x=pow(2,x*10);
	x=x-1;
	if(x<0)return 0;
	return x/1024;
}
/*
 * y=5x^2
 * 
 * y=((x*2)^2)/4
 * 4y=(2x)^2
 * sqrt(4y)=2x
 * sqrt(4y)/2=x
 * */
inline double scale_freq_r(double x)
{
	//cout << x << " " << log2(x*256) << endl;
	x=x*1024+1;
	//if(x<1.0)return 0;
	x=log2(x)/10;
	//if(::isinf(x)||::isnan(x))return 0;
	//cout << x << endl;
	return x<0?-x:x;
}
double scale_value(double x)
{
	//return x*x;
	double tmp = x - 1.0;
	if(tmp<0)
		return 1.0/(-tmp*19.0+1.0);
	else return tmp*19.0+1.0;
}
/*
 * y=1/(-19x+1)
 * y(-19x+1)=1
 * -19x+1=1/y
 * -19x=1/y-1
 * x=-(1/y-1)/19
 * */
double scale_value_r(double x)
{
	//return x*x;
	//cout << x << endl;
	if(x>1.0)x=(x-1.0)/19.0;
	else x=(-((1/x)-1.0)/19.0);
	
	if(std::isnan(x))return 1.0;
	//else if(x>1.0)return 2.0;
	//else if(x<-1.0)return 0.0;
	else return x+1.0;
}

void load(Stream& fs, double* coeff, UInt coeffs)
{
	try
	{
		//FileStream fs(File(fname.c_str(),O_RDONLY));
		struct
		{
			double freq; double val;
		} buf;
		Buffer b(&buf,sizeof(buf));
		UInt i1=0;
		double last_v=0.5;
		while(fs.Read(b)>=b.Length)
		{
			//cout << buf.freq << endl;
			UInt i2=(UInt)round(((double)buf.freq/(srate/2))*coeffs);
			//cout << i2 << endl;
			if(i2>coeffs)break;
			for(UInt i=i1;i<i2;i++)
				coeff[i]=last_v*(i2-i)/(i2-i1)+buf.val*(i-i1)/(i2-i1);
			i1=i2;
			last_v=buf.val;
		}
		for(UInt i=i1;i<coeffs;i++)
			coeff[i]=scale_value_r(last_v)/2.0;
	}
	catch(Exception& ex)
	{
	}
}

int main (int argc, char *argv[])
{
	if(argc<2)
	{
		cout << "usage: " << argv[0] << " file.jfft" << endl;
		return 0;
	}
	FileStream fs(File(argv[1],O_RDONLY));
	//goto aaaaa;
	//fft=rfftw_create_plan(8192,
	for(UInt i=0;i<CHANNELS;i++)
		filt[i]=new FFTFilter<jack_default_audio_sample_t>(8192, 8, 8, 4);
	
	jack_client_t *client;
	jack_set_error_function (error);
	JackStatus st;
	if ((client = jack_client_open ("jackfft",JackNoStartServer,&st)) == 0) {
		fprintf (stderr, "could not connect to server: status %i\n",st);
		return 1;
	}
	jack_set_process_callback (client, process, 0);
	jack_on_shutdown (client, jack_shutdown, 0);
	printf ("engine sample rate: %u\n", srate=jack_get_sample_rate (client));
	
	FFTFilter<jack_default_audio_sample_t>* tmpf=(FFTFilter<jack_default_audio_sample_t>*)filt[0];
	UInt complexsize = (UInt)(tmpf->BufferSize / 2) + 1;
	load(fs,tmpf->coefficients,complexsize);
	fs.Close();
	for(UInt i=1;i<CHANNELS;i++)
	{
		FFTFilter<jack_default_audio_sample_t>* f=(FFTFilter<jack_default_audio_sample_t>*)filt[i];
		memcpy(f->coefficients,tmpf->coefficients,sizeof(double)*complexsize);
	}
	/* create ports */
	int i;
	for(i=0;i<CHANNELS;i++)
	{
		inputs.push_back(jack_port_register (client, CONCAT("input_"<<i).c_str(), 
						 JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
		outputs.push_back(jack_port_register (client, CONCAT("output_"<<i).c_str(), 
						 JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
	}
	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		return 1;
	}
	while(true)sleep(1000);
	jack_client_close (client);
	return 0;
}
