/*
 * socketmux.h
 *
 *  Created on: 2011-07-24
 *      Author: xaxaxa
 */

#ifndef SOCKETMUX_H_
#define SOCKETMUX_H_

#include <cplib.hpp>
#include <map>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace std;
using namespace boost;

namespace xaxaxa
{
class socketmux: public xaxaxa::Object, public enable_shared_from_this<socketmux> {
public:


	socketmux();
	//weak_ptr<socketmux> _this;
	shared_ptr<socketmux> __this;
	bool retain;
	virtual ~socketmux();
	const static int maxbufsize=1024*1024*32;
	/*
	 * protocol:
	 * 0.0-0.6: cmd
	 * 0.7: outgoing bit
	 * 1-5: id
	 * 5-9: data length
	 * 9-*: data
	 */
	enum cmd
	{
		cmd_nop,
		cmd_create,
		cmd_send,
		cmd_close
	};
	struct b
	{
		Buffer b;
		int length;
	};
	struct item
	{
		weak_ptr<item> wptr;
		list<shared_ptr<item> >::iterator iter;
		shared_ptr<Stream> s;
		weak_ptr<socketmux> owner;
		xaxaxa::CircularQueue<b> queue_in;
		xaxaxa::CircularQueue<b> queue_out;

		bool sending, receiving;
		bool dosend, dorecv, closed, closed_r;
		bool outgoing;
		int id;
		int sr_i,sw_i;
		bool hasData;
		inline item(int qsize_in, int qsize_out):queue_in(qsize_in),queue_out(qsize_out)
		{
			//owner=NULL;
			sending=receiving=closed=closed_r=false;
			dosend=dorecv=true;
			hasData=false;
		}
		virtual ~item()
		{
			if(s.get()!=NULL)
			{
				s->Close();
				closed=true;
				Clear();
			}
		}
		void BeginSend();
		void BeginRecv();
		inline void sn_r(Stream* s);
		inline void sn_w(Stream* s);
		inline void update_status();
		inline void Close();
		inline void Clear();
		FUNCTION_DECLWRAPPER(_sn_r,void,Stream* s)
		{((item*)obj)->sn_r(s);}
		FUNCTION_DECLWRAPPER(_sn_w,void,Stream* s)
		{((item*)obj)->sn_w(s);}
	};
	struct cmditem
	{
		cmd c;
		bool outgoing;
		int id;
		Buffer data;
	};
	FUNCTION_DECLARE(BufferCallback,void,shared_ptr<socketmux>,shared_ptr<item>,bool/*direction_out*/,const Buffer&);
	BufferCallback ProcessBuffer;
	int max_id;//(outgoing)
	BufferManager bm;
	//ArrayList<cmditem> cmdqueue;//out of band commands
	shared_ptr<Stream> main;
	//list<item> items;
	//list<item>::iterator next_item;
	map<int,shared_ptr<item> > items_o;//outgoing
	map<int,shared_ptr<item> > items_i;//incoming
	list<shared_ptr<item> > items_r;//has data
	bool sending_i;//whether we're sending data from the incoming queue
	//map<int,shared_ptr<item> >::iterator next_item;
	list<shared_ptr<item> >::iterator cur_item;
	bool sending, receiving, dosend, dorecv, closed;
	bool sending_oob;
	int s1r_i,s1w_i;
	FUNCTION_DECLARE(RequestCallback,void,socketmux*,Buffer*,shared_ptr<socketmux::item>&);
	RequestCallback ConnectionRequest;//event

	__uint8_t tmpbuf1[9];
	__uint8_t tmpbuf2[9];
	Buffer tmpbuf;
	StringBuilder tmpsb1;
	StringBuilder tmpsb2;
	bool use_sb2;
	Buffer recvbuf;
	int recv_br;
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void BeginSend();
	void BeginRecv();
	void BeginSend(shared_ptr<item> it);
	void BeginRecv(shared_ptr<item> it);
	inline void update_status();
	map<int,shared_ptr<item> >::iterator AddStream(shared_ptr<Stream> s, Buffer* cmddata=NULL); //requests the server to open a stream; attach stream to s
	inline void __sendclose(int id, bool outgoing);
	inline void __sendcreate(int id, Buffer* cmddata);
	inline void __add_oobcmd(cmditem& it);
	void Close();
	inline void Close(shared_ptr<item> it);

	inline void s1_r(Stream* s);
	inline void s1_r2(Stream* s);
	inline void s1_w(Stream* s);
	inline void s1_w2(Stream* s);
	//inline void sn_r(Stream* s);
	//inline void sn_w(Stream* s);
	inline void __process_req();
	FUNCTION_DECLWRAPPER(_s1_r,void,Stream* s)
	{((socketmux*)obj)->s1_r(s);}
	FUNCTION_DECLWRAPPER(_s1_r2,void,Stream* s)
	{((socketmux*)obj)->s1_r2(s);}
	FUNCTION_DECLWRAPPER(_s1_w,void,Stream* s)
	{((socketmux*)obj)->s1_w(s);}
	FUNCTION_DECLWRAPPER(_s1_w2,void,Stream* s)
	{((socketmux*)obj)->s1_w2(s);}
	/*FUNCTION_DECLWRAPPER(_sn_r,void,Stream* s)
	{((socketmux*)obj)->sn_r(s);}
	FUNCTION_DECLWRAPPER(_sn_w,void,Stream* s)
	{((socketmux*)obj)->sn_w(s);}*/
};
}

#endif /* SOCKETMUX_H_ */
