/*
 * JoinStream.h
 *
 *  Created on: 2011-07-10
 *      Author: xaxaxa
 */

#ifndef JOINSTREAM_H_
#define JOINSTREAM_H_
#include <cplib/cplib.hpp>
#ifdef __debug_print123
#define dbgprint(msg) cout << msg << endl
#else
#define dbgprint(msg)
#endif
using namespace xaxaxa;
class JoinStream:public Object {
public:
	FUNCTION_DECLARE(BufferCallback,void,JoinStream*,Buffer*);
	struct item
	{
		Buffer b;
		int length;
	};
	Stream* s1;
	Stream* s2;
	BufferManager* m;
	CircularQueue<item> q1;//s1 --> s2
	CircularQueue<item> q2;//s2 --> s1
	int s1r_i,s1w_i,s2r_i,s2w_i;
	bool s1reading,s1writing,s2reading,s2writing;
	bool s1closed, s2closed;
	bool dowrite1, dowrite2;
	BufferCallback ProcessBuffer1;
	BufferCallback ProcessBuffer2;
	void s1_r(Stream* s);
	void s1_w(Stream* s);
	void s2_r(Stream* s);
	void s2_w(Stream* s);
	FUNCTION_DECLWRAPPER(_s1_r,void,Stream* s)
	{((JoinStream*)obj)->s1_r(s);}
	FUNCTION_DECLWRAPPER(_s1_w,void,Stream* s)
	{((JoinStream*)obj)->s1_w(s);}
	FUNCTION_DECLWRAPPER(_s2_r,void,Stream* s)
	{((JoinStream*)obj)->s2_r(s);}
	FUNCTION_DECLWRAPPER(_s2_w,void,Stream* s)
	{((JoinStream*)obj)->s2_w(s);}

	JoinStream(Stream* s1=NULL, Stream* s2=NULL, BufferManager* m=NULL, int sendbuffers=256, int recvbuffers=256);
	void Begin();
	void BeginRead();
	void Stop();
	void Close();
	void begin1r();//s1 -->
	void begin2w();//         --> s2
	void begin2r();//s2 -->
	void begin1w();//         --> s1
	virtual ~JoinStream();
};

#endif /* JOINSTREAM_H_ */
