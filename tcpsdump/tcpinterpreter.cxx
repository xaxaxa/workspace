/*
 * tcpinterpreter.cxx
 * 
 * Copyright 2011  <xaxaxa@xaxaxa>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>
#include <sys/types.h>
#include <linux/ip.h>
#include <netinet/tcp.h>
#include <pcap/sll.h>
#include <net/ethernet.h>
#include <map>
#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

using namespace xaxaxa;
using namespace std;

namespace xaxaxa{
namespace net
{
	
	typedef Int protoid;
	constexpr protoid noproto=0;
	struct packet;
	struct connection
	{
		virtual Byte largerThan(const connection& x) const=0;
		/*virtual bool operator==(const connection& x) const=0;
		virtual bool operator!=(const connection& x) const=0;
		virtual bool operator>(const connection& x) const=0;
		virtual bool operator<(const connection& x) const=0;
		virtual bool operator<=(const connection& x) const=0;
		virtual bool operator>=(const connection& x) const=0;*/
		
	};
	struct connection_ptr
	{
		boost::shared_ptr<connection> ptr;
		inline bool operator==(const connection_ptr& x) const
		{return this->ptr->largerThan(*(x.ptr))==0;}
		inline bool operator!=(const connection_ptr& x) const
		{return !(*this==x);}
		inline bool operator>(const connection_ptr& x) const
		{return this->ptr->largerThan(*(x.ptr))>0;}
		inline bool operator<(const connection_ptr& x) const
		{return this->ptr->largerThan(*(x.ptr))<0;}
		inline bool operator<=(const connection_ptr& x) const
		{return !(*this>x);}
		inline bool operator>=(const connection_ptr& x) const
		{return !(*this<x);}
	};
	namespace protocols
	{
		class protoint
		{
		public:
			DELEGATE(void,datadelegate,const packet& p);
			EVENT(datadelegate) dataout;
			virtual void putdata(const packet& p)=0;
			virtual string identify()=0;
			
			//for protocols with src and dst addresses only
			virtual Byte getAddrSize() const
			{return 0;}
			virtual void getAddr(const packet& p, Byte*& src, Byte*& dst) const
			{throw NotSupportedException();}
			
			//for connection oriented protocols only
			//CID=Connection IDentifier: a sequence of bytes that identifies the connection
			virtual void* getCID(const packet& p) const
			{return 0;}
			virtual Byte compareCID(void* v1, void* v2) const
			{throw NotSupportedException();}
		};
	}
	struct packet	//contains a *parsed* packet; raw packets should be represented by xaxaxa::Buffer
	{				//can be an actual packet OR a piece of data in a stream
		protocols::protoint* protocol;	//protocol of this packet, not the data within
		protoid dataprotocol;			//protocol of the data
		void* header;					//packet classes contain valid packets only. no need for a separate headerlen field
		Buffer data;
		packet* parent;
	};
	namespace protocols
	{
		class tcpint;
		class ipint;
		class ethernetint;
		
		
		class ether:public protoint
		{
			virtual void putdata(const packet& p)
			{
				const Buffer& b=p.data;
				const ether_header* h=(ether_header*)b.Data;
				if((UInt)b.Length<sizeof(ether_header))
				{
					WARN(5,"Invalid ethernet packet; length="<<b.Length);
					return;
				}
				WARN(10,"received ethernet packet; length="<<b.Length<<"; ether_type="<<ntohs(h->ether_type));
				RAISEEVENT(dataout,{this,(protoid)ntohs(h->ether_type),b.Data,b.SubBuffer(sizeof(ether_header)),(packet*)&p});
			}
			virtual string identify(){return "Ethernet";}
			virtual Byte getAddrSize() const
			{return 6;}
			virtual void getAddr(const packet& p, Byte*& src, Byte*& dst) const
			{
				const ether_header* h=(ether_header*)p.header;
				src=(Byte*)&h->ether_shost;
				dst=(Byte*)&h->ether_dhost;
			}
		};
		class sll:public protoint
		{
			virtual void putdata(const packet& p)
			{
				const Buffer& b=p.data;
				const sll_header* h=(sll_header*)b.Data;
				if((UInt)b.Length<sizeof(sll_header))
				{
					WARN(5,"Invalid linux cooked packet; length="<<b.Length);
					return;
				}
				WARN(10,"received linux cooked packet; length="<<b.Length<<"; sll_protocol="<<ntohs(h->sll_protocol));
				RAISEEVENT(dataout,{this,(protoid)ntohs(h->sll_protocol),b.Data,b.SubBuffer(sizeof(sll_header)),(packet*)&p});
			}
			virtual string identify(){return "Linux Cooked";}
		};
		class ip:public protoint
		{
			virtual void putdata(const packet& p)
			{
				const Buffer& b=p.data;
				iphdr* h=(iphdr*)b.Data;
				if(b.Length<1 || b.Length<h->ihl)
				{
					WARN(5,"Invalid IP packet");
					return;
				}
				RAISEEVENT(dataout,{this,(protoid)(h->protocol),h,b.SubBuffer(h->ihl),(packet*)&p});
			}
			virtual string identify(){return "Internet Protocol";}
			virtual Byte getAddrSize() const
			{return 4;}
			virtual void getAddr(const packet& p, Byte*& src, Byte*& dst) const
			{
				const iphdr* h=(iphdr*)p.header;
				src=(Byte*)&h->saddr;
				dst=(Byte*)&h->daddr;
			}
		};

		class tcp:public protoint
		{
			/*struct tcpconn
			{
				//A srcaddr;
				//A dstaddr;
				UShort srcport;
				UShort dstport;
				inline bool operator==(const tcpconn& x) const
				{return this->largerThan(x)==0;}
				inline bool operator!=(const tcpconn& x) const
				{return !(*this==x);}
				inline bool operator>(const tcpconn& x) const
				{return this->largerThan(x)>0;}
				inline bool operator<(const tcpconn& x) const
				{return this->largerThan(x)<0;}
				inline bool operator<=(const tcpconn& x) const
				{return !(*this>x);}
				inline bool operator>=(const tcpconn& x) const
				{return !(*this<x);}
				
			};*/
			struct tcpconn:connection
			{
				Byte addrlen;
				boost::shared_array<Byte> address;
				Byte* srcaddr;
				Byte* dstaddr;
				UShort srcport;
				UShort dstport;
				bool isRaw;
				#define __GET_SRCADDR(x,addrlen) (*((Byte(*)[addrlen])x))
				#define __GET_DSTADDR(x,addrlen) (*((Byte(*)[addrlen])(((Byte*)(x))+addrlen)))
				tcpconn():isRaw(false){}
				tcpconn(Byte addrlen, Byte* src, Byte* dst, bool raw=false):isRaw(raw)
				{
					if(raw)
					{
						srcaddr=src;
						dstaddr=dst;
					}
					else
					{
						address=boost::shared_array<Byte>(new Byte[addrlen*2]);
						Byte* addr=address.get();
						memcpy(addr,src,addrlen);
						memcpy(addr+addrlen,dst,addrlen);
					}
				}
				virtual Byte largerThan(const connection& x) const
				{
					const tcpconn& x2=dynamic_cast<const tcpconn&>(x);
					if(addrlen>x2.addrlen)return 1;
					if(addrlen<x2.addrlen)return -1;
					const Byte* saddr=isRaw?srcaddr:((Byte*)this->address.get());
					const Byte* saddr2=x2.isRaw?x2.srcaddr:((Byte*)x2.address.get());
					const Byte* daddr=isRaw?dstaddr:(saddr+addrlen);
					const Byte* daddr2=x2.isRaw?x2.dstaddr:(saddr2+x2.addrlen);
					//Byte (&srcaddr)[addrlen]=*((Byte(*)[addrlen])dataaddr);
					if(__GET_SRCADDR(saddr,addrlen)>__GET_SRCADDR(saddr2,addrlen))return 1;
					if(__GET_SRCADDR(saddr,addrlen)<__GET_SRCADDR(saddr2,addrlen))return -1;
					if(__GET_SRCADDR(daddr,addrlen)>__GET_SRCADDR(daddr2,addrlen))return 1;
					if(__GET_SRCADDR(daddr,addrlen)<__GET_SRCADDR(daddr2,addrlen))return -1;
					
					/*
					if(srcaddr<tmp.srcaddr)return -1;
					if(dstaddr>tmp.dstaddr)return 1;
					if(dstaddr<tmp.dstaddr)return -1;*/
					if(srcport>x2.srcport)return 1;
					if(srcport<x2.srcport)return -1;
					if(dstport>x2.dstport)return 1;
					if(dstport<x2.dstport)return -1;
					WARN(9,"tcpconn::largerThan returned zero");
					return 0;
				}
				/*virtual bool operator==(const tcpconn_ptr& x) const
				{return this->ptr->largerThan(*(x.ptr))==0;}
				virtual bool operator!=(const tcpconn_ptr& x) const
				{return !(*this==x);}
				virtual bool operator>(const tcpconn_ptr& x) const
				{return this->ptr->largerThan(*(x.ptr))>0;}
				virtual bool operator<(const tcpconn_ptr& x) const
				{return this->ptr->largerThan(*(x.ptr))<0;}
				virtual bool operator<=(const tcpconn_ptr& x) const
				{return !(*this>x);}
				virtual bool operator>=(const tcpconn_ptr& x) const
				{return !(*this<x);}*/
			};
			class tcpbuffer
			{
			public:
				DELEGATE(void, datadelegate, tcpbuffer& src, const Buffer& b);
				datadelegate dataout;
				struct item
				{
					ULong seq;
					Buffer data;
				};
				list<item> items;
				ULong seq;//the expected sequence number of next packet
				bool initseq;
				tcpbuffer() : initseq(false)
				{
					
				}
				void processData(ULong seq, const Buffer& datain)
				{
					if(!initseq)this->seq=seq;
					if(seq>this->seq)
					{
						item tmp;
						tmp.seq=seq;
						tmp.data=Buffer(datain.Length);
						memcpy(tmp.data.Data,datain.Data,datain.Length);
						items.push_back(tmp);
					}
					else if((seq+datain.Length)>this->seq)
					{
						const Buffer& b=(seq==this->seq)?datain:datain.SubBuffer((seq+datain.Length)-this->seq);
						CALL(dataout,*this,b);
						this->seq=seq+datain.Length;
						list<item>::iterator it=items.begin();
						while(it!=items.end())
						{
							item& tmp=*it;
							if((tmp.seq<=this->seq))
							{
								if((tmp.seq+tmp.data.Length)>this->seq)
								{
									const Buffer& b1=(tmp.seq==this->seq)?tmp.data:tmp.data.SubBuffer((tmp.seq+tmp.data.Length)-this->seq);
									CALL(dataout,*this,b1);
								}
								items.erase(it); it=items.begin();
							} else it++;
						}
					}
				}
			};
			map<connection_ptr,tcpbuffer> connections;
			typedef map<connection_ptr,tcpbuffer>::iterator iter;
			void tcpbuf_cb(tcpbuffer& src, const Buffer& b)
			{
				RAISEEVENT(dataout,{this,noproto,&src,b,NULL});
			}
			virtual void putdata(const packet& p)
			{
				const Buffer& b=p.data;
				tcphdr* h=(tcphdr*)b.Data;
				if(b.Length<20)
				{
					WARN(5,"Invalid TCP packet");
					return;
				}
				packet* pack=p.parent;
				int addrlen;
				while(true)
				{
					if(pack==NULL || pack->protocol==NULL)
					{WARN(7,"could not find src address and dst address for tcp packet");return;}
					if((addrlen=pack->protocol->getAddrSize())>0)break;
				}
				//Byte tmpbuf[sizeof(tcpconn)+(addrlen*2)];
				Byte* srcaddr; Byte* dstaddr;
				pack->protocol->getAddr(*pack,srcaddr,dstaddr);
				tcpconn* con=new tcpconn(addrlen,srcaddr,dstaddr,false);//malloc(sizeof(tcpconn)+(addrlen*2));
				//memcpy(con+1,srcaddr,addrlen);
				//memcpy(((Byte*)con+1)+addrlen,dstaddr,addrlen);
				con->srcport=h->source;
				con->dstport=h->dest;
				connection_ptr cptr={boost::shared_ptr<connection>(con)};
				iter it=connections.find(cptr);
				if(it==connections.end())
				{
					tcpconn* con1=new tcpconn(addrlen,srcaddr,dstaddr);//(tcpconn*)malloc(sizeof(tcpconn)+(addrlen*2));
					//memcpy(con1,con,sizeof(tmpbuf));
					con1->srcport=h->source;
					con1->dstport=h->dest;
					cptr.ptr=boost::shared_ptr<connection>(con1);
					WARN(9,"added new connection");
				} else 
				{
					WARN(9,"found existing connection; sport="<<h->source<<"; dport="<<h->dest);
				}
				tcpbuffer& tcpbuf=connections[cptr];
				if(FUNCTION_ISNULL(tcpbuf.dataout))tcpbuf.dataout=tcpbuffer::datadelegate(&tcp::tcpbuf_cb,this);
				tcpbuf.processData(h->seq,b);
				//RAISEEVENT(dataout,{this,noproto,h,b.SubBuffer(h->doff*4),(packet*)&p});
				//CALL(dataout,NULL,b);
			}
			virtual string identify(){return "Transmission Control Protocol";}
		};
	}
	class protostack
	{
	public:
		map<protoid,protocols::protoint*> p;
		typedef map<protoid,protocols::protoint*>::iterator myiter;
		typedef pair<protoid,protocols::protoint*> proto_pair;
		EVENT(protocols::protoint::datadelegate) dataout;
		void processPacket(const packet& pack)
		{
			myiter it=p.find(pack.dataprotocol);
			if(it==p.end()){WARN(10,"protocol " << pack.dataprotocol << " not found");return;}
			(*it).second->putdata(pack);
		}
		void cb(const packet& p)
		{
			if(p.dataprotocol==noproto)
			{//reached application layer
				WARN(10,"application layer reached. last protocol was \""<<p.protocol->identify()<<"\".");
				RAISEEVENT(dataout,p);
			}
			else processPacket(p);
		}
		protostack()
		{
			protocols::protoint* tmp;
			protocols::protoint::datadelegate del(&protostack::cb,this);
			tmp=new protocols::ether();tmp->dataout+=del;
			p.insert(proto_pair(97,tmp));
			tmp=new protocols::ip();tmp->dataout+=del;
			p.insert(proto_pair(0x0800,tmp));
			tmp=new protocols::tcp();tmp->dataout+=del;
			p.insert(proto_pair(6,tmp));
		}
		~protostack()
		{
			myiter it;
			myiter end=p.end();
			for(it=p.begin();it!=end;it++)
			{
				delete (*it).second;
			}
		}
	};

}}
