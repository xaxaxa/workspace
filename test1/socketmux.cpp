/*
 * socketmux.cpp
 *
 *  Created on: 2011-07-24
 *      Author: xaxaxa
 */

#ifndef errprint
#define errprint(x) cout<<x<<endl;
#endif

#include "socketmux.h"
namespace xaxaxa
{
	socketmux::socketmux(): bm(4096,32), tmpbuf(tmpbuf1,9) {
		// TODO Auto-generated constructor stub
		//next_item=items_o.begin();
		closed=sending_oob=use_sb2=sending=receiving=sending_i=false;
		dosend=dorecv=true;
		max_id=0;
	}

	socketmux::~socketmux() {
		//if(main.get()!=NULL)main->Close();
		//tmpbuf.Release();
	}
	void socketmux::BeginSend()
	{
		if(sending||!dosend||closed)
		{
			update_status();
			return;
		}
		dbgprint("socketmux::BeginSend()");
		//tmpsb.Length=0;
		//char asdf[9];
		StringBuilder& sb=use_sb2?tmpsb2:tmpsb1;
		if(sb.Length>0)
		{
			sending=true;
			sending_oob=true;
			/*int i;
			for(i=0;i<cmdqueue.Length;i++)
			{
				cmditem* tmp=&cmdqueue.array[i];
				asdf[0]=(tmp->outgoing?(char)(tmp->c+128):(char)tmp->c);
				*((int*)(&asdf+1))=tmp->id;
				*((int*)(&asdf+5))=tmp->data.length;
				tmpsb.Append((char*)&asdf,9);
				if(tmp->data.length>0)
				{
					tmpsb.Append(tmp->data);
					tmp->data.Release();
				}
			}
			cmdqueue.Length=0;*/
			main->BeginWrite(sb.ToBuffer(),Stream::Callback(_s1_w,this));
			use_sb2=!use_sb2;
			update_status();
			return;
		}
		//map<int,shared_ptr<item> >* m=sending_i?&items_i:&items_o;
		shared_ptr<item> it;
		//int i;
		/*while(next_item!=items_r->end())
		{
			it=*next_item;
			s1w_i=it->queue_in.BeginDequeue();
			if(s1w_i>=0)goto x;
			next_item++;
		}
		sending_i=!sending_i;
		m=sending_i?&items_i:&items_o;
		next_item=m->begin();
		while(next_item!=m->end())
		{
			it=(*next_item).second;
			s1w_i=it->queue_in.BeginDequeue();
			if(s1w_i>=0)goto x;
			next_item++;
		}*/
y:
		if(items_r.begin()==items_r.end())
		{
			update_status();
			return;
		}
		//it=*(items_r.begin());
		//return;
x:
		cur_item=items_r.begin();
		it=*cur_item;
		s1w_i=it->queue_in.BeginDequeue();
		if(s1w_i<0)
		{
			it->update_status();
			goto y;
		}
		sending=true;
		sending_oob=false;
		items_r.pop_front();
		it->iter=cur_item=items_r.insert(items_r.end(),it);
		b& buf=it->queue_in.GetPointer(s1w_i);

		//__uint8_t asdf[9];
		tmpbuf2[0]=(it->outgoing?((__uint8_t)cmd_send+128):(__uint8_t)cmd_send);
		*((int*)(tmpbuf2+1))=it->id;
		*((int*)(tmpbuf2+5))=buf.length;
		main->BeginWrite(Buffer(tmpbuf2,9),Stream::Callback(_s1_w,this));
		update_status();
		////main->BeginWrite(buf->b.SubBuffer(0,buf->length,false),Stream::Callback(_s1_w,this));
		//it->queue.EndDequeue(i);
		//next_item++;
	}
	void socketmux::BeginRecv()
	{
		if(receiving||!dorecv||closed)
		{
			update_status();
			return;
		}
		dbgprint("socketmux::BeginRecv()");
		receiving=true;
		update_status();
		recv_br=0;
		main->BeginRead(tmpbuf,Stream::Callback(_s1_r,this));
	}
	void socketmux::update_status()
	{
		bool nretain=sending||receiving&&!closed;
		if(nretain==retain)return;
		if(nretain&&!retain)
		{
			__this=shared_from_this();
		}
		else if(retain&&!nretain)
		{
			__this.reset();
		}
		retain=nretain;
	}
	void socketmux::BeginSend(shared_ptr<item> it)
	{
		//shared_ptr<item> it=(*i).second;
		it->BeginSend();
	}
	void socketmux::BeginRecv(shared_ptr<item> it)
	{
		//shared_ptr<item> it=(*i).second;
		it->BeginRecv();
	}
	map<int,shared_ptr<socketmux::item> >::iterator socketmux::AddStream(shared_ptr<Stream> s, Buffer* cmddata)
	{
		dbgprint("socketmux::AddStream()");
		shared_ptr<item> it(new socketmux::item(512,256));
		it->id=++max_id;
		it->s=s;
		it->owner=shared_from_this();
		it->outgoing=true;
		it->wptr=it;

		//it.queue=new xaxaxa::CircularQueue<b>(64);
		dbgprint("penissssssssssss");
		pair<int,shared_ptr<item> > p=make_pair<int,shared_ptr<item> >(it->id,it);
		dbgprint("xxxxxxxxxxxxxxxxxxxx");

		//return map<int,socketmux::item>::iterator();

		pair<map<int,shared_ptr<item> >::iterator,bool> ret=items_o.insert(p);
		dbgprint("zxcvbnmzxcvbnm");
		if(ret.second)
		{
			__sendcreate(it->id,cmddata);
			dbgprint("asssssssssssssssssssss");
			return ret.first;
		}
		throw Exception("could not add stream");
	}
	void socketmux::__sendclose(int id, bool outgoing)
	{
		dbgprint("socketmux::__sendclose()");
		cmditem it;
		it.c=cmd_close;
		it.id=id;
		it.data.length=0;
		it.outgoing=outgoing;
		//cmdqueue.Append(it);
		__add_oobcmd(it);
	}
	void socketmux::__sendcreate(int id,Buffer* cmddata)
	{
		dbgprint("socketmux::__sendcreate()");
		cmditem it;
		it.c=cmd_create;
		it.id=id;
		if(cmddata==NULL)
			it.data.length=0;
		else
		{
			it.data=*cmddata;
			//it.data.Retain();
		}
		//cmdqueue.Append(it);
		__add_oobcmd(it);
	}
	void __printcmd(__uint8_t* data,char* asdfghj="COMMAND: ")
		{
			bool outgoing;
			__uint8_t tmp_i=data[0];
			if((outgoing=(tmp_i>=128)))tmp_i-=128;
			socketmux::cmd c=(socketmux::cmd)tmp_i;
			switch(c)
			{
				case socketmux::cmd_create:
					dbgprint(asdfghj<<"CMD_CREATE");
					break;
				case socketmux::cmd_send:
					dbgprint(asdfghj<<"CMD_SEND");
					break;
				case socketmux::cmd_close:
					dbgprint(asdfghj<<"CMD_CLOSE");
					break;
				default:
					dbgprint("==========MALFORMED COMMAND: "<<c<<" ===========");
			}
			dbgprint("id = "<<*((int*)(data+1)));
			dbgprint("datalen = "<<*((int*)(data+5)));
		}
	void socketmux::__add_oobcmd(cmditem& it)
	{
		__uint8_t asdf[9];
		asdf[0]=(it.outgoing?(__uint8_t)(it.c+128):(__uint8_t)it.c);
		*((int*)(asdf+1))=it.id;
		*((int*)(asdf+5))=it.data.length;
		StringBuilder& sb=use_sb2?tmpsb2:tmpsb1;
		sb.Append((char*)asdf,9);
		__printcmd(asdf,"SEND COMMAND: ");
		if(it.data.length>0)
		{

			sb.Append(it.data);
			//it.data.Release();
		}
		BeginSend();
	}

	void socketmux::s1_r(Stream* s)
	{
		dbgprint("socketmux::s1_r()");
		try{
			int br=main->EndRead();
			if(br<=0)
			{
				closed=true;
				Close();
				return;
			}
			recv_br+=br;
			if(recv_br<tmpbuf.length)
			{
				main->BeginRead(tmpbuf.SubBuffer(recv_br,tmpbuf.length-recv_br),Stream::Callback(_s1_r,this));
				return;
			}
		}catch(Exception ex){closed=true;Close();return;}
		__printcmd(tmpbuf1);
		__uint8_t tmp_i=tmpbuf1[0];
		bool outgoing;
		if((outgoing=(tmp_i>=128)))tmp_i-=128;
		cmd c=(cmd)tmp_i;
		switch(c)
		{
		case cmd_create:
		case cmd_send:
		{
			int datalen=*((int*)(tmpbuf1+5));
			if(datalen>maxbufsize){closed=true;Close();return;}
			if(datalen<=0)
			{
				recvbuf=Buffer();
				__process_req();
				receiving=false;
				BeginRecv();
				return;
			}
			Buffer b(datalen);
			recvbuf=b;
			recv_br=0;
			main->BeginRead(recvbuf,Stream::Callback(_s1_r2,this));
		}
		break;
		case cmd_close:
		{
			int id=*((int*)(tmpbuf1+1));
			map<int,shared_ptr<item> >& m=outgoing?items_i:items_o;
			map<int,shared_ptr<item> >::iterator iter=m.find(id);
			if(iter==m.end())
			{
				errprint("========CMD_CLOSE: item id "<<id<<" not found [outgoing="<<outgoing<<"]========");
			}
			else
			{
				shared_ptr<item> it=(*iter).second;
				it->closed_r=true;
				it->Close();
			}
		}
		default:
			receiving=false;
			BeginRecv();
			break;
		}
	}
	void socketmux::s1_r2(Stream* s)
	{
		dbgprint("socketmux::s1_r2()");
		try{
			int br=main->EndRead();
			if(br<=0)
			{recvbuf.Release();closed=true;Close();return;}
			recv_br+=br;
			if(recv_br<recvbuf.length)
			{
				main->BeginRead(recvbuf.SubBuffer(recv_br,recvbuf.length-recv_br),Stream::Callback(_s1_r2,this));
				return;
			}
		}catch(Exception ex){recvbuf.Release();closed=true;Close();return;}
		__process_req();
		receiving=false;
		BeginRecv();
	}
	void socketmux::__process_req()
	{
		dbgprint("socketmux::__process_req()");
		bool outgoing;
		__uint8_t tmp_i=tmpbuf1[0];
		if((outgoing=(tmp_i>=128)))tmp_i-=128;
		cmd c=(cmd)tmp_i;
		int id=*((int*)(tmpbuf1+1));
		switch(c)
		{
		case cmd_create:
		{
			if(FUNCTION_ISNULL(ConnectionRequest))
			{
				__sendclose(id,false);
				break;
			}
			shared_ptr<item> it(new item(512,256));
			it->id=id;
			//it->s=NULL;
			it->outgoing=false;
			it->owner=shared_from_this();
			it->wptr=it;
			//it.queue=new xaxaxa::CircularQueue<b>(64);
			pair<map<int,shared_ptr<item> >::iterator,bool> ret=items_i.insert(make_pair<int,shared_ptr<item> >(id,it));
			if(ret.second)
			{
				FUNCTION_CALL(ConnectionRequest,this,&recvbuf,(*(ret.first)).second);
			}
			else
			{
				__sendclose(id,false);
			}
		}
		break;
		case cmd_send:
		{
			map<int,shared_ptr<item> >& m=outgoing?items_i:items_o;
			map<int,shared_ptr<item> >::iterator iter=m.find(id);
			if(iter==m.end())
			{
				dbgprint("========PROTOCOL VIOLATION: item id "<<id<<" not found========");
				break;
			}
			shared_ptr<item> it=(*iter).second;
			int index;
			if((index=it->queue_out.BeginAppend())>=0)
			{
				b& tmp=it->queue_out.GetPointer(index);
				tmp.b=recvbuf;
				tmp.length=recvbuf.length;
				it->queue_out.EndAppend(index);
				BeginSend(it);
				return;
			}
			else
			{
				errprint("receive queue for stream "<<id<<" full. data discarded.");
				//recvbuf.Release();
			}
		}
		break;
		default:
			break;
		}
		recvbuf.Release();
	}
	void socketmux::s1_w(Stream* s)
	{
		dbgprint("socketmux::s1_w()");
		try
		{main->EndWrite();}
		catch(Exception ex)
		{closed=true;Close();return;}
		if(sending_oob)
		{
			sending=false;
			StringBuilder& sb=use_sb2?tmpsb1:tmpsb2;
			sb.Length=0;
			BeginSend();
		}
		else
		{
			shared_ptr<item> it=*cur_item;
			b& buf=it->queue_in.GetPointer(s1w_i);
			main->BeginWrite(buf.b.SubBuffer(0,buf.length),Stream::Callback(_s1_w2,this));
		}

	}
	void socketmux::s1_w2(Stream* s)
	{
		try
		{main->EndWrite();}
		catch(Exception ex)
		{closed=true;Close();return;}
		sending=false;
		shared_ptr<item> it=*cur_item;
		bm.Return(&(it->queue_in.GetPointer(s1w_i).b));
		it->queue_in.EndDequeue(s1w_i);
		it->update_status();
		if(it->closed)it->Close();
		BeginRecv(it);
		BeginSend();
	}
	void socketmux::Close()
	{
		main->Close();
		if(sending&&!sending_oob)
		{
			shared_ptr<item> it=*cur_item;
			bm.Return(&(it->queue_in.GetPointer(s1w_i).b));
			it->queue_in.EndDequeue(s1w_i);
		}
		items_o.clear();
		items_i.clear();
		items_r.clear();
		sending=receiving=false;
		update_status();
	}
	void socketmux::Close(shared_ptr<item> it)
	{
		//shared_ptr<item> it=(*i).second;
		it->Close();
	}


	void socketmux::item::BeginRecv()
	{
		dbgprint("item::BeginRecv()");
		if(receiving||!dorecv||closed)return;
		if((sr_i=queue_in.BeginAppend())>=0)
		{
			dbgprint("sr_i>=0");
			receiving=true;
			shared_ptr<socketmux> _owner(owner);
			Buffer b=_owner->bm.Get();
			queue_in.GetPointer(sr_i).b=b;
			try{
				s->BeginRead(b,Stream::Callback(_sn_r,this));
			}catch(Exception ex){closed=true;Close();return;}
		}
	}
	void socketmux::item::BeginSend()
	{
		if(sending||!dosend||closed)return;
		dbgprint("item::BeginSend()");
		if((sw_i=queue_out.BeginDequeue())>=0)
		{
			dbgprint("sw_i>=0");
			sending=true;
			b& buf=queue_out.GetPointer(sw_i);
			try{
				Buffer buf1=buf.b.SubBuffer(0,buf.length);
				shared_ptr<socketmux> _owner(owner);
				if(!FUNCTION_ISNULL(_owner->ProcessBuffer))
					FUNCTION_CALL(_owner->ProcessBuffer,_owner,shared_ptr<item>(wptr),true,buf1);
				//if(!FUNCTION_ISNULL(ProcessBuffer1))FUNCTION_CALL(ProcessBuffer1,this,&buf1);
				s->BeginWrite(buf1,Stream::Callback(_sn_w,this));
			}catch(Exception ex){closed=true;Close();return;}
		}
		else if(closed_r)Close();
	}
	void socketmux::item::sn_r(Stream* s)
	{
		int l;
		try{
			l=s->EndRead();
		}catch(Exception ex){closed=true;Close();return;}
		//cout<<l<<" bytes read from s1"<<endl;
		dbgprint("sn_r()");
		if(l<=0) {closed=true;Close();return;}
		shared_ptr<socketmux> _owner(owner);
		if(!FUNCTION_ISNULL(_owner->ProcessBuffer))
			FUNCTION_CALL(_owner->ProcessBuffer,_owner,shared_ptr<item>(wptr),false,queue_in.GetPointer(sr_i).b.SubBuffer(0,l));
		queue_in.GetPointer(sr_i).length=l;
		queue_in.EndAppend(sr_i);
		receiving=false;
		update_status();
		BeginRecv();
		_owner->BeginSend();
	}
	void socketmux::item::sn_w(Stream* s)
	{
		try{
			s->EndWrite();
		}catch(Exception ex){closed=true;Close();return;}
		dbgprint("sn_w()");
		//cout<<"s2_w"<<endl;
		queue_out.GetPointer(sw_i).b.Release();
		queue_out.EndDequeue(sw_i);
		sending=false;
		BeginSend();
	}
	void socketmux::item::Close()
	{
		if(closed&&!closed_r)
		{
			if(!queue_in.CanDequeue())
			{
				//cout<<"sent cmd_close on item "<<id<<" [outgoing="<<this->outgoing<<"]"<<endl;
				closed_r=true;
				shared_ptr<socketmux> _owner(owner);
				_owner->__sendclose(this->id,this->outgoing);
				s->Close();
				map<int,shared_ptr<item> >& m=outgoing?_owner->items_o:_owner->items_i;
				m.erase(this->id);
				//Clear();
			}
		}
		else if(closed_r&&!closed)
		{
			if(!sending)
			{
				//cout<<"received cmd_close on item "<<id<<" [outgoing="<<this->outgoing<<"]"<<endl;
				closed=true;
				s->Close();
				shared_ptr<socketmux> _owner(owner);
				map<int,shared_ptr<item> >& m=outgoing?_owner->items_o:_owner->items_i;
				m.erase(this->id);
				//Clear();
			}
		}
	}
	void socketmux::item::Clear()
	{
		//return;
		shared_ptr<socketmux> _owner(owner);
		if(receiving) {queue_in.EndAppend(sr_i);}
		if(sending) {queue_out.GetPointer(sw_i).b.Release();queue_out.EndDequeue(sw_i);}
		int i;
		while((i=queue_in.BeginDequeue())>=0)
		{
			_owner->bm.Return(&(queue_in.GetPointer(i).b));
			queue_in.EndDequeue(i);
		}
		while((i=queue_out.BeginDequeue())>=0)
		{
			queue_out.GetPointer(i).b.Release();
			queue_out.EndDequeue(i);
		}
	}
	void socketmux::item::update_status()
	{
		bool tmp=queue_in.CanDequeue()&&!closed_r;
		if(tmp==hasData)return;
		if(tmp&&!hasData)
		{
			//add to linked list
			shared_ptr<socketmux> _owner(owner);
			iter=_owner->items_r.insert(_owner->items_r.end(),shared_ptr<item>(wptr));
		}
		else if(hasData&&!tmp)
		{
			//remove
			shared_ptr<socketmux> _owner(owner);
			_owner->items_r.erase(iter);
		}
		hasData=tmp;
	}
}


