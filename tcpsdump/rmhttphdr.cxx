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
	if(argc<2)
	{
		cout << "rmhttphdr - remove HTTP headers from file." << endl << "usage: rmhttphdr [-o DSTFILE] SRCFILE1 [SRCFILE2 ... SRCFILEN]" << endl << "if dstfile is omitted the source file will be overwritten." << endl;
		return 0;
	}
	Stream* out=NULL;
	const char* outfile=NULL;
	vector<const char*> files;
	for(int i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-o")==0)
		{
			if(argc<=++i)
				out=new FileStream(1);
			else outfile=argv[i];
			continue;
		}
		files.push_back(argv[i]);
	}
	
	int ret=0;
	for(unsigned int i=0;i<files.size();i++)
	{
		FileStream fs1(File(files[i],O_RDONLY));
		StreamReader sr(fs1);
		NullStream ns;
		int tmp1;
		while((tmp1=sr.Read(ns,new STRING[2]{"\r\n","\n"},2))>0);
		if(tmp1<0)
		{
			cerr << files[i] << ": could not find http header or there's no content" << endl;
			ret=1;
			continue;
		}
		int br;
		Buffer b(4096);
		if(outfile!=NULL)
		{
			unlink(outfile);
			out=new FileStream(File(outfile,O_WRONLY));
			outfile=NULL;
		}
		Stream* out1=out;
		if(out1==NULL)
		{
			unlink(files[i]);
			out1=new FileStream(File(files[i],O_WRONLY));
		}
		while((br=sr.Read(b))>0)out1->Write(b.SubBuffer(0,br));
	}
	return ret;
}

