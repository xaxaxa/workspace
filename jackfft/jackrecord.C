#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
#define CHANNELS 2
int srate;
jack_port_t* inputs[CHANNELS];
int process (jack_nframes_t length, void *arg)
{
	int c=CHANNELS;
	int16_t buf[c*length];
	for(size_t i=0;i<c;i++)
	{
		jack_default_audio_sample_t *in = (jack_default_audio_sample_t *)
					jack_port_get_buffer (inputs[i], length);
		for(int s=0;s<length;s++) {
			buf[s*c+i]=(int16_t)(in[s]*0x7FFF);
		}
	}
	write(1,buf,c*length*2);
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
	int f = fcntl(1, F_GETFL);
	fcntl(1, F_SETFL, f | O_NONBLOCK);
	jack_client_t* client;
	jack_set_error_function (error);
	JackStatus st;
	if ((client = jack_client_open ("jackrecord",JackNoStartServer,&st)) == 0) {
		fprintf(stderr, "could not connect to server: status %i\n",st);
		return 1;
	}
	jack_set_process_callback (client, process, 0);
	jack_on_shutdown (client, jack_shutdown, 0);
	fprintf(stderr,"engine sample rate: %u\n", srate=jack_get_sample_rate (client));
	for(int i=0;i<CHANNELS;i++) {
		char* n=(char*)malloc(64);
		snprintf(n,64,"input_%i",i);
		inputs[i]=(jack_port_register(client, n, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
	}
	if (jack_activate (client)) {
		fprintf(stderr, "cannot activate client");
		return 1;
	}
	while(true)sleep(1000);
	jack_client_close (client);
	return 0;
}
