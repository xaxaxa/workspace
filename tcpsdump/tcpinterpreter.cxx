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
#include <net/ethernet.h>
#include <map>
using namespace xaxaxa;
using namespace std;

namespace xaxaxa{
namespace net
{
	class protoint;
	class tcpint;
	class ipint;
	class ethernetint;
	typedef Int protoid;
	constexpr protoid noproto=0;
	class protoint
	{
	public:
		DELEGATE(void,datadelegate,protoint&,const void*,protoid,const Buffer&);
		EVENT(datadelegate) dataout;
		virtual void putdata(const Buffer& b)=0;
	};

	class ethernetint:public protoint
	{
		virtual void putdata(const Buffer& b)
		{
			ether_header* h=(ether_header*)b.Data;
			if((UInt)b.Length<sizeof(ether_header))return;
			RAISEEVENT(dataout,*this,b.Data,h->ether_type,b.SubBuffer(sizeof(ether_header)));
		}
	};
	class ipint:public protoint
	{
		virtual void putdata(const Buffer& b)
		{
			iphdr* h=(iphdr*)b.Data;
			if(b.Length<1)return;
			if(b.Length<h->ihl)return;
			RAISEEVENT(dataout,*this,h,h->protocol,b.SubBuffer(h->ihl));
		}
	};

	class tcpint:public protoint
	{
		virtual void putdata(const Buffer& b)
		{
			//CALL(dataout,NULL,b);
		}
	};
	
	class protostack
	{
	public:
		map<protoid,protoint*> protocols;
		typedef map<protoid,protoint*>::iterator myiter;
		typedef pair<protoid,protoint*> proto_pair;
		protoint::datadelegate dataout;
		void processPacket(protoid proto,const Buffer& b)
		{
			myiter it=protocols.find(proto);
			if(it==protocols.end()){WARN(5,"protocol " << proto << "not found");return;}
			(*it).second->putdata(b);
		}
		void cb(protoint& src,const void* header,protoid proto,const Buffer& b)
		{
			if(proto==noproto)
			{//reached application layer
				CALL(dataout,src,NULL,noproto,b);
			}
			else processPacket(proto,b);
		}
		protostack()
		{
			protoint* tmp;
			protoint::datadelegate del(&protostack::cb,this);
			tmp=new ipint();tmp->dataout+=del;
			protocols.insert(proto_pair(0x0800,tmp));
			tmp=new tcpint();tmp->dataout+=del;
			protocols.insert(proto_pair(6,tmp));
		}
		~protostack()
		{
			myiter it;
			myiter end=protocols.end();
			for(it=protocols.begin();it!=end;it++)
			{
				delete (*it).second;
			}
		}
	};

}}
