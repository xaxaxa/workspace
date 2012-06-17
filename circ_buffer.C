#include <iostream>
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;

CircularBuffer buf(1024*1024*32);
pthread_t w_thr;
MutexLock buf_lock;
EventHandle evt_data;	//data available for write
EventHandle evt_space;	//space available for read
bool in_eof(false),out_eof(false);
void* th1(void* v);
int main(int ac, char** av)
{
	const int bs=1024*1024*2;
	StandardStream ss;
	
	pthread_create(&w_thr,NULL,&th1,NULL);
	Int br;
	function<bool(BufferRef)> func=[&](const BufferRef& b)
	{
		Int br1;
		br+=(br1=ss.Read(b));
		//cerr << "b.Length = " << b.Length << endl;
		return (br1==b.Length);
	};
	while(true)
	{
		Int i;
	aaa:
		{
			Lock<MutexLock> lck(buf_lock);
			if(out_eof)return 0;
			if((i=buf.BeginAppend(bs))>=0) goto asdf;
		}
		evt_space.Wait();
		goto aaa;
	asdf:
		br=0;
		try
		{
			buf.BufferOperations(func, i, bs);
			//cerr << "br = " << br << endl;
		}
		catch(Exception& ex)
		{
			in_eof=true;
			break;
		}
		if(br==0)
		{
			in_eof=true;
			break;
		}
		{
			Lock<MutexLock> lck(buf_lock);
			buf.EndAppend(i, br);
		}
		evt_data.Set();
		//cerr << "evt_data.Set();" << endl;
	}
	evt_data.Set();
	pthread_join(w_thr,NULL);
}
void* th1(void* v)
{
	const int bs=1024*1024*32;
	StandardStream ss;
	Int br;
	function<bool(BufferRef)> func=[&](BufferRef b)
	{
		//cerr << "ss.Write(" << (void*)b.Data << ", " << b.Length << ");" << endl;
		ss.Write(b);
		return true;
	};
	while(true)
	{
		Int i;
		br=bs;
	aaa:
		{
			Lock<MutexLock> lck(buf_lock);
			if((i=buf.BeginDequeue(br,true))>=0) goto asdf;
			//cerr << "buf.BeginDequeue(bs) = " << i << ';' << endl; 
		}
		if(in_eof)return NULL;
		evt_data.Wait();
		goto aaa;
	asdf:
		try
		{
			buf.BufferOperations(func, i, br);
		}
		catch(Exception& ex)
		{
			out_eof=true;
			break;
		}
		{
			Lock<MutexLock> lck(buf_lock);
			buf.EndDequeue(i, br);
		}
		evt_space.Set();
	}
	evt_space.Set();
	return NULL;
}


