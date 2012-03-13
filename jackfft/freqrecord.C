/*
 * freqrecord.C
 * 
 * Copyright 2012  <xaxaxa@xaxaxa-mac>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#define cplib_glib_wrappers
#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fftfilter.C"
#include <gtkmm.h>
#include <gdk/gdk.h>
#include "eqcontrol.C"
#include <cplib/cplib.hpp>
#include <math.h>

using namespace xaxaxa;
using namespace std;
vector<jack_port_t *> inputs;
//vector<jack_port_t *> outputs;
#define CHANNELS 1
#define CONCAT(X) (((stringstream&)(stringstream()<<X)).str())
int srate;

//FFTTransform<jack_default_audio_sample_t> t(1024,20,20,16,16);
vector<FFTStream_f*> ffts;
vector<FilterBuffer<double>* > buffers;
StandardStream ss;
int process (jack_nframes_t length, void *arg)
{
	double* tmp=new double[length];
	for(size_t i=0;i<inputs.size();i++)
	{
		/*jack_default_audio_sample_t *out = 
					(jack_default_audio_sample_t *) 
					jack_port_get_buffer (outputs[i], length);*/
		jack_default_audio_sample_t *in = 
					(jack_default_audio_sample_t *) 
					jack_port_get_buffer (inputs[i], length);
		
		for(jack_nframes_t x=0;x<length;x++)
			tmp[x]=in[x];
		FilterBuffer<double>::AppendData append(*buffers[i],tmp,length);
		double* d;
		if((d=append()))
		{
			d=ffts[i]->Process();
			cerr << "Process()" << endl;
			ss.Write(Buffer((void*)d,sizeof(double)*ffts[i]->size_c));
			//cout << endl;
		}
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
int main()
{
	jack_client_t *client;
	jack_set_error_function (error);
	JackStatus st;
	if ((client = jack_client_open ("freqrecord",JackNoStartServer,&st)) == 0) {
		fprintf (stderr, "could not connect to server: status %i\n",st);
		return 1;
	}
	jack_set_process_callback (client, process, 0);
	jack_on_shutdown (client, jack_shutdown, 0);
	cerr<<"engine sample rate: "<<(srate=jack_get_sample_rate (client))<<endl;
	/* create ports */
	int i;
	for(i=0;i<CHANNELS;i++)
	{
		inputs.push_back(jack_port_register (client, CONCAT("input_"<<i).c_str(), 
						 JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
		//outputs.push_back(jack_port_register (client, CONCAT("output_"<<i).c_str(), 
		//				 JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
		ffts.push_back(new FFTStream_f(8192));
		buffers.push_back(new FilterBuffer<double>(ffts[i]->buffer,ffts[i]->size));
	}
	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		return 1;
	}
	while(true)sleep(1000);
	jack_client_close (client);
	return 0;
}

