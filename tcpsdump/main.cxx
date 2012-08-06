/*
 * main.cxx
 * 
 * Copyright 2011 Unknown <s-wang_cary@client-101>
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

#define WARNLEVEL 3
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h> /* if this gives you an error try pcap/pcap.h */
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include "tcpinterpreter.cxx"
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <cplib/cplib.hpp>
#include <pthread.h>
#include <time.h>

using namespace std;
using namespace net;
using namespace xaxaxa;

void cb(u_char *user, const pcap_pkthdr *header, const u_char *bytes);
void cb1(u_char *user, const pcap_pkthdr *header, const u_char *bytes);
void cb2(void* user, const protocols::Event& e);
void cb3(void* user, const protocols::Event& e);
protostack pstack;
int linktype;
protocols::protoint* p_int;
pcap_t* cap;
EventQueue<Buffer> q;
void* processorThread(void* v);
int main(int argc, char **argv)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	if((cap=pcap_create(argv[1],errbuf))==NULL)
	{
		cerr << errbuf;
		return 1;
	}
	pcap_set_promisc(cap,1);
	pcap_set_timeout(cap,500);
	pcap_set_snaplen(cap,65535);
	pcap_set_buffer_size(cap,1024*1024*8);
	int ret=pcap_activate(cap);
	if(ret&PCAP_ERROR)
	{
		pcap_perror(cap,(char*)"error opening capture device");
		return 1;
	}
	//capture
	//pcap_pkthdr* packetheader;
	//u_char* packetdata;
	
	/*while(true)
	{
		int ret=pcap_next_ex(cap,&packetheader,(const u_char**)&packetdata);
		if(ret==-1)
		{
			pcap_perror(cap,(char*)"error in pcap_next_ex()");
		}
	}*/
	pstack.dataout+=protocols::protoint::datadelegate(&cb3,NULL);
	linktype=pcap_datalink(cap);
	switch(linktype)
	{
		case DLT_EN10MB:
		case DLT_IEEE802:
			WARN(6,"selected protocols::ether as initial protocol interpreter");
			p_int=new protocols::ether();
			break;
		case DLT_LINUX_SLL:
			WARN(6,"selected protocols::sll as initial protocol interpreter");
			p_int=new protocols::sll();
			break;
		case DLT_RAW:
		default:
			WARN(6,"selected protocols::ip as initial protocol interpreter");
			p_int=NULL;
	}
	cerr << "linktype is "<<linktype<<endl;
	setuid(getuid()); //drop root privileges
	
	if(p_int!=NULL)
		p_int->dataout+=protocols::protoint::datadelegate(&cb2,NULL);
	pthread_t thr;
	pthread_create(&thr,NULL,processorThread,NULL);
	pcap_loop(cap,-1,cb,NULL);
	
	return 0;
}
void chkpacket(const pcap_pkthdr *header, const u_char *bytes)
{
	if(header->caplen < header->len)
		cerr << "warning: packet truncated from " << header->len << " to " << header->caplen << endl;
}
UInt pktrecv=0;
UInt pktdrp1=0;
UInt pktdrp2=0;
void chkdrop()
{
	pcap_stat st;
	if(pcap_stats(cap,&st)==0)
	{
		if(st.ps_drop>pktdrp1 || st.ps_ifdrop>pktdrp2)
		{
			WARN(1,"PACKET DROP DETECTED: kernel="<<st.ps_drop-pktdrp1<<" interface="<<st.ps_ifdrop-pktdrp2<<" received="<<st.ps_recv-pktrecv<<"; TOTAL: kernel="<<st.ps_drop<<" interface="<<st.ps_ifdrop<<" received="<<st.ps_recv);
			pktdrp1=st.ps_drop;pktdrp2=st.ps_ifdrop;pktrecv=st.ps_recv;
		}
	}
}
void cb(u_char *user, const pcap_pkthdr *header, const u_char *bytes)
{
	//from pcap
	chkpacket(header,bytes);
	//write(1,bytes,header->caplen);
	//packet p={NULL,0x0800,NULL,Buffer((void*)bytes,header->caplen),NULL};
	//pstack.processPacket(p);
	void* mem=malloc(header->caplen);
	Buffer tmpb(mem,header->caplen);
	memcpy(mem,bytes,header->caplen);
	q.Append(tmpb);
}

void processPacket(Buffer b)
{
	if(p_int==NULL)
	{
		packet p={NULL,NULL,b,0x0800};
		pstack.processPacket(p);
	}
	else
	{
		packet p={NULL,NULL,b,linktype};
		p_int->putdata(p);
	}
}
void* processorThread(void* v)
{
	time_t t=time(NULL);
	WARN(2,"started I/O thread");
	while(true)
	{
		Buffer b=q.Dequeue();
		//WARN(3,"I/O thread received packet");
		processPacket(b);
		time_t tmp=time(NULL);
		if(tmp>t)
		{
			chkdrop();
			t=tmp;
		}
		free(b.Data);
	}
	return NULL;
}

void cb2(void* user, const protocols::Event& e)
{
	//from datalink processor
	if(e.Type!=protocols::EventType::Data)
		return;
	const protocols::DataEvent& e1
		= (const protocols::DataEvent&)e;
	pstack.processPacket(e1.Data);
}
Int newfilen=0;
char* getNewFile()
{
	struct stat st;
	//Int tmp=newfilen;
	char* fn=new char[16];
	do
	{
		newfilen++;
		sprintf(fn,"s%06i",newfilen);
	} while(stat(fn,&st)==0);
	//newfilen=tmp;
	return fn;
}

map<connection_ptr,int> files;
typedef map<connection_ptr,int>::iterator files_iter;
void cb3(void* user, const protocols::Event& e)
{
	//from protocol stack
	WARN(5,"event received; type=" << protocols::EventType_[(int)e.Type]);
	if(e.Type!=protocols::EventType::DataWithConnection)
		return;
	const protocols::DataEventWithConnection& e1
		= (const protocols::DataEventWithConnection&)e;
	const packet& p=e1.Data;
	//get connection
	connection_ptr conn{(connection*)&e1.Connection};
	//const packet* pack=&p;
	
	/*while(true)
	{
		if(pack->protocol->getConnection(p,conn))break;
		pack=pack->parent;
		if(pack==NULL || pack->protocol==NULL)return;
	}*/
	//write(1,p.data.Data,p.data.Length);
	files_iter it=files.find(conn);
	int file;
	if(it==files.end())
	{
		//char* fn;
		//fn=getNewFile();
		Byte* src; Byte* dst;
		p.protocol->getAddr(p,src,dst);
		stringstream s;
		s << "s_" << p.protocol->getAddressString(p,src) << ":" << p.protocol->getSrcPort(p)
		<< "-" << p.protocol->getAddressString(p,dst) << ":" << p.protocol->getDstPort(p);
		file=open(s.str().c_str(),O_CREAT|O_WRONLY|O_TRUNC,0666);
		files[conn]=file;
		//delete[] fn;
	} else file=(*it).second;
	write(file,p.data.Data,p.data.Length);
}
