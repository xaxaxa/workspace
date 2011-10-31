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

using namespace std;
using namespace net;

void cb(u_char *user, const pcap_pkthdr *header, const u_char *bytes);
void cb1(u_char *user, const pcap_pkthdr *header, const u_char *bytes);
void cb2(void* user, protocols::protoint& p, const void* hdr, protoid proto, const Buffer& data);
void cb3(void* user, protocols::protoint& p, const void* hdr, protoid proto, const Buffer& data);
protostack pstack;
int linktype;
protocols::protoint* p_int;
int main(int argc, char **argv)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* cap;
	if((cap=pcap_create(argv[1],errbuf))==NULL)
	{
		cerr << errbuf;
		return 1;
	}
	pcap_set_promisc(cap,1);
	pcap_set_timeout(cap,200);
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
			p_int=new protocols::ether();
			break;
		case DLT_LINUX_SLL:
			p_int=new protocols::sll();
			break;
		case DLT_RAW:
		default:
			p_int=NULL;
	}
	cerr << "linktype is "<<linktype<<endl;
	
	if(p_int==NULL)
	{
		pcap_loop(cap,-1,cb,NULL);
	}
	else
	{
		p_int->dataout+=protocols::protoint::datadelegate(&cb2,NULL);
		pcap_loop(cap,-1,cb1,NULL);
	}
	
	
	return 0;
}
void chkpacket(const pcap_pkthdr *header, const u_char *bytes)
{
	if(header->caplen < header->len)
		cerr << "warning: packet truncated from " << header->len << " to " << header->caplen << endl;
}
void cb(u_char *user, const pcap_pkthdr *header, const u_char *bytes)
{
	//from pcap
	chkpacket(header,bytes);
	//write(1,bytes,header->caplen);
	pstack.processPacket(0x0800,Buffer((void*)bytes,header->caplen));
}
void cb1(u_char *user, const pcap_pkthdr *header, const u_char *bytes)
{
	//from pcap
	chkpacket(header,bytes);
	p_int->putdata(Buffer((void*)bytes,header->caplen));
}
void cb2(void* user, protocols::protoint& p, const void* hdr, protoid proto, const Buffer& data)
{
	//from datalink processor
	pstack.processPacket(proto,data);
}

void cb3(void* user, protocols::protoint& p, const void* hdr, protoid proto, const Buffer& data)
{
	//from protocol stack
	write(1,data.Data,data.Length);
}
