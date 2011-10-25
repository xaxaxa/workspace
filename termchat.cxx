#include <ncursesw/ncurses.h>
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <cplib.hpp>
#include <asyncsock.hpp>
#include <stdbool.h>
#include <boost/shared_ptr.hpp>
#include <list>
#include <stdio.h>
#include <sstream>

//using namespace boost;
using namespace xaxaxa;
using namespace xaxaxa::Sockets;

#define sptr boost::shared_ptr


struct mrevent {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool triggered;
};
void mrevent_init(struct mrevent *ev) {
    pthread_mutex_init(&ev->mutex, 0);
    pthread_cond_init(&ev->cond, 0);
    ev->triggered = false;
}
void mrevent_trigger(struct mrevent *ev) {
    pthread_mutex_lock(&ev->mutex);
    ev->triggered = true;
    pthread_cond_signal(&ev->cond);
    pthread_mutex_unlock(&ev->mutex);
}
void mrevent_reset(struct mrevent *ev) {
    pthread_mutex_lock(&ev->mutex);
    ev->triggered = false;
    pthread_mutex_unlock(&ev->mutex);
}
void mrevent_wait(struct mrevent *ev) {
     pthread_mutex_lock(&ev->mutex);
     while (!ev->triggered)
         pthread_cond_wait(&ev->cond, &ev->mutex);
     pthread_mutex_unlock(&ev->mutex);
}
using namespace std;

#define def_ip "127.0.0.1"
#define def_port 12369
#define def_name "termchat"

mrevent evt;
void* guithread(void* v);
WINDOW *win;
SocketManager* sm;
struct client;
list<client> clients;
Buffer databuf(1<<16);//64K buffer
const long databuf_mask=(1<<16)-1;
long databuf_position=0;
int min(int a, int b)
{
	return a>b?b:a;
}
void databuf_append(const Buffer& b)
{
	//memcpy(databuf.buf+(databuf_position&databuf_mask),b.buf,b.length);
	int pos=(int)(databuf_position&databuf_mask);
	memcpy(databuf.buf+pos,b.buf,min(b.length,databuf.length-pos));
	if(b.length>databuf.length-pos)
		memcpy(databuf.buf,b.buf+(databuf.length-pos),b.length-(databuf.length-pos));
	databuf_position+=b.length;
}
struct client
{
	Socket s;
	int uid;
	sptr<xaxaxa::Stream> stream;
	Buffer readbuf;
	list<client>::iterator iter;
	int write_pos;
	bool writing;
	int written_bytes;

	FUNCTION_DECLWRAPPER(cb_write,void,Stream* s)
	{
		client* c=(client*)obj;
		c->writing=false;
		try
		{
			c->stream->EndWrite();
			c->write_pos+=c->written_bytes;
			//cout << "client " << c->s._s << ": written " << c->written_bytes << " bytes; write_pos=" << c->write_pos << endl;
			c->beginwrite();
		}
		catch(Exception& ex)
		{
			//cout << "closing client " << c->s._s << endl;
			s->Close();
			clients.erase(c->iter);
		}
	}
	void beginwrite()
	{
		if(writing)return;
		if(write_pos<databuf_position-databuf.length) //the client was too slow; data have to be discarded
			write_pos=databuf_position-databuf.length;
		if(write_pos>=databuf_position)return; //there's no data to be written
		writing=true;
		int pos=(int)(write_pos&databuf_mask);
		written_bytes=min(databuf.length-pos,databuf_position-write_pos);
		stream->BeginWrite(databuf.SubBuffer(pos,written_bytes),Stream::Callback(cb_write,this));
	}
	FUNCTION_DECLWRAPPER(cb_read,void,Stream* s)
	{
		client* c=(client*)obj;
		try
		{
			int i=s->EndRead();
			if(i>0)
			{
				stringstream str;
				str << c->uid << endl;
				string s=str.str();
				databuf_append(Buffer((char*)s.c_str(),s.length()));
				databuf_append(c->readbuf.SubBuffer(0,i));
				list<client>::iterator it;
				for(it=clients.begin();it!=clients.end();it++)
				{
					(*it).beginwrite();
				}
				c->stream->BeginRead(c->readbuf,Stream::Callback(cb_read,c));
			}
			else goto err;
		}
		catch(Exception& ex)
		{
			goto err;
		}
		return;
		err:
		s->Close();
		//cout << "closing client " << c->s._s << endl;
		clients.erase(c->iter);
	}
	client():
	readbuf(4096)
	{
	}
	void init(Socket s)
	{
		this->s=s;
		this->stream=sptr<Stream>(new SocketStream(s,sm));
		try
		{
			writing=false;
			written_bytes=0;
			write_pos=databuf_position-512;//send 512 bytes of conversation history
			if(write_pos<0)write_pos=0;
			stream->BeginRead(readbuf,Stream::Callback(cb_read,this));
			beginwrite();
		}
		catch(Exception& ex)
		{
			this->stream->Close();
			//cout << "closing client " << c->s._s << endl;
			clients.erase(iter);
		}
	}
	
};


FUNCTION_DECLWRAPPER(cb_accept,void,SocketManager* m,Socket sock)
{
	Socket s=m->EndAccept(sock);
	m->BeginAccept(sock,SocketManager::Callback(cb_accept,NULL));
	client c;
	list<client>::iterator iter=clients.insert(clients.end(),c);
	
	ucred credentials;
	socklen_t ucred_length = sizeof(ucred);
	/* fill in the user data structure */
	if(getsockopt(s._s, SOL_SOCKET, SO_PEERCRED, &credentials, &ucred_length))
	{
		printf("could obtain credentials from unix domain socket");
		(*(iter)).uid=-1;
	}
	else (*(iter)).uid=credentials.uid;
	//cout << "client accepted" << endl;

	(*(iter)).iter=iter;
	(*(iter)).init(s);
}
Socket sock(AF_UNIX,SOCK_STREAM,0);
int main(int argc,char** argv)
{
	signal(SIGCHLD,SIG_IGN);
	const char* name=def_name;
	if(argc<2)goto asdf;
	name=argv[1];
asdf:
	setlocale(LC_ALL,"");
	int namelen=strlen(name);
	char name1[strlen(name)+1];
	memcpy(name1+1,name,namelen);
	name1[0]='\n';
	{
		int tmp=fork();
		if(tmp==0)
		{//child
			//supress stdout
			int out_fd=open("/dev/null",O_RDWR);
			dup2(out_fd,0);
			dup2(out_fd,1);
			dup2(out_fd,2);
			setsid();
			try{
				Socket sock(AF_UNIX,SOCK_STREAM,0);
				//IPEndPoint ep(IPAddress("0.0.0.0"),port);
				UNIXEndPoint ep(name1);
				int tmp12345=1;
				setsockopt(sock._s,SOL_SOCKET,SO_REUSEADDR,&tmp12345,sizeof(tmp12345));
				sock.Bind(&ep);
				sock.Listen(10);
				SocketManager m;
				sm=&m;
				m.BeginAccept(sock,SocketManager::Callback(cb_accept,NULL));
				m.EventLoop();
			}catch(Exception ex){}
			exit(0);
		}
	}
	int w,h;
	getmaxyx(stdscr, h, w);
	//IPEndPoint ep(IPAddress(ip),port);
	UNIXEndPoint ep(name1);
	{
		int aaa;
		for(aaa=0;aaa<3;aaa++)
		{
			try
			{sock.Connect(&ep);goto zxcv;}
			catch(Exception& ex)
			{}
			sleep(1);
		}
		cout << "could not connect after 3 attempts" << endl;
		return 1;
	}
	zxcv:
	mrevent_init(&evt);
	pthread_t thread1;
	if(pthread_create( &thread1, NULL, &guithread,NULL)!=0)
	{
		perror("could not create thread;");exit(1);
	}
	signal(SIGWINCH,SIG_IGN);
	mrevent_wait(&evt);
	//cerr << "starting network thread" << endl;
	Buffer buf(4096);
	try
	{
		while(1)
		{
			int br=sock.Recv(buf.SubBuffer(0,buf.length-1));
			((char*)buf.buf)[br]='\0';
			//cerr << (char*)buf.buf;
			//wprintw(win,"%s",buf.buf);
			waddnstr(win,buf.buf,br);
			wrefresh(win);
			//move(h-1,0);
			//refresh();
		}
	}
	catch(Exception& ex)
	{
		perror("connection closed");
		exit(0);
	}
	return 0;
}

void* guithread(void* v)
{

	initscr();			/* Start curses mode 		  */
	int x, y, w, h;
	x=0;y=0;
	getmaxyx(stdscr, h, w);
	refresh();
	win=newwin(h, w, y, x);
	scrollok(win,true);
	//wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	//box(win, 0 , 0);
	//wprintw(win,"fuck\nasdf\n\n\ngdfgdf\ngdf\ngfgd\ngfdgdfgdf\nfsd\n\ngfd\nghdfhg\ngdf\n");
	wrefresh(win);
	
	//refresh();			/* Print it on to the real screen */
	//getch();			/* Wait for user input */
	//int l=0;
	//int a=0;
	string s;
	mrevent_trigger(&evt);
	char str[1024];
	while(1)
	{
		getmaxyx(stdscr, h, w);
		//string s;
		//getline(cin,s);
		//wprintw(win,s.c_str());
		//wrefresh(win);
		//printw(s.c_str());
		//refresh();
		//cout <<s;
		move(h-1,0);
		//str[1022]='\0';
		//str[1023]='\0';
		/*char c=getch();
		if(c==10)
		{
			//wprintw(win,s.c_str());
			s+="\n";
			l++;
			//wscrl(win,1);
			//wprintw(win,"%s",s.c_str());
			Buffer b((char*)(s.c_str()),s.length());
			int bs=0;
			while(bs<b.length)
				bs+=sock.Send(b.SubBuffer(bs,b.length-bs));
			//mvwprintw(win,LINES-2,0,s.c_str());
			//wrefresh(win);
			a=0;
			s="";
			//move(LINES-1,a);
			clrtoeol();
		}
		else
		{
			s.append(1,c);
			a++;
		}*/
		
		if(getnstr(str,1023)==ERR)
		{
			wprintw(win,"ERROR\n");
			wrefresh(win);
			continue;
		}
		int len=strlen(str);
		if(len<=0)continue;
		Buffer b((char*)str,len);
		str[b.length]='\n';
		b.length++;
		int bs=0;
		while(bs<b.length)
			bs+=sock.Send(b.SubBuffer(bs,b.length-bs));
		clrtoeol();
	}
	
	endwin();			/* End curses mode		  */
	return NULL;
}
