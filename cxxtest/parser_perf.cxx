/*
 * parser_perf.cxx
 * 
 * Copyright 2012 xaxatrollo <xaxatrollo@xaxatrolloPC>
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
#include <cplib/cplib.hpp>
#include <functional>
#include <list>
using namespace std;
using namespace xaxaxa;
class parser
{
	Property<Stream> s;
	int seq;
	list<Buffer> buf;
	inline char operator[](int i)
	{
		if(seq-i<=(*(buf.end()-1)).Length)
			return (*(buf.end()-1)).Data[(i-(seq-*(buf.end()-1)).Length)];
	}
}
void do_per_char(Stream& s, const function<bool(int)>& func)
{
	Buffer b(4096);
	int br;
	int seq=0;
	while((br=s.Read(b))>0)
	{
		int x=seq+br;
		for(int i=seq;i<x;i++)
			func(i);
		seq=x;
	}
}

int main(int argc, char **argv)
{
	StandardStream ss;
	do_per_char([](int i)
	{
		
	});
	return 0;
}

