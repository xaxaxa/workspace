#include <cpoll/statemachines.H>
#include <cpoll/cpoll.H>
#include <unistd.h>
#include <cpoll/sendfd.H>
#include <iostream>
#include <socketd.H>
#include <signal.h>
#include <socketd_client.H>
#include <cppsp/page.H>
#include <cppsp/cppsp_cpoll.H>
#include <cppsp/common.H>

using namespace std;
using namespace socketd;
using namespace CP;
using namespace cppsp;
string rootDir;
class socketd_Server: public cppsp::Server {
	virtual const char* rootDir() {
		return ::rootDir.c_str();
	}
};
socketd_Server _server;
void parseArgs(int argc, char** argv, const function<void(char*, const function<char*()>&)>& cb) {
	int i = 1;
	function<char*()> func = [&]()->char*
	{
		if(i+1>=argc)return NULL;
		return argv[(++i)];
	};
	for (; i < argc; i++) {
		if (argv[i][0] == '\x00') continue;
		if (argv[i][0] == '-') {
			cb(argv[i] + 1, func);
		} else {
			cb(NULL, [argv,i]()
			{	return argv[i];});
		}
	}
}
int main(int argc, char** argv) {
	cout << "started child #" << getpid() << endl;
	{
		char cwd[255];
		if(getcwd(cwd,255)==NULL) throw runtime_error(strerror(errno));
		rootDir=cwd;
	}
	parseArgs(argc, argv,
			[&](char* name, const std::function<char*()>& getvalue)
			{
				if(strcmp(name,"r")==0) {
					rootDir=getvalue();
				} else if(strcmp(name,"c")==0) {
					CXXOpts().push_back(getvalue());
				}
			});
	
	//CP::Socket s(3, AF_UNIX, SOCK_STREAM, 0);
	//protocolHeader ph;
	int reqs=0;
	CP::Poll p;
	struct {
		int& reqs;
		CP::Poll& p;
		void operator()(socketd_client& cl, Socket* s, int64_t id) {
			if(s==NULL)kill(getpid(),9);
			/*if((++reqs)>500) {
				//recycle
				exit(0);
			}*/
			cl.ack(id);
			//printf("handle: %i\n",s->handle);
			
			struct handler:public RGC::Object {
				CP::Poll& p;
				RGC::Ref<Socket> s;
				cppsp::CPollRequest req;
				cppsp::Response resp;
				//Page* p;
				//MemoryStream ms;
				uint8_t buf[4096];
				string path;
				handler(CP::Poll& p,Socket* s):p(p),s(s),req(*s),resp(*s) {
					//printf("handler()\n");
					this->retain(2);
					req.readHeaders({&handler::readCB,this});
				}
				void readCB() {
					//printf("readCB()\n");
					char tmp[req.path.length()+rootDir.length()];
					int l=cppsp::combinePathChroot(rootDir.c_str(),req.path.c_str(),tmp);
					path=string(tmp,l);
					cppsp::loadPage(p,rootDir,path,{&handler::loadCB,this});
				}
				
				void loadCB(Page* p, exception* ex) {
					//printf("loadCB()\n");
					if(ex!=NULL) {
						cppsp::handleError(ex,resp,path);
						resp.flush( { &handler::flushCB, this });
						goto doFinish;
					}
					{
						//this->p=p;
						p->filePath=path;
						p->request=&req;
						p->response=&resp;
						p->poll=&this->p;
						p->server=&_server;
						p->handleRequest({&handler::handleRequestCB,this});
						return;
					}
				doFinish:
					//s->write(ms.data(),ms.length(),{&handler::writeCB,this});
					s->repeatRead(buf,sizeof(buf),{&handler::sockReadCB,this});
				}
				void sockReadCB(int r) {
					if(r<=0) release();
				}
				void flushCB(Response& resp) {
					s->shutdown(SHUT_WR);
					release();
				}
				void handleRequestCB(Page& p) {
					p.release();
					s->shutdown(SHUT_WR);
					release();
					s->repeatRead(buf,sizeof(buf),{&handler::sockReadCB,this});
				}
				~handler() {
					//printf("~handler()\n");
				}
			}* hdlr=new handler(p,s);
			hdlr->release();
		}
	} cb {reqs, p};
	socketd_client cl(p, &cb);
	p.loop();
}
