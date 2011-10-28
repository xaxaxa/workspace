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

using namespace std;

void cb(u_char *user, const pcap_pkthdr *header, const u_char *bytes);


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
	pcap_loop(cap,-1,cb,NULL);
	return 0;
}
void cb(u_char *user, const pcap_pkthdr *header, const u_char *bytes)
{
	if(header->caplen < header->len)
		cerr << "warning: packet truncated from " << header->len << " to " << header->caplen << endl;
	write(1,bytes,header->caplen);
}

