#include <cppsp/mountpointmgr.H>
#include <cpoll/cpoll.H>
#include <cppsp/split.H>
using namespace cppsp;
using namespace CP;
struct asdfg: public RGC::Object
{
	string s;
};
int main() {
	StandardStream ss;
	Poll p;
	ss.addToPoll(p);
	StreamReader sr(ss);
	struct CB {
		StandardStream& ss;
		StreamReader& sr;
		MountPointMgr mgr;
		void start() {
			ss.write("$ ",2);
			sr.readLine({&CB::linecb,this});
		}
		void linecb(const string& s) {
			if(s.length()<=0) goto cont;
			{
				split spl(s.data(),s.length(),' ');
				if(spl.read()) {
					if(spl.value=="mount") {
						if(!spl.read()) {
							printf("usage: mount MOUNTPOINT\n");
							goto cont;
						}
						asdfg* a=new asdfg();
						a->s=spl.value.toSTDString();
						bool b=mgr.mount(spl.value,a);
						printf("%s\n",b?"true":"false");
						if(!b) delete a;
					} else if(spl.value=="umount") {
						if(!spl.read()) {
							printf("usage: umount MOUNTPOINT\n");
							goto cont;
						}
						Object* a=mgr.umount(spl.value);
						if(a!=NULL) delete a;
					} else if(spl.value=="find") {
						if(!spl.read()) {
							printf("usage: find PATH\n");
							goto cont;
						}
						Object* a=mgr.find(spl.value);
						if(a==NULL) printf("NULL\n");
						else {
							asdfg* a1=(asdfg*)a;
							printf("%s\n",a1->s.c_str());
						}
					} else if(spl.value=="ls") {
						StreamWriter sw(ss);
						mgr.print(sw);
					} else {
						printf("commands:\n - mount\n - umount\n - find\n - ls\n");
					}
				}
			}
		cont:
			if(!sr.eof)start();
		}
	} cb {ss,sr};
	cb.start();
	p.loop();
}
