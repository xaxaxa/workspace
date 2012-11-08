#include <iostream>
#include <string>
#include <string.h>
#include <v8.h>
#include <map>
//#include <urdl/read_stream.hpp>
#include <functional>
#include <cplib/cplib.hpp>
#include <vector>
#include "curl_httpreq.H"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <generic_struct.H>
#include <generic_ui.H>
#include "fbdump_ui.H"
//#include <urdl.cpp>

using namespace v8;
using namespace std;
using namespace GenericUI::Aliases;
using GenericStruct::Types;


namespace lib
{
	static inline Handle<Data> getfunc(InvocationCallback func)
	{
		return FunctionTemplate::New(func);
	}
	static inline Handle<ObjectTemplate> gettemplate(const map<const char*,Handle<Data> >& objs)
	{
		Handle<ObjectTemplate> t=ObjectTemplate::New();
		for(auto it(objs.begin());it!=objs.end();it++)
			t->Set(String::New((*it).first),(*it).second);
		return t;
	}
};
curl::transferManager* tm;
namespace l
{
	using namespace lib;
	//boost::asio::io_service io_service;
#if false
	class httpreq
	{
	public:
		function<bool(const void* data, int len, int state/*-1:failed 1:connected 2:sent 3:recving 4:closed*/)> cb;
		boost::array<char, 4096> buf;
		urdl::read_stream stream;
		inline void dispose()
		{
			delete this;
		}
		void do_read();
		void cb_read(const boost::system::error_code& ec, size_t length)
		{
			if(length>0) {
				if(!cb(buf.data(),length,ec?4:3)) {
					dispose();
					return;
				}
				if(ec)
					dispose();
				else do_read();
			} else {
				if(!ec)
					cb(NULL,0,4);
				else {
					cerr << ec << endl;
					cb(NULL,0,-1);
				}
				dispose();
			}
		}
		void cb_open(const boost::system::error_code& ec)
		{
			if(!ec) {
				cb(NULL,0,1);
				do_read();
			} else {
				cerr << ec << endl;
				cb(NULL,0,-1);
				dispose();
			}
		}
		httpreq(const function<bool(const void* data, int len, int state)>& cb): stream(io_service), cb(cb)
		{}
		void get(const char* url)
		{
			stream.async_open(url, boost::bind(&httpreq::cb_open, this,
				boost::asio::placeholders::error));
		}
	};
	void httpreq::do_read()
	{
		boost::asio::async_read(stream, boost::asio::buffer(buf), 
		boost::bind(&httpreq::cb_read, this, boost::asio::placeholders::error, 
		boost::asio::placeholders::bytes_transferred));
	}
#endif
	static void httpget(const char* url,
		const function<bool(const void* data, int len, int state/*-1:failed 1:connected 2:sent 3:recving 4:closed*/)>& cb)
	{
		//httpreq* r=new httpreq(cb);
		//r->get(url);
		tm->addTransfer(url,cb);
	}
	static Handle<Value> get(const Arguments& a)
	{
		//*
		if(!(a.Length()<2))goto cont;
		return ThrowException(String::New("argument error: usage: get(String url, Function<void(String s)> cb)"));
	cont:
		try
		{
			String::Utf8Value s(a[0]);
			Persistent<Function> func=Persistent<Function>::New(Handle<Function>::Cast(a[1]));
			Persistent<Object> obj=Persistent<Object>::New(a.Holder());
			string* str=new string();
			httpget(*s, [func,str,obj](const void* data, int len, int state) {
				if(data!=NULL && len>0) str->append((const char*)data,len);
				if(state>=4 || state==-1)
				{
					Handle<Value> argv[]{String::New(str->data(),str->length())};
					func->Call(obj,1,argv);
					(*((Persistent<Function>*)&func)).Dispose();
					(*((Persistent<Object>*)&obj)).Dispose();
					delete str;
				}
				return true;
			});
			return Undefined(); //*/
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> download(const Arguments& a)
	{
		//*
		if(!(a.Length()<3))goto cont;
		return ThrowException(String::New("argument error: usage: download(String url, String filename, Function<void(int error)> cb)"));
	cont:
		try
		{
			String::Utf8Value url(a[0]);
			String::Utf8Value filename(a[1]);
			Persistent<Function> func=Persistent<Function>::New(Handle<Function>::Cast(a[2]));
			Persistent<Object> obj=Persistent<Object>::New(a.Holder());
			//string* str=new string();
			xaxaxa::FileStream* fs=new xaxaxa::FileStream(*filename,O_RDWR|O_CREAT|O_TRUNC,0644);
			
			httpget(*url, [func,fs,obj](const void* data, int len, int state) {
				if(data!=NULL && len>0) fs->Write({(void*)data,len});
				if(state>=4 || state==-1)
				{
					//cout << "state>=4 || state==-1" << endl;
					Handle<Value> argv[]{Integer::New(state>=4?0:state)};
					func->Call(obj,1,argv);
					(*((Persistent<Function>*)&func)).Dispose();
					(*((Persistent<Object>*)&obj)).Dispose();
					delete fs;
				}
				return true;
			});
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
		return Undefined(); //*/
	}
	static Handle<Value> exists(const Arguments& a)
	{
		if(!(a.Length()<1))goto cont;
		return ThrowException(String::New("argument error: usage: bool exists(String filename)"));
	cont:
		try
		{
			String::Utf8Value fn(a[0]);
			struct stat st;
			return Boolean::New(stat(*fn,&st)==0);
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> save(const Arguments& a)
	{
		if(!(a.Length()<2))goto cont;
		return ThrowException(String::New("argument error: usage: save(String filename, String content)"));
	cont:
		try
		{
			String::Utf8Value fn(a[0]);
			String::Utf8Value ct(a[1]);
			xaxaxa::FileStream fs(*fn,O_RDWR|O_CREAT|O_TRUNC,0644);
			const char* str=*ct;
			fs.Write({(void*)str,(int)strlen(str)});
			return Undefined();
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> load(const Arguments& a)
	{
		if(!(a.Length()<1))goto cont;
		return ThrowException(String::New("argument error: usage: String load(String filename)"));
	cont:
		try
		{
			String::Utf8Value fn(a[0]);
			xaxaxa::FileStream fs(*fn,O_RDONLY);
			xaxaxa::StringBuilder sb;
			while(sb.Append(fs,4096)>0);
			return String::New(sb.ToString().c,sb.Length());
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> ls(const Arguments& a)
	{
		if(!(a.Length()<2))goto cont;
		return ThrowException(String::New("argument error: usage: ls(String dir, Function callback)"));
	cont:
		try
		{
			String::Utf8Value dir(a[0]);
			Handle<Function> func(Handle<Function>::Cast(a[1]));
			Handle<Object> obj(a.Holder());
			DIR* d=opendir(*dir);
			if(d==NULL)return ThrowException(String::New(strerror(errno)));
			dirent* entry;
			Handle<Value> argv[1];
			while((entry=readdir(d))!=NULL)
			{
				argv[0]=String::New(entry->d_name);
				func->Call(obj,1,argv);
			}
			closedir(d);
			return Undefined();
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> symlink(const Arguments& a)
	{
		if(!(a.Length()<2))goto cont;
		return ThrowException(String::New("argument error: usage: symlink(String src, String dst)"));
	cont:
		try
		{
			String::Utf8Value src(a[0]);
			String::Utf8Value dst(a[1]);
			if(::symlink(*src,*dst)!=0)
				return ThrowException(String::New(strerror(errno)));
			return Undefined();
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> mkdir(const Arguments& a)
	{
		if(!(a.Length()<1))goto cont;
		return ThrowException(String::New("argument error: usage: mkdir(String dir)"));
	cont:
		try
		{
			String::Utf8Value dir(a[0]);
			if(::mkdir(*dir,0755)!=0)
				return ThrowException(String::New(strerror(errno)));
			return Undefined();
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> print(const Arguments& a)
	{
		if(!(a.Length()<1))goto cont;
		return ThrowException(String::New("argument error: usage: print(String s)"));
	cont:
		try
		{
			String::Utf8Value s(a[0]);
			GenericUI::attach("main").attach("log").invoke({O(*s)});
			//cout << *s << endl;
			return Undefined();
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> prompt(const Arguments& a)
	{
		if(!(a.Length()<1))goto cont;
		return ThrowException(String::New("argument error: usage: prompt(String s)"));
	cont:
		try
		{
			String::Utf8Value s1(a[0]);
			
			char* line=NULL;
			size_t len=0;
			O val=GenericUI::attach("main").attach("prompt").invokeSync({O(*s1)});
			if(val.type!=Types::array) return ThrowException(String::New("could not prompt user"));
			line=val.getString();
			//if(getline(&line,&len,stdin)<0) return ThrowException(String::New("EOF"));
			size_t len2=strlen(line);
			for(;len2>0;len2--) {
				char c=line[len2-1];
				if(c!='\x0A' && c!='\x0D')break;
			}
			Handle<String> s=String::New(line,len2);
			//free(line);
			return s;
		}
		catch(exception& ex)
		{
			return ThrowException(String::New(ex.what()));
		}
	}
	static Handle<Value> exit(const Arguments& a)
	{
		int code=0;
		if(a.Length()>0)
		{
			Integer* i=Integer::Cast(*a[0]);
			code=i->Value();
		}
		::exit(code);
	}
	static Handle<ObjectTemplate> gettemplate()
	{
		return lib::gettemplate({{"print",getfunc(print)}, {"get",getfunc(get)},
			{"download",getfunc(download)}, {"exists",getfunc(exists)},
			{"save",getfunc(save)}, {"symlink",getfunc(symlink)},
			{"mkdir",getfunc(mkdir)}, {"exit",getfunc(exit)},
			{"load",getfunc(load)}, {"ls",getfunc(ls)},
			{"prompt",getfunc(prompt)} });
	}
}
namespace fbd
{
	using namespace lib;
	static Handle<Value> dump(const Arguments& a)
	{
		if(!(a.Length()<1))goto cont;
		return ThrowException(String::New("argument error: usage: dump(String graphpath, Function callback)"));
	cont:
		
		return Undefined();
	
	}
	static Handle<ObjectTemplate> gettemplate()
	{
		return lib::gettemplate({{"dump",getfunc(dump)}});
	}
};
struct runArg {
	A scripts;
	xaxaxa::StringBuilder header;
	int concurrency;
};
void* do_run(void* arg1)
{
	Locker locker;
	runArg *arg=(runArg*)arg1;
	int concurrency=3;
	HandleScope handle_scope;
	Handle<ObjectTemplate> global = ObjectTemplate::New();
	global->Set(String::New("lib"), l::gettemplate());
	global->Set(String::New("fbd"), fbd::gettemplate());
	Persistent<Context> context = Context::New(NULL,global);
	Context::Scope context_scope(context);
	xaxaxa::StringBuilder sb;
	tm=new curl::transferManager(arg->concurrency);
	{
		ScriptOrigin origin(String::New("/fbdump.js"),Integer::New(0),Integer::New(0));
		Handle<String> source = String::New(arg->header.ToString().c, arg->header.ToString().length); //"lib.get(\"http://xa.us.to\",function(s){lib.print(s);});"
		Script::Compile(source,&origin)->Run();
	}
	//char* ch=(char*)0;
	//*ch=2;
	//xaxaxa::StringBuilder sb;
	for(int i=0;i<(int)arg->scripts.length();i++)
	{
		sb.Clear();
		{
			xaxaxa::FileStream fs(arg->scripts[i].getString(),O_RDONLY);
			while(sb.Append(fs,4096)>0);
		}
		//cout << sb.ToCString() << endl;
		ScriptOrigin origin(String::New(arg->scripts[i].getString()),Integer::New(0),Integer::New(0));
		Handle<String> source = String::New(sb.ToString().c, sb.Length()); //"lib.get(\"http://xa.us.to\",function(s){lib.print(s);});"
		Script::Compile(source,&origin)->Run();
	}
	delete arg;
	//curl::eventLoop(&curl_inst);
	tm->eventLoop();
	//l::io_service.run();
	context.Dispose();
	GenericUI::attach("main").attach("finish").invoke(O::undefined());
}
int main(int argc, char* argv[]) {
	xaxaxa::Util.SetHandlers();
	/*l::httpget("http://192.168.5.11",[](const void* data, int len, int state) {
		//cout << state << " " << len << endl;
		if(data!=NULL && len>0) write(1,data,len);
		return true;
	});
	l::io_service.run();
	return 0;*/
	if(GenericUI::attach("main").attach("checkUI").invokeSync(O::undefined()).type==Types::undefined)
		GenericUI::load("fbdump_cui.so");
	if(GenericUI::attach("main").attach("checkUI").invokeSync(O::undefined()).type==Types::undefined)
		cerr << "warn: there's no UI loaded and loading the default UI failed" << endl;
	
	//handlers
	auto& iface=GenericUI::attach("main");
	iface.attach("finish");
	iface.attach("start").attach([](O o) {
		//startArgs* a=(startArgs*)v;
		pthread_t thr;
		runArg* arg=new runArg();
		arg->scripts=o.getArray()[1].getArray();
		arg->concurrency=o.getArray()[2].getInt32();
		xaxaxa::StringBuilder& sb=arg->header;
		string tmp_s1=(xaxaxa::Util.GetDirFromPath(xaxaxa::Util.GetProgramPath())+"/fbdump.js");
		const char* fn1=tmp_s1.c_str();
		{
			xaxaxa::FileStream fs(fn1,O_RDONLY);
			while(sb.Append(fs,4096)>0);
        }
        sb.Append(o.getArray()[0].getString());
		pthread_create(&thr, NULL, &do_run, arg);
		return O::undefined();
	});
	
	A tmp_a(Types::array);
	for(int i=0;i<argc;i++)
		tmp_a.append(argv[i]);
	return GenericUI::attach("main").attach("run").invokeSync(A{O(argc), O(tmp_a)}).getInt32();
	//return 0;
}
