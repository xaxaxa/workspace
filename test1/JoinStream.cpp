/*
 * JoinStream.cpp
 *
 *  Created on: 2011-07-10
 *      Author: xaxaxa
 */


#include "JoinStream.h"
#include <cplib/cplib.hpp>
using namespace xaxaxa;

JoinStream::JoinStream(Stream* s1, Stream* s2, BufferManager* m, int sendbuffers, int recvbuffers):q1(sendbuffers),q2(recvbuffers) {
	this->m=(m==NULL?m=BufferManager::GetDefault():m);
	this->s1=s1;
	this->s2=s2;
	s1reading=false;
	s2reading=false;
	s1writing=false;
	s2writing=false;
	dowrite1=false;
	dowrite2=false;
	s1closed=false;
	s2closed=false;
}

JoinStream::~JoinStream() {
}

void JoinStream::Begin()
{
	dowrite1=true;
	dowrite2=true;
	begin1r();
	begin2r();
	begin1w();
	begin2w();
}
void JoinStream::BeginRead()
{
	dowrite1=false;
	dowrite2=false;
	begin1r();
	begin2r();
}
void JoinStream::Stop()
{
	if(s1writing&&!s1closed) return;
	if(s2writing&&!s2closed) return;
	Close();
}
void JoinStream::Close()
{
	dbgprint("Close()");
	this->s1->Close();
	this->s2->Close();
	if(s1reading) {q1.EndAppend(s1r_i);}
	if(s2reading) {q2.EndAppend(s2r_i);}
	if(s2writing) {m->Return(q1.GetPointer(s2w_i).b);q1.EndDequeue(s2w_i);}
	if(s1writing) {m->Return(q2.GetPointer(s1w_i).b);q2.EndDequeue(s1w_i);}
	int i;
	while((i=q1.BeginDequeue())>=0)
	{
		m->Return(q1.GetPointer(i).b);
		q1.EndDequeue(i);
	}
	while((i=q2.BeginDequeue())>=0)
	{
		m->Return(q2.GetPointer(i).b);
		q2.EndDequeue(i);
	}
	onclose(this);
	//this->Release();
}
void JoinStream::begin1r()//s1 -->
{
	dbgprint("begin1r()");
	if(s1reading)return;
	s1r_i=q1.BeginAppend();
	if(s1r_i>=0)
	{
		dbgprint("s1r_i>=0");
		s1reading=true;
		Buffer b=m->Get();
		q1.GetPointer(s1r_i).b=b;
		try{
		s1->BeginRead(b,Stream::Callback(_s1_r,this));
		}catch(Exception& ex){s1closed=true;Stop();return;}
	}
}
void JoinStream::begin2w()//         --> s2
{
	if(s2writing||!dowrite2)return;
	dbgprint("begin2w()");
	s2w_i=q1.BeginDequeue();
	if(s2w_i>=0)
	{
		dbgprint("s2w_i>=0");
		s2writing=true;
		item& it=q1.GetPointer(s2w_i);
		try{
		Buffer buf1=it.b.SubBuffer(0,it.length);
		if(!FUNCTION_ISNULL(ProcessBuffer1))FUNCTION_CALL(ProcessBuffer1,this,&buf1);
		s2->BeginWrite(buf1,Stream::Callback(_s2_w,this));
		}catch(Exception& ex){s2closed=true;Stop();return;}
	}
	else if(s1closed) Stop();
}
void JoinStream::begin2r()//s2 -->
{
	if(s2reading)return;
	s2r_i=q2.BeginAppend();
	if(s2r_i>=0)
	{
		s2reading=true;
		Buffer b=m->Get();
		q2.GetPointer(s2r_i).b=b;
		try{
		s2->BeginRead(b,Stream::Callback(_s2_r,this));
		}catch(Exception& ex){s2closed=true;Stop();return;}
	}
}
void JoinStream::begin1w()//         --> s1
{
	if(s1writing||!dowrite1)return;
	s1w_i=q2.BeginDequeue();
	if(s1w_i>=0)
	{
		s1writing=true;
		item& it=q2.GetPointer(s1w_i);
		try{
		Buffer buf1=it.b.SubBuffer(0,it.length);
		if(!FUNCTION_ISNULL(ProcessBuffer2))FUNCTION_CALL(ProcessBuffer2,this,&buf1);
		s1->BeginWrite(buf1,Stream::Callback(_s1_w,this));
		}catch(Exception& ex){s1closed=true;Stop();return;}
	}
	else if(s2closed) Stop();
}

inline void JoinStream::s1_r(Stream* s)
{
	int l;
	try{
	l=s1->EndRead();
	}catch(Exception& ex){s1closed=true;Stop();return;}
	//cout<<l<<" bytes read from s1"<<endl;
	dbgprint("s1_r()");
	if(l<=0) {s1closed=true;Stop();return;}
	q1.GetPointer(s1r_i).length=l;
	q1.EndAppend(s1r_i);
	s1reading=false;
	begin1r();
	begin2w();
}
inline void JoinStream::s2_w(Stream* s)
{
	try{
	s2->EndWrite();
	}catch(Exception& ex){s2closed=true;Stop();return;}
	//cout<<"s2_w"<<endl;
	m->Return(q1.GetPointer(s2w_i).b);
	q1.EndDequeue(s2w_i);
	s2writing=false;
	begin2w();
	begin1r();
}
inline void JoinStream::s2_r(Stream* s)
{
	int l;
	try{
	l=s2->EndRead();
	}catch(Exception& ex){s2closed=true;Stop();return;}
	//cout<<l<<" bytes read from s1"<<endl;
	if(l<=0) {s2closed=true;Stop();return;}
	q2.GetPointer(s2r_i).length=l;
	q2.EndAppend(s2r_i);
	s2reading=false;
	begin2r();
	begin1w();
}
inline void JoinStream::s1_w(Stream* s)
{
	//WARN(1,this << "::s1_w()");
	try{
	s1->EndWrite();
	}catch(Exception& ex){s1closed=true;Stop();return;}
	//cout<<"s1_w"<<endl;
	m->Return((q2.GetPointer(s1w_i).b));
	q2.EndDequeue(s1w_i);
	s1writing=false;
	begin1w();
	begin2r();
}


