#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include <errno.h>
#include <stdexcept>
#include <exception>
#include <generic_struct.H>
#include <limits.h>
#include <cpoll.H>
#include <sys/wait.h>
#include <functional>
#include <cout_object.H>
#include <algorithm>
#include <cctype>
#include <locale>
#include "execd.H"

using namespace EXECD;
using namespace std;
using namespace std::placeholders;
using namespace GenericStruct;
using namespace GenericStruct::Aliases;
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}
/*
void onWrite(int8_t* buf, int32_t len) {
	write(1,buf,len);
}
void do_send(O o) {
	EXECD::execd_header h;
	EXECD::tmpbuffer buf;
	//std::function<void(tmpbuffer&,const int8_t*,int32_t)> tmpf=&tmpbuffer::write;
	//o.serialize(bind(tmpf, buf, _1, _2));
	o.serialize([&buf](const int8_t* b,int32_t len){buf.write(b, len);});
	h.obj_len=htonl(buf.length);
	onWrite((int8_t*)&h, sizeof(h));
	onWrite(buf.buf, buf.length);
}*/
class deallocator
{
public:
	function<void()> func;
	deallocator(const function<void()>& func): func(func) {}
	~deallocator() { func(); }
};
void split(const char* s, int len, char delim, const function<void(const char*, int)>& cb) {
	if(len==-1)len=strlen(s);
	const char* s1=s;
	const char* end=s+len;
	while(true) {
		s=strchr(s, delim);
		if(s==NULL) break;
		cb(s1, s-s1);
		s1=++s;
	}
	cb(s1, end-s1);
}

void* do_cpoll_loop(void* v) {
	CP::Poll* p=(CP::Poll*)v;
	try {
		p->loop();
	}catch(...) {}
	p->release();
	return NULL;
}
void start_cpoll_loop(CP::Poll& p) {
	p.retain();
	pthread_t thr;
	pthread_create(&thr,NULL,do_cpoll_loop,(void*)&p);
}

void do_exec_command(execd_client& client, string cmdstr, bool shell=false) {
	A cmdarray(Types::array);
	bool first=true;
	split(cmdstr.c_str(), -1, ' ', [&](const char* s, int len) {
		//cerr << len << endl;
		cmdarray.append(A((const int8_t*)s,len));
		if(first && shell) {
			cmdarray.append(A((const int8_t*)s,len));
			first=false;
		}
	});
	//cerr << cmdarray << endl;
	//do_send(A{O((int8_t)EXECD::cmds::execute),O(++cmdid),O{O(cmdarray),O(++taskid),O(1)}});
	int32_t taskid=client.do_exec(cmdarray, true);
	eprintf("[%i]\n",taskid);
}
void help() {
	eprintf("\x1B[41;1;33mcommands: eXec setReturnoutput Kill sendInput Shell iNteract\n");
	eprintf("exec: x commandname arg0 [arg1 ...]\n");
	eprintf("setreturnoutput: r taskid [1/0]\n");
	eprintf("kill: k taskid signal\n");
	eprintf("sendinput: i taskid string\n");
	eprintf("shell: s\n");
	eprintf("interact: n taskid\x1B[0;0;0m\n");
}
int main() {
	execd_client client;
	/*split("asd ghj zxc  d x", -1, ' ', [](const char* s, int len) {
		cerr << string(s,len) << endl;
	});
	return 0;*/
	
	
	
	//spawn execd
	
	
	int _i[2]; int _o[2];
	checkError(pipe(_i));
	checkError(pipe(_o));
	int ret=fork();
	
	
	if(ret==0) {
		dup2(_i[0], 0);
		dup2(_o[1], 1);
		close(_i[1]);
		close(_o[0]);
		close(_i[0]);
		close(_o[1]);
		
		//method 1: directly spawn on local machine
		//*
		execlp("./execd", "execd",NULL);
		//*/
		
		//method 2: ssh
		/*
		const char* sshserver="user1@192.168.5.11";
		execlp("ssh","ssh",sshserver,"execd",NULL);
		//*/
		
		perror("execlp");
		_exit(1);
	}
	if(ret<0) return 1;
	close(_i[0]);
	close(_o[1]); //*/
	
	
	
	
	
	
	
	client.onWrite=[_i](const int8_t* data, int32_t len) {
		write(_i[1], data, len);
	};
	client.onEvent=[](O o) {
		cerr << "\x1B[41;1;33mevent: " << o << "\x1B[0;0;0m" << endl;
	};
	CP::Poll p;
	CP::File gsdfhkjgh(_o[0],true);
	client.start(p, gsdfhkjgh);
	start_cpoll_loop(p);
	
	help();
	bool shell=false;
	while(true) {
		if(shell)
			eprintf("\x1B[1;1;1mexecd sh # \x1B[0;0;0m");
		else
			eprintf("\x1B[1;1;1mexecd # \x1B[0;0;0m");
		fflush(stdout);
		char* line=NULL;
		size_t n=0;
		ssize_t len=getline(&line, &n, stdin);
		deallocator d([line](){ if(line!=NULL)free(line); });
		if(len<=0) {
			if(shell) {
				shell=false;
				eprintf("\n");
				continue;
			}
			break;
		}
		string cmdstr;
		if(shell) {
			cmdstr=string(line);
			trim(cmdstr);
			if(cmdstr.length()<=0) continue;
			do_exec_command(client, cmdstr, true);
			continue;
		}
		
		char* tmp1=strchr(line, ' ');
		
		if(tmp1!=NULL) {
			//tmp1++;
			if(tmp1>=(line+len)) continue;
			cmdstr=string(tmp1);
			trim(cmdstr);
		}
		
		switch(*line) {
			case 'x':
			{
				if(tmp1==NULL) break;
				//cerr << cmdstr << endl;
				do_exec_command(client, cmdstr);
				break;
			}
			case 'r':
			{
				if(tmp1==NULL) break;
				break;
			}
			case 'k':
			{
				if(tmp1==NULL) break;
				const char* tmp2=cmdstr.c_str();
				const char* tmp3=strchr(tmp2, ' ');
				string taskid(tmp2, tmp3-tmp2);
				int32_t sig=atoi(++tmp3);
				client.do_kill(atoi(taskid.c_str()), sig);
				break;
			}
			case 'i':
			{
				if(tmp1==NULL) break;
				cmdstr+="\n";
				const char* tmp2=cmdstr.c_str();
				const char* tmp3=strchr(tmp2, ' ');
				if(tmp3==NULL)continue;
				string taskid(tmp2, tmp3-tmp2);
				A data(++tmp3);
				//data.append((int8_t)'\n');
				client.do_sendInput(atoi(taskid.c_str()),data);
				break;
			}
			case 'n':
			{
				if(tmp1==NULL) break;
				int32_t taskid(atoi(cmdstr.c_str()));
				while(true) {
					int8_t buf[4096];
					int br=read(0,buf,4096);
					if(br<=0) {
						eprintf("\n");
						break;
					}
					A data(buf, br);
					client.do_sendInput(taskid,data);
				}
				break;
			}
			case 's':
				shell=true;
				break;
			case '\n':
				break;
			default:
				help();
				break;
		}
	}
	eprintf("\n");
	//do_send(A{O((int8_t)EXECD::cmds::execute),O(123),O{O{O("ls"),O("ls")},O(234),O(1)}});
	//do_send(A{O((int8_t)EXECD::cmds::execute),O(124),O{O{O("pwd"),O("pwd")},O(235),O(1)}});
}
