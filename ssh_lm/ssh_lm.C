#include <gtkmm.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
using namespace Gtk;
using namespace std;

Glib::RefPtr<Gtk::Builder> builder;
bool default_server=false;
void set_default(int screen) {
	Gtk::Window* w;
	
	switch(screen) {
		case 1: //login
		{
			builder->get_widget("window1", w);
			Gtk::Button* butt;
			builder->get_widget("b_login", butt);
			w->set_default(*butt);
			Gtk::Entry* t;
			builder->get_widget(default_server?"t_user":"t_server", t);
			t->grab_focus();
			break;
		}
		/*case 2: //log
			builder->get_widget("b_relogin", butt);
			butt->grab_focus();
			break;*/
	}
}

void display_log() {
	Gtk::Window* w;
	builder->get_widget("window1", w);
	Gtk::Table* tb;
	builder->get_widget("tb_log", tb);
	w->remove();
	w->add(*tb);
	Gtk::Button* butt;
	builder->get_widget("b_relogin", butt);
	butt->set_sensitive(false);
	Gtk::Label* lb;
	builder->get_widget("l_title1", lb);
	lb->set_text("Logging in...");
	set_default(2);
}
void show_log() {
	Gtk::TextView* tv;
	builder->get_widget("t_log", tv);
	Glib::RefPtr< TextBuffer > tbuf=tv->get_buffer();
	tbuf->set_text("");
	display_log();
}

void display_login() {
	Gtk::Window* w;
	builder->get_widget("window1", w);
	Gtk::Table* tb;
	builder->get_widget("tb_login", tb);
	w->remove();
	w->add(*tb);
	tb->set_sensitive(true);
	set_default(1);
	
}
void login_fail() {
	Gtk::Button* butt;
	builder->get_widget("b_relogin", butt);
	butt->set_sensitive(true);
	Gtk::Label* lb;
	builder->get_widget("l_title1", lb);
	lb->set_text("Login failed");
	butt->grab_focus();
}
void append_msg(const char* msg, int len=-1) {
	if(len==-1)len=strlen(msg);
	if(len<=0)return;
	Gtk::TextView* tv;
	builder->get_widget("t_log", tv);
	Glib::RefPtr< TextBuffer > tbuf=tv->get_buffer();
	tbuf->insert(tbuf->end(), Glib::ustring(msg, len));
}
void* thr1(void* V) {
	gdk_threads_enter();
	string srv, port, usr, psw;
	Gtk::Entry* t;
	builder->get_widget("t_server", t);
	srv=t->get_text();
	builder->get_widget("t_port", t);
	port=t->get_text();
	builder->get_widget("t_user", t);
	usr=t->get_text();
	builder->get_widget("t_pass", t);
	psw=t->get_text();
	Gtk::TextView* tv;
	builder->get_widget("t_log", tv);
	Glib::RefPtr< TextBuffer > tbuf=tv->get_buffer();
	show_log();
	gdk_threads_leave();
	int p[2];
	pipe(p);
	pid_t pid=fork();
	
	if(pid==0) {
		close(p[0]);
		dup2(p[1],1);
		dup2(p[1],2);
		close(p[1]);
		execl("./try_login", "try_login",
			srv.c_str(), port.c_str(), usr.c_str(), psw.c_str(), NULL);
		perror("execl");
		exit(2);
	}
	close(p[1]);
	if(pid>0) {
		char buf[4096];
		int br;
		while((br=read(p[0], buf, 4096))>0) {
			gdk_threads_enter();
			tbuf->insert(tbuf->end(), Glib::ustring(buf, br));
			gdk_threads_leave();
		}
		close(p[0]);
		int ret;
		do {
			waitpid(pid,&ret,0);
		} while(!WIFEXITED(ret));
		ret=WEXITSTATUS(ret);
		if(ret!=0) {
			stringstream ss;
			ss << "script returned " << ret << endl;
			gdk_threads_enter();
			append_msg(ss.str().c_str());
			login_fail();
			gdk_threads_leave();
		} else {
			execl("./run_user_session", "run_user_session",
				srv.c_str(), usr.c_str(), NULL);
		}
	} else {
		close(p[0]);
		const char* err=strerror(errno);
		stringstream ss;
		ss << "fork(): " << err << endl;
		append_msg(ss.str().c_str());
		gdk_threads_enter();
		login_fail();
		gdk_threads_leave();
	}
		
}
int main(int argc, char** argv)
{
	//usage: ssh_lm default_server
	
	Gtk::Main kit(argc, argv);
	builder = Gtk::Builder::create_from_file("main.ui");
	Gtk::Window* w;
	builder->get_widget("window1", w);
	Gtk::Button* butt;
	builder->get_widget("b_login", butt);
	
	Pango::FontDescription font;
	font.set_size(7*PANGO_SCALE);
	Gtk::TextView* tv;
	builder->get_widget("t_log", tv);
	tv->modify_font(font);
	
	butt->signal_clicked().connect([]() {
		Gtk::Table* table;
		builder->get_widget("tb_login", table);
		table->set_sensitive(false);
		//start thread
		pthread_t thr;
		pthread_create(&thr,NULL,thr1,(void*)NULL);
	});
	
	
	builder->get_widget("b_relogin", butt);
	butt->signal_clicked().connect([]() {
		display_login();
	});
	
	if(argc>1) {
		default_server=true;
		Gtk::Entry* t;
		builder->get_widget("t_server", t);
		t->set_text(argv[1]);
	}
	set_default(1);
	
	g_thread_init (NULL);
	gdk_threads_init();
	kit.run(*w);
	return 0;
}

