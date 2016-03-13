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
#define CONCAT(X) (((stringstream&)(stringstream()<<X)).str())
double cur_index=0;
int srate;

using namespace xaxaxa;
using namespace std;


Filter<jack_default_audio_sample_t>** filt;

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
			coeff[i]=last_v;
	}
	catch(Exception& ex)
	{
	}
}

int main (int argc, char *argv[])
{
	if(argc<2)
	{
		cerr << "usage: " << argv[0] << " srate channels file.jfft" << endl;
		return 0;
	}
	FileStream fs(File(argv[1],O_RDONLY));
	//goto aaaaa;
	//fft=rfftw_create_plan(8192,
	for(UInt i=0;i<CHANNELS;i++)
	{
		FFTFilter<jack_default_audio_sample_t>* trololo=new FFTFilter<jack_default_audio_sample_t>
		//bs, inbuffers,	outbuffers,	overlap,buffersperperiod,	paddingL,paddingR,	fftsize
		(1024, 20,			20,			2,		12,					4,		 0,			8192 * 2);

		//trololo->freq_scale=9./10.;
		filt[i]=trololo;
	}
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
	UInt complexsize = (UInt)(tmpf->PeriodSize() / 2) + 1;
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
