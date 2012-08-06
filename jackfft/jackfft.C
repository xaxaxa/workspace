/*
 * jackfft.C
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
#define WARNLEVEL 3
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

using namespace std;



vector<jack_port_t *> inputs;
vector<jack_port_t *> outputs;
#define CHANNELS 2
#define CONCAT(X) (((stringstream&)(stringstream()<<X)).str())
double cur_index = 0;
int srate;
bool display_spectrum = false;

using namespace xaxaxa;
using namespace std;

File efd(eventfd(0,0));
FFTFilter<jack_default_audio_sample_t>* filt[CHANNELS];
FFTFilter<jack_default_audio_sample_t>** filt2 = NULL;
struct timespec last_refreshed;
#define EQ_POINTS 1500
EQControl* c;
EQControl* c2;
Glib::RefPtr<Gtk::Builder> b;
string fname = ".default.jfft";


inline double scale_freq(double x)
{
	x = pow(2, x * 10);
	x = x - 1;
	if(x < 0)return 0;
	return x / 1024;
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
	x = x * 1024 + 1;
	//if(x<1.0)return 0;
	x = log2(x) / 10;
	//if(::isinf(x)||::isnan(x))return 0;
	//cout << x << endl;
	return x < 0 ? -x : x;
}
inline double logn(double n, double x)
{
	return log(x) / log(n);
}

//graph to coefficient
//x: [0.0,2.0]
inline double scale_value(double x)
{
	//return x*x;
	double tmp = x - 1.;
	tmp *= 5.;
	/*if(tmp<0)
		return 1.0/(-tmp*19.0+1.0);
	else return tmp*19.0+1.0;*/
	return pow(2, tmp);
}
/*
 * y=1/(-19x+1)
 * y(-19x+1)=1
 * -19x+1=1/y
 * -19x=1/y-1
 * x=-(1/y-1)/19
 * */
inline double scale_value_r(double x)
{
	//return x*x;
	//cout << x << endl;
	return logn(2, x) / 5. + 1.;

	/*if(std::isnan(x))return 1.0;
	//else if(x>1.0)return 2.0;
	//else if(x<-1.0)return 0.0;
	else return x+1.0;*/
}

inline Long get_nsec(const timespec& t)
{
	return ((Long)t.tv_sec) * 1000000000 + (Long)t.tv_nsec;
}
template<class t> inline double complex_to_real(const t& x)
{
	auto r = x[0];
	auto i = x[1];
	//return r;
	return sqrt(pow(r, 2) + pow(i, 2));
}
int process(jack_nframes_t length, void *arg)
{
	jack_default_audio_sample_t* in_samples[inputs.size()];
	jack_default_audio_sample_t* out_samples[inputs.size()];
	for(size_t i = 0; i < inputs.size(); i++)
	{
		in_samples[i]=(jack_default_audio_sample_t *)
			jack_port_get_buffer(inputs[i], length);
		out_samples[i]=(jack_default_audio_sample_t *)
			jack_port_get_buffer(outputs[i], length);
	}
	for(size_t i = 0; i < inputs.size(); i++)
	{
		jack_default_audio_sample_t *out = out_samples[i];
		jack_default_audio_sample_t *in = in_samples[i];

		if(filt2 != NULL && filt2[i] != NULL)
		{
			filt2[i]->PutData(in, length);
			if(filt2[i]->GetData(out, length) > 0)
			{
				delete filt[i];
				filt[i] = NULL;
				continue;
			}
		}
		filt[i]->Process(in, out, length);
		/*Int a;
		while((a = ((FFTFilter<jack_default_audio_sample_t>*)filt[i])->OutBuffer.BeginDequeue()) >= 0)
		{
			//cout << a << endl;
			((FFTFilter<jack_default_audio_sample_t>*)filt[i])->OutBuffer.EndDequeue(a);
		}*/
		//for(UInt i=0;i<length;i++)
		//	out[i] = in[i];
	}
	/*for(size_t i=0; i<length; i++)
	{
		double avg=0;
		for(register unsigned int ii=0;ii<CHANNELS;ii++)
			avg+=((double)out_samples[ii][i])/CHANNELS;
		for(register unsigned int ii=0;ii<CHANNELS;ii++)
			out_samples[ii][i]=avg+(out_samples[ii][i]-avg)*3;
	}*/
	for(size_t i = 0; i < inputs.size(); i++)
	{
		if(filt[i] != NULL)goto asdfghjkl;
	}
	for(size_t i = 0; i < inputs.size(); i++)
	{
		filt[i] = filt2[i];
	}
	delete[] filt2;
	filt2 = NULL;
asdfghjkl:
	FFTFilter<jack_default_audio_sample_t>* trololo = ((FFTFilter<jack_default_audio_sample_t>*)filt[0]);
	if(display_spectrum && trololo->didprocess)
	{
		trololo->didprocess = false;
		Long tmp_i=1;
		efd.Write(BufferRef(&tmp_i,sizeof(tmp_i)));
	}
	return 0;
}
void* thread1(void* v)
{
	while(true)
	{
		Long tmp;
		efd.Read(BufferRef(&tmp,sizeof(tmp)));
		WARN(8,tmp);
		struct timespec t;
		clock_gettime(CLOCK_MONOTONIC, &t);
		if(get_nsec(last_refreshed) + (60 * 1000000) < get_nsec(t))
		{
			last_refreshed = t;
			//refresh
			for(decltype(c2->datalen) i = 0; i < c2->datalen; i++)
				c2->data[i] = 0;
			for(size_t ii = 0; ii < inputs.size(); ii++)
			{
				if(filt[ii] == NULL)continue;
				UInt complexsize = ((FFTFilter<jack_default_audio_sample_t>*)filt[ii])->ComplexSize();
				for(decltype(c2->datalen) i = 0; i < c2->datalen; i++)
				{
					UInt complex_index = scale_freq((double)i / (c2->datalen - 1)) * (complexsize - 1);
					c2->data[i] += (complex_to_real(((FFTFilter<jack_default_audio_sample_t>*)filt[ii])->tmpcomplex[complex_index])) / 100 / inputs.size();
				}
			}
			gdk_threads_enter();
			c2->do_draw();
			gdk_threads_leave();
		}
	}
	return NULL;
}
void error(const char *desc)
{
	fprintf(stderr, "JACK error: %s\n", desc);
}
void jack_shutdown(void *arg)
{
	exit(1);
}
//UInt pb_size=0;
//Gtk::ProgressBar* pb;
//UInt freqs = 50;

void update_fft(FFTFilter<jack_default_audio_sample_t>** filt2 = NULL)
{
	if(filt2 != NULL)
	{
		for(UInt i = 0; i < CHANNELS; i++)
		{
			if(filt2[i] == NULL)continue;
			auto f = filt2[i];
			UInt complexsize = f->ComplexSize();
			for(UInt n = 0; n < complexsize; n++)
				f->coefficients[n] = scale_value(c->GetPoint(scale_freq_r((double)n / complexsize) * EQ_POINTS) * 2.0);
		}
		return;
	}
	for(UInt i = 0; i < CHANNELS; i++)
	{
		auto f = (FFTFilter<jack_default_audio_sample_t>*)(filt[i]);
		UInt complexsize = f->ComplexSize();
		for(UInt n = 0; n < complexsize; n++)
			f->coefficients[n] = scale_value(c->GetPoint(scale_freq_r((double)n / complexsize) * EQ_POINTS) * 2.0);
	}
}
void on_change(void* user, UInt i1, UInt i2)
{
	EQControl* c = (EQControl*)user;
	for(UInt i = 0; i < CHANNELS; i++)
	{
		auto f = (FFTFilter<jack_default_audio_sample_t>*)(filt[i]);
		UInt complexsize = f->ComplexSize();
		//complexsize /= 5;
		UInt min_index = floor(scale_freq((double)i1 / EQ_POINTS) * (double)complexsize);
		UInt max_index = ceil(scale_freq((double)i2 / EQ_POINTS) * (double)complexsize);
		//cout << "i1="<<i1<<"; i2="<<i2<<"; scale_freq="<<scale_freq((double)i1/EQ_POINTS)<<endl;
		//cout << "min_index="<<min_index<<"; max_index="<<max_index<<endl;
		UInt max_n = floor(max_index);
		if(max_n > complexsize)max_n = complexsize;
		for(UInt n = min_index; n < max_n; n++)
		{
			double tmp = (c->GetPoint(scale_freq_r((double)n / complexsize) * EQ_POINTS) * 2.0);
			//cout << c->GetPoint(scale_freq_r((double)n/complexsize)*EQ_POINTS) << endl;
			tmp = scale_value(tmp);
			//if(tmp1>4)cout << tmp1 << " " << tmp << endl;
			f->coefficients[n] = tmp;
		}
	}
}
void on_mousemove(void* user, double i)
{
	EQControl* c = (EQControl*)user;
	double freq = scale_freq(i / c->datalen) * srate / 2.0;
	Gtk::Label* l;
	b->get_widget("label1", l);
	l->set_text(CONCAT((UInt)freq << " Hz").c_str());
}

void do_save(Stream& fs)
{
	try
	{
		struct
		{
			double freq;
			double val;
		} buf;
		Buffer b(&buf, sizeof(buf));
		for(UInt i = 0; i < EQ_POINTS; i++)
		{
			buf.freq = scale_freq((double)i / EQ_POINTS) * srate / 2;
			buf.val = scale_value(c->GetPoint(i) * 2.0);
			//cout << buf.val << endl;
			fs.Write(b);
		}
	}
	catch(Exception& ex)
	{
	}
}
void saveas();
void save()
{
	if(fname.length() == 0)
	{
		saveas();
		return;
	}
	FileStream fs(File(fname.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666));
	do_save(fs);
	fs.Close();
}

void do_load(Stream& fs)
{
	try
	{
		//FileStream fs(File(fname.c_str(),O_RDONLY));
		struct
		{
			double freq;
			double val;
		} buf;
		Buffer b(&buf, sizeof(buf));
		UInt i1 = 0;
		double last_v = 0.5;
		while(fs.Read(b) >= b.Length)
		{
			//cout << buf.freq << endl;
			UInt i2 = (UInt)round(scale_freq_r((double)buf.freq / (srate / 2)) * EQ_POINTS);
			//cout << i2 << endl;
			if(i2 > EQ_POINTS)break;
			for(UInt i = i1; i < i2; i++)
				c->data[i] = scale_value_r(last_v * (i2 - i) / (i2 - i1) + buf.val * (i - i1) / (i2 - i1)) / 2.0;
			i1 = i2;
			last_v = buf.val;
		}
		for(UInt i = i1; i < EQ_POINTS; i++)
			c->data[i] = scale_value_r(last_v) / 2.0;
		c->queue_draw();
		update_fft();
	}
	catch(Exception& ex) {}
	catch(exception& ex) {}
	catch(Glib::Exception& ex) {}
}

void loadfile();
void load()
{
	try {
		if(fname.length() == 0)
		{
			loadfile();
			return;
		}
		FileStream fs(File(fname.c_str(), O_RDONLY));
		do_load(fs);
		fs.Close();
	} catch(Exception& ex) {}
	catch(exception& ex) {}
	catch(Glib::FileError& ex) {}
}

void saveas()
{
	Gtk::FileChooserDialog* d;
	b->get_widget("filechooserdialog1", d);
	Gtk::Window* w;
	b->get_widget("window1", w);
	d->set_transient_for(*w);
	d->set_action(FILE_CHOOSER_ACTION_SAVE);
	Gtk::Button* b1 = d->add_button(Stock::SAVE, RESPONSE_OK);
	if(d->run() == RESPONSE_OK)
	{
		GIOGenericStream s = Glib::RefPtr<Gio::OutputStream>::cast_dynamic(d->get_file()->replace());
		do_save(s);
		s.Close();
		fname = d->get_filename();
		if(fname.length() <= 0)
		{
			WARN(1, "current file name could not be updated because the file path is not obtainable(not a local file?)");
		}
	}
	d->hide();
	b1->get_parent()->Gtk::Container::remove(*b1);
	delete b1;
}
void loadfile()
{
	Gtk::FileChooserDialog* d;
	b->get_widget("filechooserdialog1", d);
	Gtk::Window* w;
	b->get_widget("window1", w);
	d->set_transient_for(*w);
	d->set_action(FILE_CHOOSER_ACTION_OPEN);
	Gtk::Button* b = d->add_button(Stock::OPEN, RESPONSE_OK);
	if(d->run() == RESPONSE_OK)
	{
		GIOGenericStream s = Glib::RefPtr<Gio::InputStream>::cast_dynamic(d->get_file()->read());
		do_load(s);
		s.Close();
		fname = d->get_filename();
		if(fname.length() <= 0)
		{
			WARN(1, "current file name could not be updated because the file path is not obtainable(not a local file?)");
		}
	}
	d->hide();
	b->get_parent()->Gtk::Container::remove(*b);
	delete b;
}
void apply_pitchshift1(FFTFilter<jack_default_audio_sample_t>** filt2)
{
	double asdf = 1.;
	Gtk::CheckButton* cb;
	Gtk::Entry* e;
	b->get_widget("c_pitchshift", cb);
	if(cb->get_active())
	{
		b->get_widget("t_pitch1", e);
		asdf = strtod(e->get_text().c_str(), NULL);
		b->get_widget("t_pitch2", e);
		asdf /= strtod(e->get_text().c_str(), NULL);
	}
	if(filt2 != NULL)
	{
		for(size_t i = 0; i < inputs.size(); i++)
		{
			if(filt2[i] == NULL)continue;
			filt2[i]->freq_scale = asdf;
		}
		return;
	}
	for(size_t i = 0; i < inputs.size(); i++)
		((FFTFilter<jack_default_audio_sample_t>*)filt[i])->freq_scale = asdf;
}
void apply_pitchshift()
{
	apply_pitchshift1(NULL);
}
void apply_label_workaround(Gtk::Label* l)
{
	l->signal_size_allocate().connect([l](Allocation & a)
	{
		l->set_size_request(a.get_width() - 5, -1);
		//l->set_size_request(-1,-1);
	});
}
int main(int argc, char *argv[])
{
	memset(&last_refreshed, 0, sizeof(last_refreshed));
	for(int i = 1; i < argc; i++)
	{
		string tmp(argv[i]);
		if(tmp == "-s")display_spectrum = true;
	}
	Util.ChDir(Util.GetDirFromPath(Util.GetProgramPath()));
	//goto aaaaa;
	//fft=rfftw_create_plan(8192,
	for(UInt i = 0; i < CHANNELS; i++)
	{
		FFTFilter<jack_default_audio_sample_t>* trololo = new FFTFilter<jack_default_audio_sample_t>
		//bs, inbuffers,	outbuffers,	overlap,buffersperperiod,	padding,	fftsize
		(1024, 16,			16,			2,		12,					2,			8192 * 2);

		//trololo->freq_scale=9./10.;
		filt[i] = trololo;
	}
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
	jack_set_error_function(error);
	JackStatus st;
	if((client = jack_client_open("jackfft", JackNoStartServer, &st)) == 0) {
		fprintf(stderr, "could not connect to server: status %i\n", st);
		return 1;
	}
	jack_set_process_callback(client, process, 0);
	jack_on_shutdown(client, jack_shutdown, 0);
	printf("engine sample rate: %u\n", srate = jack_get_sample_rate(client));
	/* create ports */
	int i;
	for(i = 0; i < CHANNELS; i++)
	{
		inputs.push_back(jack_port_register(client, CONCAT("input_" << i).c_str(),
											JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
		outputs.push_back(jack_port_register(client, CONCAT("output_" << i).c_str(),
											 JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
	}

//aaaaa:
	Gtk::Main kit(argc, argv);

	b = Gtk::Builder::create_from_file("main2.ui");
	Gtk::Window* w;
	//Gtk::Grid* g;
	//Gtk::EventBox* eb;

	Gtk::Button* bt;
	b->get_widget("window1", w);
	c = new EQControl(EQ_POINTS);
	c->Change += EQControl::ChangeDelegate(&on_change, c);
	c->MouseMove += EQControl::MouseDelegate(&on_mousemove, c);

	//UInt complexsize = (UInt)(((FFTFilter<jack_default_audio_sample_t>*)filt[0])->PeriodSize() / 2) + 1;


	b->get_widget("b_save", bt);
	bt->signal_clicked().connect(&save);
	b->get_widget("b_saveas", bt);
	bt->signal_clicked().connect(&saveas);
	b->get_widget("b_open", bt);
	bt->signal_clicked().connect(&loadfile);
	b->get_widget("b_revert", bt);
	bt->signal_clicked().connect(&load);
	b->get_widget("b_apply", bt);
	bt->signal_clicked().connect(&apply_pitchshift);
	Gtk::CheckButton* cb;
	b->get_widget("c_pitchshift", cb);
	cb->signal_toggled().connect(&apply_pitchshift);


	Gtk::Label* lb;
	b->get_widget("label8", lb);
	apply_label_workaround(lb);

	b->get_widget("b_params", bt);
	bt->signal_clicked().connect([]()
	{
		Gtk::Window* w1;
		Gtk::Dialog* d;
		b->get_widget("window1", w1);
		b->get_widget("dialog1", d);
		d->set_transient_for(*w1);
		if(d->run() != RESPONSE_APPLY)goto hhhhh;

		FFTFilter<jack_default_audio_sample_t>** tmp;
		tmp = new FFTFilter<jack_default_audio_sample_t>*[CHANNELS];
		Int bs, overlap, bpp, padding, fftsize;
		Gtk::Entry* ent;
		b->get_widget("t_bs", ent);
		bs = atoi(ent->get_text().c_str());
		b->get_widget("t_overlap", ent);
		overlap = atoi(ent->get_text().c_str());
		b->get_widget("t_bpp", ent);
		bpp = atoi(ent->get_text().c_str());
		b->get_widget("t_padding", ent);
		padding = atoi(ent->get_text().c_str());
		b->get_widget("t_fftsize", ent);
		fftsize = atoi(ent->get_text().c_str());
		Int buffers;
		buffers = bpp + padding * 2;
		for(int i = 0; i < CHANNELS; i++)
		{
			tmp[i] = new FFTFilter<jack_default_audio_sample_t>
			//bs, inbuffers,	outbuffers,	overlap,buffersperperiod,	padding,	fftsize
			(bs,  buffers,		buffers,	overlap, bpp,				padding,	fftsize);
		}
		update_fft(tmp);
		apply_pitchshift1(tmp);
		filt2 = tmp; //assumed to be atomic on all cpus, otherwise the cpu sucks and is not supported
hhhhh:
		d->hide();
	});

	b->get_widget("b_shift", bt);
	bt->signal_clicked().connect([]()
	{
		Gtk::Window* w1;
		Gtk::Dialog* d;
		b->get_widget("window1", w1);
		b->get_widget("dialog_shift", d);
		d->set_transient_for(*w1);
		if(d->run() != RESPONSE_OK)goto hhhhh;
		Gtk::Entry* ent;
		b->get_widget("t_x", ent);
		double x;
		x = atof(ent->get_text().c_str());
		for(Int i=0;i<EQ_POINTS;i++)
		{
			c->data[i]=scale_value_r(scale_value(c->data[i])*pow(2,x));
		}
		c->do_draw();
		update_fft();
	hhhhh:
		d->hide();
	});
	Gtk::FileChooserDialog* d;
	b->get_widget("filechooserdialog1", d);
	//d->signal_response().connect(&on_response);
	d->add_button(Stock::CANCEL, RESPONSE_CANCEL);
	Gtk::Viewport* v;
	b->get_widget("viewport1", v);
	v->add(*c);

	//c->set_hexpand(true);
	//c->set_vexpand(true);
	c->show();

	b->get_widget("viewport2", v);
	//if(display_spectrum)
	//{
	c2 = new EQControl(EQ_POINTS);
	v->add(*c2);
	c2->show();
	c2->MouseMove += EQControl::MouseDelegate(&on_mousemove, c2);
	/*}
	else
	{
		Gtk::Table* t1;
		b->get_widget("table1",t1);
		t1->remove(*v);
	}*/

	b->get_widget("c_spectrum", cb);
	cb->set_active(display_spectrum);
	v->set_visible(display_spectrum);
	cb->signal_toggled().connect([cb]()
	{
		display_spectrum = cb->get_active();
		Gtk::Viewport* vp;
		b->get_widget("viewport2", vp);
		vp->set_visible(display_spectrum);
	});

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

	if(jack_activate(client)) {
		fprintf(stderr, "cannot activate client");
		return 1;
	}
	g_thread_init (NULL);
	gdk_threads_init();
	//glib_threads_init();
	
	pthread_t thr;
	pthread_create(&thr,NULL,&thread1,NULL);
	kit.run(*w);

	jack_client_close(client);
	return 0;
}

