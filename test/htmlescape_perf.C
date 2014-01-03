#include <cpoll/cpoll.H>
#include <stdio.h>
using namespace CP;
void htmlEscape1(const char* in, int inLen, CP::StreamWriter& sw) {
	int sz=0;
	for (int i = 0; i < inLen; i++) {
		switch (in[i]) {
			case '&':
				sz+=5;
				break;
			case '<':
				sz+=4;
				break;
			case '>':
				sz+=4;
				break;
			default:
				sz++;
				break;
		}
	}
	
	char* data=sw.beginWrite(sz);
	char* c=data;
	for (int i = 0; i < inLen; i++) {
		switch (in[i]) {
			case '&':
				c[0]='&'; c[1]='a'; c[2]='m'; c[3]='p'; c[4]=';';
				c+=5;
				break;
			case '<':
				c[0]='&'; c[1]='l'; c[2]='t'; c[3]=';';
				c+=4;
				break;
			case '>':
				c[0]='&'; c[1]='g'; c[2]='t'; c[3]=';';
				c+=4;
				break;
			default:
				*(c++)=in[i];
		}
	}
	sw.endWrite(sz);
}

/// At p inserts the proper encoding of c, and returns the new string cursor (end of inserted symbols).
char *onion_html_add_enc(char c, char *p){
	switch(c){
		case '<':
			*p++='&';
			*p++='l';
			*p++='t';
			*p++=';';
			break;
		case '>':
			*p++='&';
			*p++='g';
			*p++='t';
			*p++=';';
			break;
		case '"':
			*p++='&';
			*p++='q';
			*p++='u';
			*p++='o';
			*p++='t';
			*p++=';';
			break;
		default:
			*p++=c;
	}
	return p;
}

/// Returns the encoding for minimal set
int onion_html_encoding_size(char c){
	switch(c){
		case '<':
			return 4;
		case '>':
			return 4;
		case '"':
			return 6;
	}
	return 1;
}

void htmlEscape2(const char* in, CP::StreamWriter& sw) {
	/// first calculate size
	const char *p=in;
	int size=0;
	while( (*p) ){
		size+=onion_html_encoding_size(*p);
		p++;
	}
	p=in;
	char* ret=sw.beginWrite(size);
	char *t=ret;
	while( (*p) ){
		t=onion_html_add_enc(*p, t);
		p++;
	}
	sw.endWrite(size);
}
int main(int argc, char** argv) {
	if(argc<3) {
		printf("usage: %s funcN stringN tests\n",argv[0]);
		return 1;
	}
	const char* s1="gsfhdjkghjkarhfsdfgsdkgfdshjdsfgkhjsfdgghfsdjkghjksd"
	"gsjfdhkghfd<><GFDgfdg<gfdshfgd<gfdsg>gfhsgfdghjggfdjskg<gfsfhsjdgk"
	"gsfdhjkghsf><ghsfdgkjfds<gfsgs>ghfsdhgf>gfsdghsjkfd<gsfdg>gfsdg>gfsd>";
	const char* s2="ghfsdjkg<gfdsg>gfsdgj</gfsdhj>gsfhdjk<";
	const char* s=atoi(argv[2])==2?s2:s1;
	int n=atoi(argv[3]);
	MemoryStream ms;
	StreamWriter sw(ms);
	switch(atoi(argv[1])) {
		case 1:
			for(int i=0;i<n;i++) htmlEscape1(s,strlen(s),sw);
			break;
		case 2:
			for(int i=0;i<n;i++) htmlEscape2(s,sw);
			break;
	}
}
