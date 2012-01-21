#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fftfilter.C"
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

int main (int argc, char *argv[])
{
	//fft=rfftw_create_plan(8192,
	for(UInt i=0;i<CHANNELS;i++)
		filt[i]=new FFTFilter<jack_default_audio_sample_t>(8192*4, 2);
	
	/*CircularQueue<int> q(2,3);
	auto tmp=q.BeginAppend();
	q.GetPointer(tmp)=123;
	(&q.GetPointer(tmp))[1]=1234;
	(&q.GetPointer(tmp))[2]=12345;
	q.EndAppend(tmp);
	tmp=q.BeginAppend();
	q.GetPointer(tmp)=456;
	(&q.GetPointer(tmp))[1]=4567;
	(&q.GetPointer(tmp))[2]=45678;
	q.EndAppend(tmp);
	while((tmp=q.BeginDequeue())>=0)
	{
		cout << (&q.GetPointer(tmp))[0] << endl;
		cout << (&q.GetPointer(tmp))[1] << endl;
		cout << (&q.GetPointer(tmp))[2] << endl << "-----" << endl;
		q.EndDequeue(tmp);
	}
	tmp=q.BeginAppend();
	q.GetPointer(tmp)=789;
	(&q.GetPointer(tmp))[1]=7890;
	(&q.GetPointer(tmp))[2]=78901;
	q.EndAppend(tmp);
	
	while((tmp=q.BeginDequeue())>=0)
	{
		cout << (&q.GetPointer(tmp))[0] << endl;
		cout << (&q.GetPointer(tmp))[1] << endl;
		cout << (&q.GetPointer(tmp))[2] << endl << "-----" << endl;
		q.EndDequeue(tmp);
	}
	
	return 0;*/
	
	jack_client_t *client;
	jack_set_error_function (error);
	JackStatus st;
	if ((client = jack_client_open ("client1",JackNoStartServer,&st)) == 0) {
		fprintf (stderr, "could not connect to server: status %i\n",st);
		return 1;
	}
	jack_set_process_callback (client, process, 0);
	jack_on_shutdown (client, jack_shutdown, 0);
	printf ("engine sample rate: %u\n", srate=jack_get_sample_rate (client));
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
	for(;;)
	  sleep (1000);

	jack_client_close (client);
	exit (0);
}
