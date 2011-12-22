/*
 * rmhttphdr.cxx
 * 
 * Copyright 2011  <xaxaxa@xaxaxa>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

using namespace std;
using namespace xaxaxa;
int main(int argc, char **argv)
{
	string ifile,ofile;
	if(argc<2)
	{
		cout << "rmhttphdr - remove HTTP headers from file." << endl << "usage: rmhttphdr SRCFILE [DSTFILE]" << endl << "if dstfile is omitted the source file will be overwritten." << endl;
		return 0;
	}
	ifile=string(argv[1]);
	if(argc>=3)ofile=string(argv[2]);
	else ofile=ifile;
	
	FileStream fs1(fopen(ifile.c_str(),"rb"));
	StreamReader sr(fs1);
	NullStream ns;
	int tmp1;
	while((tmp1=sr.Read(ns,new STRING[2]{"\r\n","\n"},2))>0);
	if(tmp1<0)
	{
		cerr << ifile << ": could not find http header or there's no content" << endl;
		return 1;
	}
	int br;
	Buffer b(4096);
	unlink(ofile.c_str());
	FileStream fs2(fopen(ofile.c_str(),"wb"));
	while((br=sr.Read(b))>0)fs2.Write(b.SubBuffer(0,br));
	return 0;
}

