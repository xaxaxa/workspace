#include <gtkmm.h>
#include <gdk/gdk.h>
#include "eqcontrol.C"
#include "fftfilter.C"
#include <math.h>
#include <cpoll/cpoll.H>
#include <vector>
#include <type_traits>
#include <sigc++/sigc++.h>
#include <functional>
#include <jack/jack.h>
using namespace std;
using namespace xaxaxa;
using namespace CP;

//globals
int jack_srate;
jack_port_t* jack_in;
jack_port_t* jack_out;
struct play_info {
	double* data;
	int length,pos;
};
play_info play_data{nullptr},rec_data{nullptr};
pthread_mutex_t mutex;
function<void()> rec_cb=[](){};
double volume=1;
bool is_playing=false;
//play from or record to the specified buffer repeatedly
void beginPlay(double* data, int datalen, bool record=false) {
	play_info tmp {data,datalen,0};
	pthread_mutex_lock(&mutex);
	if(record) rec_data=tmp;
	else play_data=tmp;
	pthread_mutex_unlock(&mutex);
}
void endPlay(bool record=false) {
a:
	pthread_mutex_lock(&mutex);
	if(record) rec_data.data=nullptr;
	else play_data.data=nullptr;
	if(is_playing) {
		pthread_mutex_unlock(&mutex);
		timespec ts;
		ts.tv_sec=0;
		ts.tv_nsec=1000000;
		nanosleep(&ts,nullptr);
		goto a;
	}
	pthread_mutex_unlock(&mutex);
}
void jack_error (const char *desc) {
	fprintf (stderr, "JACK error: %s\n", desc);
}
void jack_shutdown (void *arg) { }
int jack_process (jack_nframes_t length, void *arg) {
	play_info play,rec;
	pthread_mutex_lock(&mutex);
	play=play_data;
	rec=rec_data;
	is_playing=true;
	pthread_mutex_unlock(&mutex);
	jack_default_audio_sample_t* out = (jack_default_audio_sample_t*)jack_port_get_buffer(jack_out, length);
	jack_default_audio_sample_t* in = (jack_default_audio_sample_t*)jack_port_get_buffer(jack_in, length);
	if(play.data!=nullptr) {
		int outpos=0;
		int l=play.length-play.pos;
		if(l>(int)length)l=length;
		for(int i=0;i<l;i++) out[i]=play.data[i+play.pos]*volume;
		outpos+=l;
		while(outpos<(int)length) {
			l=length-outpos;
			if(l>play.length) l=play.length;
			for(int i=0;i<l;i++) out[outpos+i]=play.data[i]*volume;
		}
		play.pos=(play.pos+length)%play.length;
	} else {
		memset(out,0,sizeof(jack_default_audio_sample_t)*length);
	}
	if(rec.data!=nullptr) {
		int outpos=0;
		int l=rec.length-rec.pos;
		if(l>(int)length)l=length;
		for(int i=0;i<l;i++) rec.data[i+rec.pos]=in[i];
		if(l==rec.length-rec.pos) rec_cb();
		//printf("%i\n",rec.pos);
		outpos+=l;
		while(outpos<(int)length) {
			l=length-outpos;
			if(l>rec.length) l=rec.length;
			for(int i=0;i<l;i++) rec.data[i]=in[outpos+i];
			if(l==rec.length) rec_cb();
		}
		rec.pos=(rec.pos+length)%rec.length;
	}
	pthread_mutex_lock(&mutex);
	if(play.data==play_data.data && play.length==play_data.length)
		play_data.pos=play.pos;
	if(rec.data==rec_data.data && rec.length==rec_data.length)
		rec_data.pos=rec.pos;
	is_playing=false;
	pthread_mutex_unlock(&mutex);
	return 0;
}


namespace sigc
{
    template <typename Functor>
    struct functor_trait<Functor, false>
    {
        typedef decltype (::sigc::mem_fun (std::declval<Functor&> (),
                                           &Functor::operator())) _intermediate;

        typedef typename _intermediate::result_type result_type;
        typedef Functor functor_type;
    };
}
#define TOKEN_TO_STRING(TOK) # TOK
#define JACKFFT_GETWIDGET(builder,x) builder->get_widget(TOKEN_TO_STRING(x), x)

double* get_amplitude(jackfft_complex* data, int datalen) {
	double* tmp=new double[datalen];
	double a=1.0/sqrt(datalen);
	for(int i=0;i<datalen;i++) {
		auto point=data[i];
		tmp[i]=sqrt(point[0]*point[0]+point[1]*point[1])*a-1;
	}
	return tmp;
}
void do_rotate(double* data, int datalen, int amount) {
	if(amount<0) { //rotate left
		double buf[-amount];
		memcpy(buf,data,sizeof(double)*-amount);
		memmove(data,data-amount,sizeof(double)*(datalen+amount));
		memmove(data+datalen+amount,buf,sizeof(double)*-amount);
	} else if(amount>0) {
		double buf[amount];
		memcpy(buf,data+datalen-amount,sizeof(double)*amount);
		memmove(data+amount,data,sizeof(double)*(datalen-amount));
		memmove(data,buf,sizeof(double)*amount);
	}
}
inline void multiply_complex(jackfft_complex& res, jackfft_complex c1, jackfft_complex c2) {
	jackfft_complex c11={c1[0],c1[1]};
	jackfft_complex c21={c2[0],c2[1]};
	res[0]=c11[0]*c21[0]-c11[1]*c21[1];
	res[1]=c11[0]*c21[1]+c11[1]*c21[0];
}
double db_to_1(double d) {
	return pow(10,d/10);
}
bool prompt(Glib::RefPtr<Gtk::Builder> builder, Gtk::Window* parent, const char* label,
	const char* def, string& response) {
	Gtk::Dialog* dialog;
	Gtk::Label* l;
	Gtk::Entry* t;
	builder->get_widget("dialog_prompt",dialog);
	builder->get_widget("prompt_label",l);
	builder->get_widget("prompt_text",t);
	l->set_label(label);
	t->set_text(def);
	dialog->set_transient_for(*parent);
	if(dialog->run() == RESPONSE_OK) {
		dialog->hide();
		response=t->get_text();
		return true;
	}
	dialog->hide();
	return false;
}
bool prompt_saveas(Glib::RefPtr<Gtk::Builder> builder, Gtk::Window* parent, string& path) {
	bool ret=false;
	Gtk::FileChooserDialog* dialog;
	builder->get_widget("filechooser",dialog);
	dialog->set_transient_for(*parent);
	dialog->set_action(FILE_CHOOSER_ACTION_SAVE);
	Gtk::Button* b1 = dialog->add_button(Stock::SAVE, RESPONSE_OK);
	if(dialog->run() == RESPONSE_OK) {
		path=dialog->get_filename();
		ret=true;
	}
	dialog->hide();
	b1->get_parent()->Gtk::Container::remove(*b1);
	return ret;
}
bool prompt_open(Glib::RefPtr<Gtk::Builder> builder, Gtk::Window* parent, string& path) {
	bool ret=false;
	Gtk::FileChooserDialog* dialog;
	builder->get_widget("filechooser",dialog);
	dialog->set_transient_for(*parent);
	dialog->set_action(FILE_CHOOSER_ACTION_OPEN);
	Gtk::Button* b1 = dialog->add_button(Stock::OPEN, RESPONSE_OK);
	if(dialog->run() == RESPONSE_OK) {
		path=dialog->get_filename();
		ret=true;
	}
	dialog->hide();
	b1->get_parent()->Gtk::Container::remove(*b1);
	return ret;
}
void alert(Glib::RefPtr<Gtk::Builder> builder, Gtk::Window* parent, const char* message) {
	Gtk::MessageDialog* dialog;
	builder->get_widget("dialog_message",dialog);
	dialog->set_transient_for(*parent);
	dialog->set_message(message);
	dialog->run();
	dialog->hide();
}
struct WaveDisplay: public Gtk::DrawingArea {
	static const int peaklength=1024;
	typedef double peak_t[2];
	peak_t* peaks;
	WaveDisplay() {
		peaks=new peak_t[peaklength];
		set_app_paintable(true);
		set_events(get_events()|EXPOSURE_MASK);
		set_redraw_on_allocate(true);
		signal_draw().connect(sigc::mem_fun(*this,&WaveDisplay::img_draw));
	}
	~WaveDisplay() {
		delete[] peaks;
	}
	bool img_draw(const ::Cairo::RefPtr<::Cairo::Context>& gc) {
		int w=get_allocation().get_width();
		int h=get_allocation().get_height();
		double x1,y1,x2,y2;
		gc->get_clip_extents(x1,y1,x2,y2);
		gc->set_line_width(1);
		gc->set_antialias(::Cairo::ANTIALIAS_NONE);
		gc->set_line_cap(::Cairo::LINE_CAP_SQUARE);
		
		for(int x=(int)x1;x<(int)x2;x++) {
			int s=int64_t(x)*peaklength/w;
			int e=int64_t(x+1)*peaklength/w;
			if(e>=peaklength)e=peaklength-1;
			double min=peaks[s][0],max=peaks[s][1];
			for(int i=s+1;i<=e;i++) {
				if(peaks[i][0]<min)min=peaks[i][0];
				if(peaks[i][1]>max)max=peaks[i][1];
			}
			double y1=floor((1-max)*h/2);
			double y2=ceil((1-min)*h/2);
			if(y1>=h)y1=h-1;
			if(y2<1)y2=1;
			
			gc->move_to(x+1,1);
			gc->line_to(x+1,y1+1);
			gc->set_source_rgb(1,1,1);
			gc->stroke();
			
			gc->move_to(x+1,y1+1);
			gc->line_to(x+1,y2+2);
			gc->set_source_rgb(1,0,0);
			gc->stroke();
			
			gc->move_to(x+1,y2+2);
			gc->line_to(x+1,h+1);
			gc->set_source_rgb(1,1,1);
			gc->stroke();
		}
		/*gc->move_to(1,1);
		gc->line_to(1,1);
		gc->set_source_rgb(1,1,0);
		gc->stroke();
		gc->begin_new_path();
		gc->move_to(2,2);
		gc->line_to(3,3);
		gc->set_source_rgb(0,0,1);
		gc->stroke();*/
		return true;
	}
	void update_img(double* data, int datalen) {
		for(int i=0;i<peaklength;i++) {
			int s=int64_t(i)*datalen/peaklength;
			int e=int64_t(i+1)*datalen/peaklength;
			if(e>=datalen)e=datalen-1;
			double min=data[s],max=data[s];
			for(int x=s+1;x<=e;x++) {
				if(!isfinite(data[x])) continue;
				if(data[x]<min || !isfinite(min))min=data[x];
				if(data[x]>max || !isfinite(min))max=data[x];
			}
			if(!isnormal(min))min=0;
			if(!isnormal(max))max=0;
			if(min<-1)min=-1;
			if(min>1)min=1;
			if(max<-1)max=-1;
			if(max>1)max=1;
			peaks[i][0]=min;
			peaks[i][1]=max;
		}
	}
	void clear(double value=0) {
		for(int i=0;i<peaklength;i++) {
			peaks[i][0]=value;
			peaks[i][1]=value;
		}
	}
};
class WaveListItem: public Gtk::Box {
public:
	Gtk::CheckButton cb;
	WaveDisplay img;
	WaveListItem():Gtk::Box(ORIENTATION_HORIZONTAL,0) {
		pack_start(cb,false,true);
		pack_start(img,true,true);
	}
	void update_img(double* data, int datalen) {
		img.update_img(data,datalen);
	}
};
struct jackfft_wavegen {
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* mainWindow;
	EQControl graph_amplitude;
	EQControl graph_phase;
	WaveDisplay graph_impulse;
	FFT* fft;
	jackfft_wavegen():graph_amplitude(4097),graph_phase(4097) {
		fft=new FFT(8192,true);
	}
	void init() {
		initUI();
		mainWindow->show_all();
		update_graph_impulse(nullptr);
	}
	template<class WidgetType>
	void getWidget(string name, WidgetType*& w) {
		builder->get_widget("wavegen_"+name, w);
	}
	void initUI() {
		getWidget("w", mainWindow);
		
		Gtk::Viewport* vp;
		Gtk::Button* b;
		
		getWidget("vp_amplitude",vp);
		vp->add(graph_amplitude);
		graph_amplitude.MouseMove += EQControl::MouseDelegate(&jackfft_wavegen::graph_mousemove, this);
		graph_amplitude.signal_button_release_event().connect(sigc::mem_fun(*this,&jackfft_wavegen::update_graph_impulse));
		graph_amplitude.signal_motion_notify_event().connect(sigc::mem_fun(*this,&jackfft_wavegen::graph_amplitude_mousemove));
		
		getWidget("vp_phase",vp);
		vp->add(graph_phase);
		graph_phase.MouseMove += EQControl::MouseDelegate(&jackfft_wavegen::graph_mousemove, this);
		graph_phase.signal_button_release_event().connect(sigc::mem_fun(*this,&jackfft_wavegen::update_graph_impulse));
		graph_phase.signal_motion_notify_event().connect(sigc::mem_fun(*this,&jackfft_wavegen::graph_phase_mousemove));
		
		getWidget("vp_impulse",vp);
		graph_impulse.set_events(graph_impulse.get_events()|POINTER_MOTION_MASK|BUTTON_MOTION_MASK);
		vp->add(graph_impulse);
		graph_impulse.signal_motion_notify_event().connect(sigc::mem_fun(*this,&jackfft_wavegen::graph_impulse_mousemove));
		
		getWidget("b_constant",b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_wavegen::b_constant_clicked));
		getWidget("b_constant1",b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_wavegen::b_constant1_clicked));
		getWidget("b_random",b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_wavegen::b_random_clicked));
		getWidget("b_resize",b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_wavegen::b_resize_clicked));
		getWidget("b_normalize",b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_wavegen::b_normalize_clicked));
		getWidget("b_saveas",b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_wavegen::b_saveas_clicked));
	}
	void resize(int fftsize) {
		graph_phase.resize((uint32_t)fftsize/2+1);
		graph_amplitude.resize((uint32_t)fftsize/2+1);
		delete fft;
		fft=new FFT(fftsize,true);
		graph_phase.queue_draw();
		update_graph_impulse(nullptr);
	}
	void b_constant_clicked() {
		string tmp;
		if(!prompt(builder,mainWindow,"Phase (in degrees)", "0", tmp)) return;
		double phase=strtod(tmp.c_str(),nullptr)*M_PI/180;
		double d=phase_to_graph(phase);
		for(uint32_t i=0;i<graph_phase.datalen;i++) {
			graph_phase.data[i]=d;
		}
		graph_phase.queue_draw();
		update_graph_impulse(nullptr);
	}
	void b_constant1_clicked() {
		string tmp;
		if(!prompt(builder,mainWindow,"Amplitude", "1", tmp)) return;
		double d=amplitude_to_graph(strtod(tmp.c_str(),nullptr));
		for(uint32_t i=0;i<graph_amplitude.datalen;i++) {
			graph_amplitude.data[i]=d;
		}
		graph_amplitude.queue_draw();
		update_graph_impulse(nullptr);
	}
	void b_random_clicked() {
		for(uint32_t i=0;i<graph_phase.datalen;i++) {
			graph_phase.data[i]=drand48();
		}
		graph_phase.queue_draw();
		update_graph_impulse(nullptr);
	}
	void b_resize_clicked() {
		char s[64];
		snprintf(s,sizeof(s),"%i",fft->size);
		string tmp;
		if(!prompt(builder,mainWindow,"Impulse response length", s, tmp)) return;
		resize(atoi(tmp.c_str()));
	}
	void b_normalize_clicked() {
		double max=0,min=0;
		for(int i=0;i<fft->size;i++) {
			double d=fft->Data[i];
			if(d>max)max=d;
			if(d<min)min=d;
		}
		double max_a=(-min<=max)?max:-min;
		if(max_a==0) return;
		double a=1.0/max_a;
		for(int i=0;i<fft->size;i++) {
			fft->Data[i]*=a;
		}
		graph_impulse.update_img(fft->Data,fft->size);
		graph_impulse.queue_draw();
		update_peak((-min<=max)?1:-1);
	}
	void b_saveas_clicked() {
		string tmp;
		if(!prompt_saveas(builder,mainWindow,tmp)) return;
		CP::File f(tmp.c_str(),O_RDWR|O_TRUNC|O_CREAT,0666);
		f.writeAll((u8*)fft->Data,sizeof(double)*fft->size);
	}
	
	//in radians
	double graph_to_phase(double d) {
		if(d<0)d=0;
		if(d>1)d=1;
		d=d*2-1;
		return d*M_PI;
	}
	double phase_to_graph(double d) {
		while(d<-M_PI)d+=2*M_PI;
		while(d>M_PI)d-=2*M_PI;
		return (d/M_PI+1)/2;
	}
	static const int graph_range=7.;
	inline double logn(double n, double x) {
		return log(x) / log(n);
	}
	double graph_to_amplitude(double x) {
		double tmp = x - 0.5;
		tmp *= graph_range;
		return pow(2, tmp);
	}
	double amplitude_to_graph(double x) {
		return logn(2, x) / graph_range + 0.5;
	}
	bool update_graph_impulse(GdkEventButton* event) {
		double* data=graph_phase.data;
		double* data2=graph_amplitude.data;
		fft->Data_c[0][0]=graph_to_amplitude(data2[0]);
		fft->Data_c[0][1]=0;
		for(int i=1;i<fft->size_c;i++) {
			double a=graph_to_amplitude(data2[i]);
			fft->Data_c[i][0]=a*cos(graph_to_phase(data[i]));
			fft->Data_c[i][1]=a*sin(graph_to_phase(data[i]));
		}
		fft->Reverse();
		double max=0,min=0;
		for(int i=0;i<fft->size;i++) {
			double d=fft->Data[i]/fft->size;
			fft->Data[i]=d;
			if(d>max)max=d;
			if(d<min)min=d;
		}
		graph_impulse.update_img(fft->Data,fft->size);
		update_peak((-min<=max)?max:min);
		graph_impulse.queue_draw();
		return false;
	}
	void update_peak(double peak) {
		Gtk::Label* l;
		getWidget("l_peak", l);
		char tmp[64];
		snprintf(tmp,64,"peak: %.2f",peak);
		l->set_text(tmp);
	}
	void graph_mousemove(double i) {
		Gtk::Label* l;
		getWidget("l_freq", l);
		char tmp[64];
		snprintf(tmp,64,"%i",(int)i);
		l->set_text(tmp);
	}
	bool graph_phase_mousemove(GdkEventMotion* event) {
		if(event->y<0)event->y=0;
		uint32_t h=graph_phase.get_allocation().get_height();
		Gtk::Label* l;
		getWidget("l_phase", l);
		char tmp[64];
		snprintf(tmp,64,"%i °",int(180-(event->y*360/h)));
		l->set_text(tmp);
		return false;
	}
	bool graph_amplitude_mousemove(GdkEventMotion* event) {
		if(event->y<0)event->y=0;
		uint32_t h=graph_amplitude.get_allocation().get_height();
		Gtk::Label* l;
		getWidget("l_phase", l);
		char tmp[64];
		snprintf(tmp,64,"%.3f",graph_to_amplitude(1-(double(event->y)/h)));
		l->set_text(tmp);
		return false;
	}
	bool graph_impulse_mousemove(GdkEventMotion* event) {
		if(event->x<0)event->x=0;
		uint32_t w=graph_impulse.get_allocation().get_width();
		graph_mousemove(double(event->x)*fft->size/w);
		return false;
	}
};
struct jackfft_analyzer_measure {
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* mainWindow;
	WaveDisplay graph_test,graph_amplitude,graph_phase,graph_impulse;
	FFT* fft_test;
	FFT* fft_measure;
	int rotate_by;
	jackfft_analyzer_measure() {
		fft_measure=fft_test=nullptr;
		rotate_by=0;
	}
	~jackfft_analyzer_measure() {
		if(fft_test!=nullptr) delete fft_test;
		if(fft_measure!=nullptr) delete fft_measure;
	}
	void init() {
		graph_test.clear(-1);
		graph_amplitude.clear(-1);
		graph_phase.clear(-1);
		graph_impulse.clear(0);
		initUI();
		mainWindow->show_all();
	}
	template<class WidgetType>
	void getWidget(string name, WidgetType*& w) {
		builder->get_widget("measure_"+name, w);
	}
	void initUI() {
		getWidget("w", mainWindow);
		Gtk::Button* b;
		Gtk::Viewport* vp;
		Gtk::Entry* t;
		Gtk::Scale* s;
		
		getWidget("vp_test", vp);
		vp->add(graph_test);
		getWidget("vp_amplitude", vp);
		vp->add(graph_amplitude);
		getWidget("vp_phase", vp);
		vp->add(graph_phase);
		getWidget("vp_impulse", vp);
		vp->add(graph_impulse);
		getWidget("s_volume",s);
		s->signal_change_value().connect([this](ScrollType scroll, double new_value){
			volume=db_to_1(new_value);
			return false;
		});
		getWidget("s_phase",s);
		s->signal_change_value().connect([this](ScrollType scroll, double new_value){
			rotate_by=new_value*fft_test->size/200;
			return false;
		});
		getWidget("t_periods", t);
		t->set_width_chars(3);
		getWidget("b_start", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer_measure::b_start_clicked));
		getWidget("b_stop", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer_measure::b_stop_clicked));
	}
	void b_start_clicked() {
		Gtk::Scale* s;
		Gtk::Entry* t;
		getWidget("s_volume",s);
		volume=db_to_1(s->get_adjustment()->get_value());
		getWidget("s_phase",s);
		rotate_by=s->get_adjustment()->get_value()*fft_test->size/200;
		beginPlay(fft_test->Data,fft_test->size);
		getWidget("t_periods", t);
		int size=fft_test->size*atoi(t->get_text().c_str());
		if(fft_measure==nullptr) fft_measure=new FFT(size,true);
		else if(fft_measure->size!=size) {
			delete fft_measure;
			fft_measure=new FFT(size,true);
		}
		rec_cb=[this](){received_buffer();};
		beginPlay(fft_measure->Data,size,true);
		Gtk::Button* b;
		getWidget("b_start", b); b->hide();
		getWidget("b_stop", b); b->show();
	}
	void received_buffer() {
		do_rotate(fft_measure->Data,fft_measure->size,rotate_by);
		fft_measure->Forward();
		g_idle_add(&jackfft_analyzer_measure::received_buffer1,this);
	}
	static int received_buffer1(void* v) {
		jackfft_analyzer_measure* This=(jackfft_analyzer_measure*)v;
		This->received_buffer2();
		return 0;
	}
	void received_buffer2() {
		int periods=fft_measure->size/fft_test->size;
		graph_impulse.update_img(fft_measure->Data,fft_measure->size);
		graph_impulse.queue_draw();
		int datalen=fft_test->size_c;
		double* tmp=new double[datalen*2];
		double* tmp2=tmp+datalen;
		double a=1.0/sqrt(datalen);
		for(int i=0;i<datalen;i++) {
			auto point=fft_measure->Data_c[i*periods];
			auto point2=fft_test->Data_c[i];
			double asdfg=point2[0]*point2[0]+point2[1]*point2[1];
			double re=(point[0]*point2[0]+point[1]*point2[1])/asdfg;
			double im=(point[1]*point2[0]-point[0]*point2[1])/asdfg;
			tmp[i]=sqrt(re*re+im*im)*a-1;
			tmp2[i]=atan2(im,re)/M_PI;
		}
		graph_amplitude.update_img(tmp,datalen);
		graph_amplitude.queue_draw();
		graph_phase.update_img(tmp2,datalen);
		graph_phase.queue_draw();
		delete[] tmp;
	}
	void b_stop_clicked() {
		endPlay(); endPlay(true);
		Gtk::Button* b;
		getWidget("b_start", b); b->show();
		getWidget("b_stop", b); b->hide();
	}
	void setTestWave(double* data, int datalen) {
		b_stop_clicked();
		if(fft_test!=nullptr) delete fft_test;
		fft_test=new FFT(datalen,true);
		memcpy(fft_test->Data,data,sizeof(double)*datalen);
		fft_test->Forward();
		double* tmp=get_amplitude(fft_test->Data_c,fft_test->size_c);
		graph_test.update_img(tmp,fft_test->size_c);
		delete[] tmp;
	}
};
struct jackfft_analyzer {
	struct analyzerListItem: public WaveListItem {
		FFT fft;
		analyzerListItem(int datalen):fft(datalen,true) {
			
		}
		void drawTimeDomain() {
			update_img(fft.Data,fft.size);
		}
		void drawFreqDomain() {
			double* tmp=get_amplitude(fft.Data_c,fft.size_c);
			update_img(tmp,fft.size_c);
			delete[] tmp;
		}
		void time2freq() {
			fft.Forward();
		}
		void freq2time() {
			fft.Reverse();
			for(int i=0;i<fft.size;i++) {
				fft.Data[i]/=fft.size;
			}
		}
	};
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* mainWindow;
	jackfft_wavegen* wg;
	jackfft_analyzer_measure* m;
	vector<analyzerListItem*> items;
	jackfft_analyzer() {
		wg=nullptr;
		m=nullptr;
	}
	void init() {
		initUI();
		mainWindow->show_all();
	}
	template<class WidgetType>
	void getWidget(string name, WidgetType*& w) {
		builder->get_widget("analyzer_"+name, w);
	}
	void initUI() {
		getWidget("w", mainWindow);
		Gtk::Button* b;
		Gtk::RadioButton* rb;
		
		getWidget("b_cconvolve", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_cconvolve_clicked));
		getWidget("b_convolve", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_convolve_clicked));
		getWidget("b_inverse", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_inverse_clicked));
		getWidget("b_measure", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_measure_clicked));
		getWidget("b_remove", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_remove_clicked));
		getWidget("b_selectall", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_selectall_clicked));
		getWidget("b_deselectall", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_deselectall_clicked));
		getWidget("b_rotate", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_rotate_clicked));
		getWidget("b_saveas", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_saveas_clicked));
		getWidget("b_noise", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_noise_clicked));
		getWidget("b_open", b);
		b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_open_clicked));
		getWidget("rb_time", rb);
		rb->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::rb_timefreq_clicked));
	}
	void b_cconvolve_clicked() {
		int len=0,cnt=0;
		vector<analyzerListItem*> tmpitems;
		for(int i=0;i<(int)items.size();i++) {
			if(items[i]->cb.get_active()) {
				if(len==0) len=items[i]->fft.size_c;
				else if(len!=items[i]->fft.size_c) {
					alert(builder,mainWindow,"All selected items must be of the same length");
					return;
				}
				cnt++;
				tmpitems.push_back(items[i]);
			}
		}
		if(cnt<=1) {
			alert(builder,mainWindow,"Select at least two items");
			return;
		}
		analyzerListItem* item=new analyzerListItem((len-1)*2);
		memcpy(item->fft.Data_c,tmpitems[0]->fft.Data_c,sizeof(jackfft_complex)*len);
		jackfft_complex* res=item->fft.Data_c;
		for(int i=1;i<cnt;i++) {
			jackfft_complex* tmp_a=tmpitems[i]->fft.Data_c;
			for(int ii=0;ii<len;ii++) {
				multiply_complex(res[ii],res[ii],tmp_a[ii]);
			}
		}
		item->freq2time();
		doAddItem(item);
	}
	void b_convolve_clicked() {
		int len=0,cnt=0;
		analyzerListItem* item1=nullptr,* item2=nullptr;
		for(int i=0;i<(int)items.size();i++) {
			if(items[i]->cb.get_active()) {
				if(item1==nullptr) item1=items[i];
				else if(item2==nullptr) item2=items[i];
				else {
					alert(builder,mainWindow,"Select only two items");
					return;
				}
			}
		}
		if(item1==nullptr || item2==nullptr) {
			alert(builder,mainWindow,"Select two items");
			return;
		}
		analyzerListItem* item=new analyzerListItem(item1->fft.size+item2->fft.size);
		memcpy(item->fft.Data,item1->fft.Data,sizeof(double)*item1->fft.size);
		memset(item->fft.Data+item1->fft.size,0,sizeof(double)*item2->fft.size);
		item->time2freq();
		FFT tmpfft(item->fft.size,true);
		memcpy(tmpfft.Data,item2->fft.Data,sizeof(double)*item2->fft.size);
		memset(tmpfft.Data+item2->fft.size,0,sizeof(double)*item1->fft.size);
		tmpfft.Forward();
		for(int i=0;i<item->fft.size_c;i++) {
			multiply_complex(item->fft.Data_c[i],item->fft.Data_c[i],tmpfft.Data_c[i]);
		}
		item->freq2time();
		doAddItem(item,false);
	}
	void b_inverse_clicked() {
		bool b=false;
		for(int i=0;i<(int)items.size();i++) {
			if(items[i]->cb.get_active()) {
				int len=items[i]->fft.size,len2=items[i]->fft.size_c;
				analyzerListItem* item=new analyzerListItem(len);
				jackfft_complex* tmp_a=items[i]->fft.Data_c;
				jackfft_complex* tmp_a2=item->fft.Data_c;
				for(int ii=0;ii<len2;ii++) {
					double tmp=tmp_a[ii][0]*tmp_a[ii][0]+tmp_a[ii][1]*tmp_a[ii][1];
					if(tmp<0.000001) {
						b=true;
						tmp_a2[ii][0]=tmp_a[ii][0];
						tmp_a2[ii][1]=tmp_a[ii][1];
						continue;
					}
					tmp_a2[ii][0]=tmp_a[ii][0]/tmp;
					tmp_a2[ii][1]=-tmp_a[ii][1]/tmp;
				}
				item->freq2time();
				doAddItem(item);
			}
		}
		if(b) alert(builder,mainWindow,"Warning: some items were not invertible because of zeroes in its Fourier transform");
	}
	void b_measure_clicked() {
		analyzerListItem* item=nullptr;
		for(int i=0;i<(int)items.size();i++) {
			if(items[i]->cb.get_active()) {
				if(item==nullptr) item=items[i];
				else {
					alert(builder,mainWindow,"Please select one item only");
					return;
				}
			}
		}
		if(item==nullptr) {
			alert(builder,mainWindow,"Select the item to be used as the test waveform");
			return;
		}
		if(m==nullptr) {
			m=new jackfft_analyzer_measure();
			m->builder=builder;
			m->init();
			Gtk::Button* b;
			builder->get_widget("measure_b_import", b);
			b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_import_m_clicked));
		}
		m->setTestWave(item->fft.Data,item->fft.size);
		m->mainWindow->present();
	}
	void b_remove_clicked() {
		for(int i=0;i<(int)items.size();i++) {
			if(items[i]->cb.get_active()) {
				delete items[i];
				items.erase(items.begin()+i);
				i--;
			}
		}
	}
	void b_selectall_clicked() {
		for(int i=0;i<(int)items.size();i++)
			items[i]->cb.set_active(true);
	}
	void b_deselectall_clicked() {
		for(int i=0;i<(int)items.size();i++)
			items[i]->cb.set_active(false);
	}
	void b_rotate_clicked() {
		analyzerListItem* item=nullptr;
		for(int i=0;i<(int)items.size();i++) 
			if(items[i]->cb.get_active()) {
				if(item!=nullptr) {
					alert(builder,mainWindow,"Select one item only");
					return;
				}
				item=items[i];
			}
		char def[64];
		snprintf(def,sizeof(def),"%i",item->fft.size/2);
		string res;
		if(!prompt(builder,mainWindow,"Amount to rotate by (samples)",def,res)) return;
		do_rotate(item->fft.Data,item->fft.size,atoi(res.c_str()));
		item->time2freq();
		doRedrawItem(item);
		item->queue_draw();
	}
	void b_saveas_clicked() {
		string tmp;
		for(int i=0;i<(int)items.size();i++) {
			if(items[i]->cb.get_active()) {
				if(!prompt_saveas(builder,mainWindow,tmp)) return;
				CP::File f(tmp.c_str(),O_RDWR|O_TRUNC|O_CREAT,0666);
				f.writeAll((u8*)items[i]->fft.Data,sizeof(double)*items[i]->fft.size);
			}
		}
	}
	void b_noise_clicked() {
		if(wg==nullptr) {
			wg=new jackfft_wavegen();
			wg->builder=builder;
			wg->init();
			Gtk::Button* b;
			builder->get_widget("wavegen_b_import", b);
			b->signal_clicked().connect(sigc::mem_fun(*this,&jackfft_analyzer::b_import_clicked));
		}
		wg->mainWindow->present();
	}
	void b_import_clicked() {
		analyzerListItem* item=new analyzerListItem(wg->fft->size);
		memcpy(item->fft.Data,wg->fft->Data,sizeof(double)*wg->fft->size);
		doAddItem(item);
	}
	void b_import_m_clicked() {
		analyzerListItem* item=new analyzerListItem(m->fft_test->size);
		int periods=m->fft_measure->size/m->fft_test->size;
		for(int i=0;i<item->fft.size_c;i++) {
			auto point=m->fft_measure->Data_c[i*periods];
			auto point2=m->fft_test->Data_c[i];
			double asdfg=point2[0]*point2[0]+point2[1]*point2[1];
			item->fft.Data_c[i][0]=(point[0]*point2[0]+point[1]*point2[1])/asdfg;
			item->fft.Data_c[i][1]=(point[1]*point2[0]-point[0]*point2[1])/asdfg;
		}
		item->freq2time();
		doAddItem(item);
	}
	void b_open_clicked() {
		string tmp;
		if(!prompt_open(builder,mainWindow,tmp)) return;
		CP::File f(tmp.c_str(),O_RDONLY);
		struct stat st;
		if(stat(tmp.c_str(),&st)!=0) {
			alert(builder,mainWindow,"error opening file");
			return;
		}
		int len=st.st_size/sizeof(double);
		analyzerListItem* item=new analyzerListItem(len);
		f.readAll(item->fft.Data,sizeof(double)*len);
		doAddItem(item);
	}
	void rb_timefreq_clicked() {
		Gtk::RadioButton* rb;
		getWidget("rb_time", rb);
		bool td=rb->get_active();
		for(int i=0;i<(int)items.size();i++) {
			if(td) items[i]->drawTimeDomain();
			else items[i]->drawFreqDomain();
			items[i]->queue_draw();
		}
	}
	void doAddItem(analyzerListItem* item, bool update_f=true) {
		item->img.set_events(item->img.get_events()|ENTER_NOTIFY_MASK|LEAVE_NOTIFY_MASK);
		if(update_f) item->time2freq();
		item->property_height_request().set_value(50);
		Gtk::Box* box;
		getWidget("box_main", box);
		box->add(*item);
		item->show_all();
		items.push_back(item);
		Gtk::RadioButton* rb;
		getWidget("rb_time", rb);
		item->img.signal_enter_notify_event().connect([this,item](GdkEventCrossing*){
			item_mouseenter(item);
			return false;
		});
		item->img.signal_leave_notify_event().connect([this,item](GdkEventCrossing*){
			item_mouseexit(item);
			return false;
		});
		if(rb->get_active())
			item->drawTimeDomain();
		else item->drawFreqDomain();
	}
	void doRedrawItem(analyzerListItem* item) {
		Gtk::RadioButton* rb;
		getWidget("rb_time", rb);
		if(rb->get_active())
			item->drawTimeDomain();
		else item->drawFreqDomain();
	}
	void item_mouseenter(analyzerListItem* item) {
		Gtk::Label* l;
		getWidget("l_status", l);
		char tmp[256];
		snprintf(tmp,sizeof(tmp),"Length: %i",item->fft.size);
		l->set_label(tmp);
	}
	void item_mouseexit(analyzerListItem* item) {
		Gtk::Label* l;
		getWidget("l_status", l);
		l->set_label("");
	}
};


int main(int argc, char** argv) {
	srand48(time(NULL));
	
	pthread_mutex_init(&mutex, NULL);
	jack_client_t* client;
	jack_set_error_function(jack_error);
	JackStatus st;
	if ((client = jack_client_open ("jackfft_analyzer",JackNoStartServer,&st)) == 0) {
		fprintf(stderr, "could not connect to server: status %i\n",st); goto sdfgh;
	}
	jack_set_process_callback(client, jack_process, 0);
	jack_on_shutdown(client, jack_shutdown, 0);
	int bs; bs=jack_get_buffer_size(client);
	printf("sample rate: %i\nbuffer size: %i\n", jack_srate=jack_get_sample_rate(client),bs);
	jack_in=jack_port_register(client, "in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	jack_out=jack_port_register(client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	if (jack_activate(client)) fprintf(stderr, "cannot activate client");
sdfgh:
	Gtk::Main kit(argc, argv);
	jackfft_analyzer a;
	a.builder=Gtk::Builder::create_from_file("jackfft_analyzer.ui");
	Gtk::FileChooserDialog* filechooser;
	a.builder->get_widget("filechooser",filechooser);
	//filechooser->add_button(Stock::CANCEL, RESPONSE_CANCEL);
	a.init();
	kit.run(*a.mainWindow);
}

