#define IS_UI
#include <gtkmm.h>
#include <gdk/gdk.h>
#include <generic_ui.H>
#include <cplib/cplib.hpp>
#include "fbdump_ui.H"
using namespace GenericUI::Aliases;
using GenericStruct::Types;
void init()
{
	//printf("sssssssssssss\n");
	GenericUI::attach("main").attach("checkUI").attach(
	[](O o) {
		return O(1);
	});
	GenericUI::attach("main").attach("log").attach(
	[](O o) {
		if(o.type==Types::array && o.getArray().length()>0 && o.getArray()[0].type==Types::array)
			fprintf(stderr, "%s\n", o.getArray()[0].getString());
		return O::undefined();
	});
	GenericUI::attach("main").attach("prompt").attach(
	[](O o) {
		if(o.type==Types::array && o.getArray().length()>0 && o.getArray()[0].type==Types::array)
			fprintf(stderr, "\x1B[41;1;33m%s\x1B[0;0;0m\n", o.getArray()[0].getString());
		char* line=NULL;
		size_t len=0;
		int i=getline(&line,&len,stdin);
		if(i<0) return O::undefined();
		O tmp(line);
		free(line);
		return tmp;
	});
	GenericUI::attach("main").attach("run").attach([](O o) {
		int argc=o.getArray()[0].getInt32();
		A argv=o.getArray()[1].getArray();
		if(argc<2) {
			cout << "usage: " << argv[0].getString() << 
				" -a access_token [-o JS_CODE]... [-c concurrency] filename.js [filename2.js ...]" << endl;
			return O(1);
		}
		
		xaxaxa::StringBuilder sb;
		int concurrency=3;
		A scripts(Types::array);
		A header;
		for(int i=1;i<argc;i++)
			if(strcmp(argv[i].getString(),"-o")==0 && (i+1)<argc) {
				sb.Append(argv[i+1].getString());
				sb.Append(";\n");
				i++;
			} else if(strcmp(argv[i].getString(),"-a")==0 && (i+1)<argc) {
				sb.Append("a=\"");
				sb.Append(argv[i+1].getString());
				sb.Append("\";");
				i++;
			} else if(strcmp(argv[i].getString(),"-c")==0 && (i+1)<argc) {
				concurrency=atoi(argv[i+1].getString());
				i++;
			} else {
				scripts.append(argv[i]);
			}
		const char* env_vars=getenv("FBDUMP_VARS");
		if(env_vars!=NULL) {
			sb.Append(env_vars);
			sb.Append(";\n");
		}

		
		header=sb.ToCString();
		GenericUI::attach("main").attach("finish").attach([](O o) {
			exit(0);
			return O::undefined();
		});
		GenericUI::attach("main").attach("start").invoke({O(header), O(scripts), O(concurrency)});
		while(1)sleep(3600);
		//delete a1;
		return O(0);
	});
}
