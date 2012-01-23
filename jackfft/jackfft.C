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
//UInt pb_size=0;
//Gtk::ProgressBar* pb;
//UInt freqs = 50;
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
#define EQ_POINTS 1500
EQControl* c;
Glib::RefPtr<Gtk::Builder> b;
string fname=".default.jfft";

void update_fft()
{
	for(UInt i=0;i<CHANNELS;i++)
	{
		auto f=(FFTFilter<jack_default_audio_sample_t>*)(filt[i]);
		UInt complexsize = (UInt)(f->BufferSize / 2) + 1;
		for(UInt n=0;n<complexsize;n++)
			f->coefficients[n]=scale_value(c->GetPoint(scale_freq_r((double)n/complexsize)*EQ_POINTS)*2.0);
	}
}
void on_change(void* user, UInt i1, UInt i2)
{
	EQControl* c=(EQControl*)user;
	for(UInt i=0;i<CHANNELS;i++)
	{
		auto f=(FFTFilter<jack_default_audio_sample_t>*)(filt[i]);
		UInt complexsize = (UInt)(f->BufferSize / 2) + 1;
		//complexsize /= 5;
		UInt min_index=floor(scale_freq((double)i1/EQ_POINTS)*(double)complexsize);
		UInt max_index=ceil(scale_freq((double)i2/EQ_POINTS)*(double)complexsize);
		//cout << "i1="<<i1<<"; i2="<<i2<<"; scale_freq="<<scale_freq((double)i1/EQ_POINTS)<<endl;
		//cout << "min_index="<<min_index<<"; max_index="<<max_index<<endl;
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
void on_mousemove(void* user, double i)
{
	EQControl* c=(EQControl*)user;
	double freq=scale_freq(i/c->datalen)*srate/2.0;
	Gtk::Label* l;
	b->get_widget("label1",l);
	l->set_text(CONCAT((UInt)freq<<" Hz").c_str());
}

void save()
{
	try
	{
		FileStream fs(File(fname.c_str(),O_CREAT|O_WRONLY|O_TRUNC,0666));
		struct
		{
			double freq; double val;
		} buf;
		Buffer b(&buf,sizeof(buf));
		for(UInt i=0;i<EQ_POINTS;i++)
		{
			buf.freq=scale_freq((double)i/EQ_POINTS)*srate/2;
			buf.val=scale_value(c->GetPoint(i)*2.0);
			//cout << buf.val << endl;
			fs.Write(b);
		}
	}
	catch(Exception& ex)
	{
	}
}
void load()
{
	try
	{
		FileStream fs(File(fname.c_str(),O_RDONLY));
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
			UInt i2=(UInt)round(scale_freq_r((double)buf.freq/(srate/2))*EQ_POINTS);
			//cout << i2 << endl;
			if(i2>EQ_POINTS)break;
			for(UInt i=i1;i<i2;i++)
				c->data[i]=scale_value_r(last_v*(i2-i)/(i2-i1)+buf.val*(i-i1)/(i2-i1))/2.0;
			i1=i2;
			last_v=buf.val;
		}
		for(UInt i=i1;i<EQ_POINTS;i++)
			c->data[i]=scale_value_r(last_v)/2.0;
		c->queue_draw();
		update_fft();
	}
	catch(Exception& ex)
	{
	}
}

void saveas()
{
	Gtk::FileChooserDialog* d;
	b->get_widget("filechooserdialog1",d);
	Gtk::Window* w;
	b->get_widget("window1",w);
	cout << w << endl;
	d->set_transient_for(*w);
	if(d->run()==RESPONSE_OK)
	{
		
	}
	d->hide();
}
int main (int argc, char *argv[])
{
	//goto aaaaa;
	//fft=rfftw_create_plan(8192,
	for(UInt i=0;i<CHANNELS;i++)
		filt[i]=new FFTFilter<jack_default_audio_sample_t>(8192, 8, 8, 4);
	
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
	if ((client = jack_client_open ("jackfft",JackNoStartServer,&st)) == 0) {
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
	
	b = Gtk::Builder::create_from_file("a.glade");
	Gtk::Window* w;
	//Gtk::Grid* g;
	//Gtk::EventBox* eb;
	Gtk::Viewport* v;
	Gtk::Button* bt;
	b->get_widget("window1",w);
	b->get_widget("viewport1",v);
	c=new EQControl(EQ_POINTS);
	c->Change+=EQControl::ChangeDelegate(&on_change,c);
	c->MouseMove+=EQControl::MouseDelegate(&on_mousemove,c);
	
	b->get_widget("b_save",bt);
	bt->signal_clicked().connect(&save);
	b->get_widget("b_saveas",bt);
	bt->signal_clicked().connect(&saveas);
	b->get_widget("b_open",bt);
	bt->signal_clicked().connect(&load);
	
	Gtk::FileChooserDialog* d;
	b->get_widget("filechooserdialog1",d);
	//d->signal_response().connect(&on_response);
	d->add_button(Stock::CANCEL,RESPONSE_CANCEL);
	d->add_button(Stock::OPEN,RESPONSE_OK);
	
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
	load();
	kit.run(*w);
	
	jack_client_close (client);
	return 0;
}
