#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <unistd.h>

using namespace std;
vector<jack_port_t *> inputs;
vector<jack_port_t *> outputs;
#define CHANNELS 2
int srate;
int freq; //in radians/s
double phase=0;
inline double modulus_d(double a, double b)
{
	int tmp=a/b;
	return a-tmp*b;
}
int process (jack_nframes_t length, void *arg)
{
	for(size_t i=0;i<inputs.size();i++)
	{
		jack_default_audio_sample_t *in = (jack_default_audio_sample_t *)
					jack_port_get_buffer (inputs[i], length);
		jack_default_audio_sample_t *out = (jack_default_audio_sample_t *)
					jack_port_get_buffer (outputs[i], length);
		for(int s=0;s<length;s++) {
			double t=double(s)/srate;
			double a=sin(freq*t+phase);
			out[s]=a*in[s];
		}
	}
	phase=modulus_d(phase+double(freq)*length/srate,2*M_PI);
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
		cout << "usage: " << argv[0] << " FREQ" << endl;
		return 0;
	}
	freq=atoi(argv[1]);
	jack_client_t *client;
	jack_set_error_function (error);
	JackStatus st;
	char clientname[64];
	snprintf(clientname,64,"modulator-%iHz",freq);
	freq*=2*M_PI;
	if ((client = jack_client_open (clientname,JackNoStartServer,&st)) == 0) {
		fprintf (stderr, "could not connect to server: status %i\n",st);
		return 1;
	}
	jack_set_process_callback (client, process, 0);
	jack_on_shutdown (client, jack_shutdown, 0);
	printf ("engine sample rate: %u\n", srate=jack_get_sample_rate (client));
	int i;
	for(i=0;i<CHANNELS;i++)
	{
		char* n=(char*)malloc(64);
		snprintf(n,64,"input_%i",i);
		inputs.push_back(jack_port_register (client, n,
						 JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
		n=(char*)malloc(64);
		snprintf(n,64,"output_%i",i);
		outputs.push_back(jack_port_register (client, n,
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
