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
#include <linux/tcp.h>
#include <pcap/sll.h>
#include <net/ethernet.h>
#include <map>
#include <string>

using namespace xaxaxa;
using namespace std;

namespace xaxaxa{
namespace net
{
	
	typedef Int protoid;
	constexpr protoid noproto=0;
	namespace protocols
	{
		class protoint;
		class tcpint;
		class ipint;
		class ethernetint;
		class protoint
		{
		public:
			DELEGATE(void,datadelegate,protoint&,const void*,protoid,const Buffer&);
			EVENT(datadelegate) dataout;
			virtual void putdata(const Buffer& b)=0;
			virtual string identify()=0;
		};

		class ether:public protoint
		{
			virtual void putdata(const Buffer& b)
			{
				ether_header* h=(ether_header*)b.Data;
				if((UInt)b.Length<sizeof(ether_header))
				{
					WARN(5,"Invalid ethernet packet; length="<<b.Length);
					return;
				}
				WARN(10,"received ethernet packet; length="<<b.Length<<"; ether_type="<<ntohs(h->ether_type));
				RAISEEVENT(dataout,*this,b.Data,(protoid)ntohs(h->ether_type),b.SubBuffer(sizeof(ether_header)));
			}
			virtual string identify(){return "Ethernet";}
		};
		class sll:public protoint
		{
			virtual void putdata(const Buffer& b)
			{
				sll_header* h=(sll_header*)b.Data;
				if((UInt)b.Length<sizeof(sll_header))
				{
					WARN(5,"Invalid linux cooked packet; length="<<b.Length);
					return;
				}
				WARN(10,"received linux cooked packet; length="<<b.Length<<"; sll_protocol="<<ntohs(h->sll_protocol));
				RAISEEVENT(dataout,*this,b.Data,(protoid)ntohs(h->sll_protocol),b.SubBuffer(sizeof(sll_header)));
			}
			virtual string identify(){return "Linux Cooked";}
		};
		class ip:public protoint
		{
			virtual void putdata(const Buffer& b)
			{
				iphdr* h=(iphdr*)b.Data;
				if(b.Length<1 || b.Length<h->ihl)
				{
					WARN(5,"Invalid IP packet");
					return;
				}
				RAISEEVENT(dataout,*this,h,h->protocol,b.SubBuffer(h->ihl));
			}
			virtual string identify(){return "Internet Protocol";}
		};

		class tcp:public protoint
		{
			virtual void putdata(const Buffer& b)
			{
				tcphdr* h=(tcphdr*)b.Data;
				if(b.Length<20)
				{
					WARN(5,"Invalid TCP packet");
					return;
				}
				RAISEEVENT(dataout,*this,h,noproto,b.SubBuffer(h->doff*4));
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
		void processPacket(protoid proto,const Buffer& b)
		{
			myiter it=p.find(proto);
			if(it==p.end()){WARN(5,"protocol " << proto << " not found");return;}
			(*it).second->putdata(b);
		}
		void cb(protocols::protoint& src,const void* header,protoid proto,const Buffer& b)
		{
			if(proto==noproto)
			{//reached application layer
				WARN(10,"application layer reached. last protocol was \""<<src.identify()<<"\".");
				RAISEEVENT(dataout,src,NULL,noproto,b);
			}
			else processPacket(proto,b);
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
