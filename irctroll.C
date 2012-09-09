#include <iostream>
#include <ncurses.h>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>

#define NICK "hussain12"
#define USER "hussain12"
#define RNAME "hussain"
#define SERV "irc.freenode.net"
#define PORT 8001

using namespace xaxaxa;
using namespace Sockets;

WINDOW *win;
Socket sock(AF_INET,SOCK_STREAM,0);
SocketStream str(sock);
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
	char stri[1024];
	while(1)
	{
		getmaxyx(stdscr, h, w);
		move(h-1,0);
		
		if(getnstr(stri,1023)==ERR)
		{
			wprintw(win,"ERROR\n");
			wrefresh(win);
			continue;
		}
		int len=strlen(stri);
		if(len<=0)continue;
		Buffer b((char*)stri,len);
		stri[b.Length]='\n';
		b.Length++;
		
		str.BeginWrite(b, function<void(void*,Stream*)>([&](void* asdf,Stream* sdfg) {
			str.EndWrite();
		}));
		
		
		clrtoeol();
	}
	
	endwin();			/* End curses mode		  */
	return NULL;
}


int main()
{
	int itr=0;
	signal(SIGPIPE, SIG_IGN);
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart functions if	 interrupted by handler */
	sigaction(SIGHUP, &sa, NULL);
	
	
	sock.Connect(IPEndPoint{"89.16.176.16",PORT});
	StandardStream stds;
	Buffer b(4096);
	
	function<void(void*,Stream*)> cb1=[&](void* asdf,Stream* sdfg) {
		int br=str.EndRead();
		stds.Write(b.SubBuffer(0,br));
		str.BeginRead(b,cb1);
	};
	str.BeginRead(b,cb1);
	
	stringstream ss;
	string serv=SERV;
	ss << "CAP LS\r\nNICK " << NICK << "\r\nUSER "<< USER << " " << USER << " " << "irc.freenode.org" << " :" << RNAME << "\r\n" << "CAP END\r\n";
	string s=ss.str();
	
	str.BeginWrite(Buffer(s.c_str(),s.length()), function<void(void*,Stream*)>([&](void* asdf,Stream* sdfg) {
		str.EndWrite();
	}));
	
	SocketManager* m=SocketManager::GetDefault();
	/*Buffer inbuf(4096);
	Socket stdsock(0);
	SocketManager::Callback stdcb([&](void* v, SocketManager* m1, Socket sock) {
		int i=m->EndRead(stdsock);
		if(i<=0)exit(0);
		str.BeginWrite(inbuf.SubBuffer(i), Stream::Callback([&](void* asdf,Stream* sdfg) {
			str.EndWrite();
			//cout << m << endl;
			m->BeginRead(stdsock,inbuf,stdcb);
		}));
		
	});*/
	//m->BeginRead(stdsock,inbuf,stdcb);
	
	//start gui thread
	//pthread_t thread;
	//pthread_create(&thread,NULL,guithread,NULL);

	
	
	m->EventLoop();
}
