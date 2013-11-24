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
#define JACKFFT_USE_SOUNDTOUCH

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
#include "histcontrol.C"
#include <cplib/cplib.hpp>
#include <math.h>
#ifdef JACKFFT_USE_SOUNDTOUCH
#include <soundtouch/SoundTouch.h>
#endif
//#include <atomic_ops.h>

using namespace std;
#define rmb() /*AO_nop_read()*/
#define wmb() /*AO_nop_write()*/


vector<jack_port_t *> inputs;
vector<jack_port_t *> outputs;
jack_port_t* outputs2;
#define CHANNELS 2
#define CONCAT(X) (((stringstream&)(stringstream()<<X)).str())
double cur_index = 0;
int srate;
bool display_spectrum = false;
bool display_spectrogram = false;
bool output_spectrogram = false;
bool spectrum2=false;

using namespace xaxaxa;
using namespace std;

File efd(eventfd(0,0));
FFTFilter<jack_default_audio_sample_t>* filt[CHANNELS];
FFTFilter<jack_default_audio_sample_t>** filt2 = NULL;
#ifdef JACKFFT_USE_SOUNDTOUCH
soundtouch::SoundTouch soundTouch[CHANNELS];
bool apply_soundTouch=false;
#endif
struct timespec last_refreshed;
#define EQ_POINTS 10000
#define SPECTRUM2_POINTS 128
#define SPECTROGRAM_LEN 1000
EQControl* c;
EQControl* c_eq2;
EQControl* c2;
HistoryControl* c3;
Glib::RefPtr<Gtk::Builder> b;
string fname = ".default.jfft";
bool do_mux=false;
double v_mux=0.5;
static const int TOTAL_NOTES=7;
static const int OUTPUT_NOTES=14;
double spectrogramOutputPhase[OUTPUT_NOTES];

template<class t> inline double complex_to_real(const t& x)
{
	auto r = x[0];
	auto i = x[1];
	//return r;
	return sqrt(pow(r, 2) + pow(i, 2));
}

inline double abs1(double d)
{
	return d<0?-d:d;
}
//accuracy is the # of output samples
void asdasdasd(double* out, jackfft_complex* in, int len, int maxfreq, int accuracy, double multiplier=1) {
	static const int base=268; //Hz
	int max_exponent=(int)floor(log2(double(maxfreq)/base));
	if(max_exponent>5)max_exponent=5;	//ignore frequencies >= 3520Hz
	int maxf=base*pow(2,max_exponent);
	for(int i=0;i<accuracy;i++) {
		//out[i]=0;
		double x=double(i)/accuracy;
		for(int exponent=0;exponent<max_exponent;exponent++) {
			int i1=(int)floor(base*pow(2,exponent+x)/maxfreq*len);
			int i2=(int)ceil(base*pow(2,exponent+(double(i+1)/accuracy))/maxfreq*len);
			double dist=double(i2-i1)/2;
			double center=i1+dist;
			for(int zxc=i1;zxc<i2;zxc++) {
				double dist1=dist-abs1(zxc-center);
				dist1/=dist;
				out[i]+=complex_to_real(in[zxc])/len*500*accuracy*multiplier/(i2-i1)/max_exponent;
			}
		}
	}
}

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
static const int graph_range=7.;
inline double scale_value(double x)
{
	//return x*x;
	double tmp = x - 1.;
	tmp *= graph_range;
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
	return logn(2, x) / graph_range + 1.;

	/*if(std::isnan(x))return 1.0;
	//else if(x>1.0)return 2.0;
	//else if(x<-1.0)return 0.0;
	else return x+1.0;*/
}

inline Long get_nsec(const timespec& t)
{
	return ((Long)t.tv_sec) * 1000000000 + (Long)t.tv_nsec;
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
#ifdef JACKFFT_USE_SOUNDTOUCH
		if(apply_soundTouch) {
			soundTouch[i].putSamples(in,length);
			soundTouch[i].receiveSamples(out,length);
			in=out;
		}
#endif

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
	if(do_mux)
		for(size_t i=0; i<length; i++)
		{
			double avg=0;
			for(register unsigned int ii=0;ii<CHANNELS;ii++)
				avg+=((double)out_samples[ii][i]);
			avg/=CHANNELS;
			for(register unsigned int ii=0;ii<CHANNELS;ii++)
				out_samples[ii][i]=2*(avg*(1.-v_mux)+(out_samples[ii][i]-avg)*v_mux);
		}
	if(output_spectrogram && spectrum2) {
		rmb();
		jack_default_audio_sample_t *out = (jack_default_audio_sample_t *)
			jack_port_get_buffer(outputs2, length);
		if(out==NULL) goto aaaaa;
		for(int t=0;t<length;t++) out[t]=0;
		for(int i=0;i<OUTPUT_NOTES;i++) {
			double freq=268*pow(2,double(i)/TOTAL_NOTES);
			double freq2=268*8*pow(2,double(i)/TOTAL_NOTES);
			double freq3=268*16*pow(2,double(i)/TOTAL_NOTES);
			int start=i*SPECTRUM2_POINTS/TOTAL_NOTES;
			int end=(i+1)*SPECTRUM2_POINTS/TOTAL_NOTES;
			double val=0;
			for(int x=start;x<end;x++)
				if(c2->data[x]>val)val=c2->data[x];
			for(int t=0;t<length;t++) {
				out[t]+=val*sin(spectrogramOutputPhase[i]+2*M_PI*freq*t/srate)/2/TOTAL_NOTES;
			}
			for(int t=0;t<length;t++) {
				out[t]+=val*sin(spectrogramOutputPhase[i]+2*M_PI*freq2*t/srate)/7/TOTAL_NOTES;
			}
			for(int t=0;t<length;t++) {
				out[t]+=val*sin(spectrogramOutputPhase[i]+2*M_PI*freq3*t/srate)/12/TOTAL_NOTES;
			}
			spectrogramOutputPhase[i]=
				xaxaxa::modulus(spectrogramOutputPhase[i]+2*M_PI*freq*length/srate,double(2*M_PI));
		}
	}
aaaaa:
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
	if((display_spectrum || display_spectrogram) && trololo->didprocess)
	{
		trololo->didprocess = false;
		Long tmp_i=1;
		efd.Write(BufferRef(&tmp_i,sizeof(tmp_i)));
	}
	return 0;
}
void* thread1(void* v)
{
	//double* sdfsdfsdf=new double[EQ_POINTS];
	while(true)
	{
		Long tmp;
		efd.Read(BufferRef(&tmp,sizeof(tmp)));
		WARN(8,tmp);
		/*bool b=false;
		for(int i=0;i<CHANNELS;i++) {
			filt[i]->didprocess=false;
			filt[i]->doProc();
			b |= filt[i]->didprocess;
		}
		if(!b)continue;*/
		struct timespec t;
		clock_gettime(CLOCK_MONOTONIC, &t);
		if(get_nsec(last_refreshed) + (10 * 1000000) < get_nsec(t))
		{
			last_refreshed = t;
			//refresh
			for(decltype(c2->datalen) i = 0; i < c2->datalen; i++)
				c2->data[i] = 0;
			memset(c2->data, 0, c2->datalen*sizeof(double));
			for(size_t ii = 0; ii < inputs.size(); ii++)
			{
				if(filt[ii] == NULL)continue;
				auto fftf=((FFTFilter<jack_default_audio_sample_t>*)filt[ii]);
				UInt complexsize = fftf->ComplexSize();
				
				int eqpoints=spectrum2?SPECTRUM2_POINTS:EQ_POINTS;
				if(c2->datalen!=eqpoints) {
					gdk_threads_enter();
					c2->resize(eqpoints);
					c3->resize(SPECTROGRAM_LEN,eqpoints);
					gdk_threads_leave();
				}
				
				jackfft_complex* data=fftf->lastSpectrum;
				if(spectrum2) {
					asdasdasd(c2->data, data, complexsize, srate/2, c2->datalen, 1.d/inputs.size()/complexsize * 100);
				}
				else
					for(decltype(c2->datalen) i = 0; i < c2->datalen; i++)
					{
						UInt complex_index = scale_freq((double)i / (c2->datalen - 1)) * (complexsize - 1);
						c2->data[i] += complex_to_real(data[complex_index]) / complexsize / inputs.size() * 100;
					}
				//
			}
			/*for(decltype(c2->datalen) i = 0; i < c2->datalen; i++) {
				if(c2->data[i]<1./pow(2,8)) c2->data[i]=0;
				else c2->data[i]=((log(c2->data[i])/log(2))+8.)/8;
			}*/
			for(int i=0;i<3;i++)
				c3->addFrames(c2->data,1);
			gdk_threads_enter();
			if(display_spectrum)c2->queue_draw();
			if(display_spectrogram)c3->queue_draw();
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
#ifdef CEPSTRUM
		UInt complexsize = c==c_eq2?f->ComplexSize2():f->ComplexSize();
#else
		UInt complexsize = f->ComplexSize();
#endif
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
#ifdef CEPSTRUM
			if(c==c_eq2) f->coefficients2[n] = tmp;
			else
#endif
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
		b->get_widget("t_pitch", e);
		asdf = strtod(e->get_text().c_str(), NULL);
		asdf=pow(2,asdf/7.d);
		//b->get_widget("t_pitch2", e);
		//asdf /= strtod(e->get_text().c_str(), NULL);
	}
#ifdef JACKFFT_USE_SOUNDTOUCH
	if(asdf==1.) {
		apply_soundTouch=false;
	} else {
		for(int i=0;i<CHANNELS;i++) {
			soundTouch[i].setPitch(asdf);
		}
		wmb();
		apply_soundTouch=true;
	}
#else
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
#endif
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
void setFilterParams(FFTFilter<jack_default_audio_sample_t>& f)
{
	/*f.proc=[]() {
		Long tmp_i=1;
		efd.Write(BufferRef(&tmp_i,sizeof(tmp_i)));
	};*/
}
#define TOKEN_TO_STRING(TOK) # TOK
#define JACKFFT_GETWIDGET(x) b->get_widget(TOKEN_TO_STRING(x), x)

jack_client_t *client;
void enableSpectrogramOutput() {
	outputs2=(jack_port_register(client, "spectrogram",
		JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
	for(int i=0;i<OUTPUT_NOTES;i++)
		spectrogramOutputPhase[i]=0;
}
void disableSpectrogramOutput() {
	jack_port_unregister(client, outputs2);
}

int main(int argc, char *argv[])
{
	memset(&last_refreshed, 0, sizeof(last_refreshed));
	for(int i = 1; i < argc; i++)
	{
		string tmp(argv[i]);
		if(tmp == "-s")display_spectrum = true;
		if(tmp == "-S")display_spectrogram = true;
	}
	Util.ChDir(Util.GetDirFromPath(Util.GetProgramPath()));
	//goto aaaaa;
	//fft=rfftw_create_plan(8192,
	for(UInt i = 0; i < CHANNELS; i++)
	{
		FFTFilter<jack_default_audio_sample_t>* trololo = new FFTFilter<jack_default_audio_sample_t>
		//bs, inbuffers,	outbuffers,	overlap,buffersperperiod,	paddingL,paddingR,	fftsize
		(1024, 12,			12,			2,		8,					0,		 0,			8192);

		//trololo->freq_scale=9./10.;
		setFilterParams(*trololo);
		filt[i] = trololo;
	}

	
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
#ifdef JACKFFT_USE_SOUNDTOUCH
		soundTouch[i].setSampleRate(srate);
		soundTouch[i].setChannels(1);
#endif
	}

//aaaaa:
	Gtk::Main kit(argc, argv);

	b = Gtk::Builder::create_from_file("main2.ui");
	
	Gtk::Window* window1;
	Gtk::Button* b_save;
	Gtk::Button* b_saveas;
	Gtk::Button* b_open;
	Gtk::Button* b_revert;
	Gtk::Button* b_apply;
	Gtk::Button* b_params;
	Gtk::Button* b_shift;
	Gtk::CheckButton* c_pitchshift;
	Gtk::CheckButton* c_spectrum;
	Gtk::CheckButton* c_spectrogram;
	Gtk::CheckButton* c_spectrogram1;
	Gtk::CheckButton* c_spectrum2;
	Gtk::CheckButton* c_mux;
	Gtk::Label* label8;
	Gtk::FileChooserDialog* filechooserdialog1;
	Gtk::Viewport* viewport1;
	Gtk::Viewport* viewport2;
	Gtk::Viewport* viewport4;
	Gtk::Scale* s_mux;
	
	//get controls
	JACKFFT_GETWIDGET(window1); JACKFFT_GETWIDGET(b_save); JACKFFT_GETWIDGET(b_saveas);
	JACKFFT_GETWIDGET(b_open); JACKFFT_GETWIDGET(b_revert); JACKFFT_GETWIDGET(b_apply);
	JACKFFT_GETWIDGET(c_pitchshift); JACKFFT_GETWIDGET(label8); JACKFFT_GETWIDGET(b_params);
	JACKFFT_GETWIDGET(b_shift); JACKFFT_GETWIDGET(filechooserdialog1); JACKFFT_GETWIDGET(viewport1);
	JACKFFT_GETWIDGET(viewport2); JACKFFT_GETWIDGET(c_spectrum); JACKFFT_GETWIDGET(c_spectrum2);
	JACKFFT_GETWIDGET(s_mux); JACKFFT_GETWIDGET(c_mux); JACKFFT_GETWIDGET(viewport4);
	JACKFFT_GETWIDGET(c_spectrogram); JACKFFT_GETWIDGET(c_spectrogram1);
	
	//initialize controls
	b_save->signal_clicked().connect(&save);
	b_saveas->signal_clicked().connect(&saveas);
	b_open->signal_clicked().connect(&loadfile);
	b_revert->signal_clicked().connect(&load);
	b_apply->signal_clicked().connect(&apply_pitchshift);
	c_pitchshift->signal_toggled().connect(&apply_pitchshift);
	
	
	c = new EQControl(EQ_POINTS);
	
	c->Change += EQControl::ChangeDelegate(&on_change, c);
	c->MouseMove += EQControl::MouseDelegate(&on_mousemove, c);
	
#ifdef CEPSTRUM
	c_eq2 = new EQControl(EQ_POINTS);
	c_eq2->Change += EQControl::ChangeDelegate(&on_change, c_eq2);
	c_eq2->MouseMove += EQControl::MouseDelegate(&on_mousemove, c_eq2);
#endif
	//UInt complexsize = (UInt)(((FFTFilter<jack_default_audio_sample_t>*)filt[0])->PeriodSize() / 2) + 1;


	apply_label_workaround(label8);

	b_params->signal_clicked().connect([]()
	{
		Gtk::Window* w1;
		Gtk::Dialog* d;
		b->get_widget("window1", w1);
		b->get_widget("dialog1", d);
		d->set_transient_for(*w1);
		if(d->run() != RESPONSE_APPLY)goto hhhhh;

		FFTFilter<jack_default_audio_sample_t>** tmp;
		tmp = new FFTFilter<jack_default_audio_sample_t>*[CHANNELS];
		Int bs, overlap, bpp, padding1, padding2, fftsize;
		Gtk::Entry* ent;
		b->get_widget("t_bs", ent);
		bs = atoi(ent->get_text().c_str());
		b->get_widget("t_overlap", ent);
		overlap = atoi(ent->get_text().c_str());
		b->get_widget("t_bpp", ent);
		bpp = atoi(ent->get_text().c_str());
		b->get_widget("t_padding1", ent);
		padding1 = atoi(ent->get_text().c_str());
		b->get_widget("t_padding2", ent);
		padding2 = atoi(ent->get_text().c_str());
		b->get_widget("t_fftsize", ent);
		fftsize = atoi(ent->get_text().c_str());
		Int buffers;
		buffers = (bpp + padding1 + padding2) + 4;
		for(int i = 0; i < CHANNELS; i++)
		{
			tmp[i] = new FFTFilter<jack_default_audio_sample_t>
			//bs, inbuffers,	outbuffers,	overlap,buffersperperiod,	padding1,padding2	fftsize
			(bs,  buffers,		buffers,	overlap,bpp,				padding1,padding2,	fftsize);
			setFilterParams(*(tmp[i]));
		}
		update_fft(tmp);
		apply_pitchshift1(tmp);
		filt2 = tmp; //assumed to be atomic on all cpus, otherwise the cpu sucks and is not supported
hhhhh:
		d->hide();
	});

	b_shift->signal_clicked().connect([]()
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
	//d->signal_response().connect(&on_response);
	filechooserdialog1->add_button(Stock::CANCEL, RESPONSE_CANCEL);
	viewport1->add(*c);

	//c->set_hexpand(true);
	//c->set_vexpand(true);
	c->show();
	
#ifdef CEPSTRUM
	Gtk::Viewport* vp_eq2;
	b->get_widget("vp_eq2", vp_eq2);
	vp_eq2->add(*c_eq2);
	c_eq2->show();
#endif
	

	//if(display_spectrum)
	//{
	c2 = new EQControl(EQ_POINTS);
	viewport2->add(*c2);
	c2->show();
	c2->MouseMove += EQControl::MouseDelegate(&on_mousemove, c2);
	
	c3 = new HistoryControl(SPECTROGRAM_LEN,EQ_POINTS);
	c3->show();
	viewport4->add(*c3);
	/*}
	else
	{
		Gtk::Table* t1;
		b->get_widget("table1",t1);
		t1->remove(*v);
	}*/

	//spectrum display
	c_spectrum->set_active(display_spectrum);
	viewport2->set_visible(display_spectrum);
	c_spectrum->signal_toggled().connect([c_spectrum]()
	{
		Gtk::Window* w1;
		Gtk::Viewport* v;
		b->get_widget("window1", w1);
		b->get_widget("viewport1", v);
		
		gint ww,wh; w1->get_size(ww,wh);
		if(display_spectrum && !c_spectrum->get_active()) {
			w1->resize(ww,wh-v->get_allocation().get_height());
		} else if(!display_spectrum && c_spectrum->get_active()) {
			w1->resize(ww,wh+v->get_allocation().get_height());
		}
		display_spectrum = c_spectrum->get_active();
		Gtk::Viewport* vp;
		b->get_widget("viewport2", vp);
		vp->set_visible(display_spectrum);
	});
	
	//spectrogram display
	c_spectrogram->set_active(display_spectrogram);
	viewport4->set_visible(display_spectrogram);
	c_spectrogram->signal_toggled().connect([c_spectrogram]()
	{
		Gtk::Window* w1;
		Gtk::Viewport* v;
		b->get_widget("window1", w1);
		b->get_widget("viewport1", v);
		
		gint ww,wh; w1->get_size(ww,wh);
		if(display_spectrogram && !c_spectrogram->get_active()) {
			w1->resize(ww,wh-v->get_allocation().get_height());
		} else if(!display_spectrogram && c_spectrogram->get_active()) {
			w1->resize(ww,wh+v->get_allocation().get_height());
		}
		display_spectrogram = c_spectrogram->get_active();
		Gtk::Viewport* vp;
		b->get_widget("viewport4", vp);
		vp->set_visible(display_spectrogram);
	});
	
	//spectrogram output
	c_spectrogram1->set_active(output_spectrogram);
	c_spectrogram1->signal_toggled().connect([c_spectrogram1]()
	{
		if(output_spectrogram && !c_spectrogram1->get_active()) {
			disableSpectrogramOutput();
		} else if(!output_spectrogram && c_spectrogram1->get_active()) {
			enableSpectrogramOutput();
		}
		wmb();
		output_spectrogram=c_spectrogram1->get_active();
	});
	
	c_spectrum2->set_active(spectrum2);
	c_spectrum2->signal_toggled().connect([c_spectrum2]()
	{
		spectrum2 = c_spectrum2->get_active();
	});
	
	c_mux->set_active(do_mux);
	Gtk::Adjustment adj_mux(v_mux,0,1,0.01);
	s_mux->set_adjustment(adj_mux);
	
	auto update_mux=[&]() {
		do_mux=c_mux->get_active();
		v_mux=adj_mux.get_value();
	};
	
	s_mux->signal_value_changed().connect(update_mux);
	c_mux->signal_toggled().connect(update_mux);
	
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
	kit.run(*window1);

	jack_client_close(client);
	return 0;
}

