#include <cppsp/common.H>
#include <cpoll/cpoll.H>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <string>

using namespace CP;
using namespace std;
int main(int argc, char** argv) {
	StandardStream ss;
	if(argc<2) {
		printf("usage: %s file.cppsp\n",argv[0]);
		return 1;
	}
	string path;
	{
		char cwd[256];
		if(getcwd(cwd,255)==NULL) throw runtime_error(strerror(errno));
		char tmp[strlen(cwd)+strlen(argv[1])+1];
		int tmpl=strlen(cwd);
		cwd[tmpl]='/';
		cwd[tmpl+1]='\0';
		int l=cppsp::combinePath(cwd,argv[1],tmp);
		path=string(tmp,l);
	}
	/*File inp(open(argv[1],O_RDONLY));
	File out_c(open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0666));
	File out_s(open(argv[3],O_RDWR|O_CREAT|O_TRUNC,0666));
	
	MemoryStream ms;
	//
	inp.readToEnd(ms);
	ms.flush();
	
	MemoryStream ms2;
	MemoryStream ms3;
	cppsp::doParse("cppsp_cls_test1_cppsp", (const char*)ms.data(), ms.length(), out_c, out_s);*/
	
	
	
	//*
	CP::File* f=cppsp::compilePage(path);
	if(f==NULL) throw runtime_error(strerror(errno));
	CP::StreamReader sr(*f);
	while(true) {
		string s=sr.readLine();
		cout << s << endl;
		if(sr.eof)return 0;
	}
	//*/
	
	/*
	char tmp[strlen(argv[1])+strlen(argv[2])];
	int i=cppsp::combinePathChroot(argv[1],argv[2],tmp);
	ss.write(tmp,i);
	ss.write("\n",1);
	*/
	
	/*
	StreamWriter sw(ss);
	sw.write("================C++===================\n");
	sw.flush();
	ss.write(ms2.data(),ms2.length());
	sw.writeF("\n\n\n\n\n================string table================%i\n",ms3.length());
	sw.flush();
	ss.write(ms3.data(),ms3.length());*/
}
