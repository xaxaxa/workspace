#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <fstream>
#include <unistd.h>

#include "fftfilter.C"
#include "jfftfile.H"

using namespace std;
bool jackfftPrintWarnings=true;
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

		filt[i]->Process(in, out, length);
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

int main (int argc, char *argv[])
{
	if(argc<2)
	{
		cout << "usage: " << argv[0] << " file.jfft" << endl;
		return 0;
	}
	ifstream jfftStream(argv[1]);
	stringstream jfftBuffer;
	jfftBuffer << jfftStream.rdbuf();
	string jfft = jfftBuffer.str();
	
	for(unsigned i=0;i<CHANNELS;i++)
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
	unsigned complexsize = (unsigned)(tmpf->PeriodSize() / 2) + 1;
	loadjfft((uint8_t*)jfft.data(), jfft.length(), tmpf->coefficients,complexsize, srate);
	for(unsigned i=1;i<CHANNELS;i++)
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
