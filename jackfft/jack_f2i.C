#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cpoll/cpoll.H>

using namespace std;
vector<jack_port_t *> inputs;
vector<jack_port_t *> outputs;
#define CHANNELS 4
#define CONCAT(X) (((stringstream&)(stringstream()<<X)).str())
double cur_index=0;
int srate;
#define JACK_FLOAT_MAX 1.0 //e+50
#define INT_MAX 2147483647
#define INT_MIN -2147483648
int process (jack_nframes_t nframes, void *arg)
{
	for(size_t i=0;i<inputs.size();i++)
	{
		jack_default_audio_sample_t *out = 
					(jack_default_audio_sample_t *) 
					jack_port_get_buffer (outputs[i], nframes);
		jack_default_audio_sample_t *in = 
					(jack_default_audio_sample_t *) 
					jack_port_get_buffer (inputs[i], nframes);

		//memcpy (out, in, sizeof (jack_default_audio_sample_t) * nframes);
		for(jack_nframes_t ii=0;ii<nframes;ii++)
		{
			//cout << in[ii] << endl;
			//out[ii]=in[ii]/16.0;
			// ******FLOAT TO INT CONVERSION******
			int* out_int=(int*)&out[ii];
			double tmp=in[ii]*INT_MAX/JACK_FLOAT_MAX;
			if(tmp>=(float)INT_MAX)
				*out_int=INT_MAX;
			else if(tmp<=(float)INT_MIN)
				*out_int=INT_MIN;
			else *out_int=(int)tmp;
		}
		/*double freq=80;
		for(jack_nframes_t ii=0;ii<nframes;ii++)
		{
			out[ii]=sin(cur_index);
			//cout << out[ii] << endl;
			//out[ii]*=0x0.1p-1070;//1000000000000000000;
			
			cur_index+=1.0/srate*freq*(2*M_PI);
		}*/
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
        /*KIRBY: Create a JACK client.  
         This is our connection to the JACK daemon.*/
	//cout << 0x0.09p-1070 << endl;
	jack_client_t *client;

	
	/* tell the JACK server to call error() whenever it
	   experiences an error.  Notice that this callback is
	   global to this process, not specific to each client.
	
	   This is set here so that it can catch errors in the
	   connection process
	*/
	/*KIRBY: Nuff said.*/
	jack_set_error_function (error);

	/* try to become a client of the JACK server */
	/*KIRBY:  This is where our pointer "client" 
          gets something to point to.  
	  You will notice that the functions called later take a client as 
	  a parameter - this is what we pass.*/
	JackStatus st;
	if ((client = jack_client_open ("float2int",JackNoStartServer,&st)) == 0) {
		fprintf (stderr, "could not connect to server: status %i\n",st);
		return 1;
	}

	/* tell the JACK server to call `process()' whenever
	   there is work to be done.
	*/

	jack_set_process_callback (client, process, 0);

	/* tell the JACK server to call `srate()' whenever
	   the sample rate of the system changes.
	*/

	//jack_set_sample_rate_callback (client, srate, 0);

	/* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, jack_shutdown, 0);

	/* display the current sample rate. once the client is activated 
	   (see below), you should rely on your own sample rate
	   callback (see above) for this value.
	*/
	printf ("engine sample rate: %u\n", srate=jack_get_sample_rate (client));
	CP::StringPool sp;
	/* create ports */
	int i;
	for(i=0;i<CHANNELS;i++)
	{
		CP::MemoryStream ss;
		CP::StreamWriter sw(ss);
		sw.writeF("input_%i", i); sw.flush();
		inputs.push_back(jack_port_register (client, sp.addString(ss.String()).data(), 
						 JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
		ss.clear();
		sw.writeF("output_%i", i); sw.flush();
		outputs.push_back(jack_port_register (client, sp.addString(ss.String()).data(), 
						 JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
	}
	/* tell the JACK server that we are ready to roll */
	/*KIRBY: So, once we are in a position to start 
          doing whatever it is we do, this is how we announce that.*/
	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		return 1;
	}

	

	/* Since this is just a toy, run for a few seconds, then finish */
	/*KIRBY: We changed that, too.  Now we run until we get killed.*/
	for(;;)
	  sleep (1000);

	/*KIRBY: Close the client*/
	jack_client_close (client);
	exit (0);
}
