#include <iostream>
#include <jack/jack.h>
#include <memory.h>
#include <vector>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fftfilter.C"
#include <gtkmm.h>
#include <gdk/gdk.h>
#include "eqcontrol.C"

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
UInt pb_size=0;
Gtk::ProgressBar* pb;
UInt freqs = 50;
double scale_freq(double x)
{
	return x*x;
}
double scale_freq_r(double x)
{
	return sqrt(x);
}
double scale_value(double x)
{
	//return x*x;
	double tmp = x - 1.0;
	if(tmp<0)
		return 1.0/(-tmp*19.0+1.0);
	else return tmp*20.0;
}
bool on_motion_notify(GdkEventMotion* evt)
{
	//cout << "on_motion_notify" << endl;
	if(pb_size<=0)pb_size=pb->get_allocation().get_width();
	UInt ind=evt->x/pb_size;
	//cout << pb_size << endl;
	if(ind>=freqs)return true;
	Int h=gdk_window_get_height(evt->window);
	pb[ind].set_fraction((float)(h-evt->y)/h);
	for(UInt i=0;i<CHANNELS;i++)
	{
		auto f=(FFTFilter<jack_default_audio_sample_t>*)(filt[i]);
		UInt complexsize = (UInt)(f->BufferSize / 2) + 1;
		//complexsize /= 5;
		UInt min_index=scale_freq((double)ind/freqs)*complexsize;
		UInt max_index=scale_freq((double)(ind+1)/freqs)*complexsize;
		//cout << "min_index="<<min_index<<endl;
		//cout << "max_index="<<max_index<<endl;
		for(UInt n=min_index;n<max_index;n++)
		{
			f->coefficients[n]=scale_value((double)(h-evt->y)/h*2);
		}
	}
	return true;
}
#define EQ_POINTS 300
void on_change(void* user, UInt i1, UInt i2)
{
	EQControl* c=(EQControl*)user;
	for(UInt i=0;i<CHANNELS;i++)
	{
		auto f=(FFTFilter<jack_default_audio_sample_t>*)(filt[i]);
		UInt complexsize = (UInt)(f->BufferSize / 2) + 1;
		//complexsize /= 5;
		UInt min_index=scale_freq((double)i1/EQ_POINTS)*complexsize;
		UInt max_index=scale_freq((double)i2/EQ_POINTS)*complexsize;
		//cout << "min_index="<<min_index<<endl;
		//cout << "max_index="<<max_index<<endl;
		UInt max_n=floor(max_index);
		if(max_n>complexsize)max_n=complexsize;
		for(UInt n=min_index;n<max_n;n++)
		{
			double tmp=(c->GetPoint(scale_freq_r((double)n/complexsize)*EQ_POINTS)*2.0);
			//cout << c->GetPoint(scale_freq_r((double)n/complexsize)*EQ_POINTS) << endl;
			tmp=scale_value(tmp);
			//if(tmp1>4)cout << tmp1 << " " << tmp << endl;
			f->coefficients[n]=tmp;
		}
	}
}
int main (int argc, char *argv[])
{
	//goto aaaaa;
	//fft=rfftw_create_plan(8192,
	for(UInt i=0;i<CHANNELS;i++)
		filt[i]=new FFTFilter<jack_default_audio_sample_t>(8192*2, 8, 8, 4);
	
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
//aaaaa:
	Gtk::Main kit(argc, argv);
	Glib::RefPtr<Gtk::Builder> b;
	b = Gtk::Builder::create_from_file("a.glade");
	Gtk::Window* w;
	//Gtk::Grid* g;
	//Gtk::EventBox* eb;
	Gtk::Viewport* v;
	b->get_widget("window1",w);
	b->get_widget("viewport1",v);
	EQControl* c=new EQControl(EQ_POINTS);
	c->Change+=EQControl::ChangeDelegate(&on_change,c);
	v->add(*c);
	c->set_hexpand(true);
	c->set_vexpand(true);
	c->show();
	//b->get_widget("grid1",g);
	//b->get_widget("eventbox1",eb);
	
	
	/*eb->signal_motion_notify_event().connect(sigc::ptr_fun(&on_motion_notify));
	pb=new Gtk::ProgressBar[freqs];
	for(UInt i=0;i<freqs;i++)
	{
		Gtk::ProgressBar* p=pb+i;
		//pb=new Gtk::ProgressBar();
		//p->set_text(CONCAT((i*srate/2/freqs)).c_str());
		//p->set_show_text(true);
		p->set_orientation(Gtk::ORIENTATION_VERTICAL);
		p->set_inverted(true);
		p->set_vexpand(true);
		p->set_size_request(10,-1);
		p->set_fraction(0.5);
		g->attach(*p,i,0,1,1);
		p->show();
		//if(i==0)pb_size=p->get_allocation().get_width();
	}*/
	
	kit.run(*w);
	
	jack_client_close (client);
	return 0;
}
