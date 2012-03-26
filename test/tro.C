#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cplib/cplib.hpp>

using namespace std;
using namespace xaxaxa;

int main(int argc, char** argv)
{
	/*if(argc<2)
	{
		printf("usage: %s file\n", argv[0]);
		return 1;
	}*/
	//FileStream fs(File(argv[1],O_RDONLY));
	//fs.Seek(14,Stream::SeekFrom::Begin);
	StandardStream ss;
	StreamReader sr(ss);
	StreamWriter& sw(sr);
	/*char buf[3];
	long long tmp=0;
	char tmp_index=0;
	Buffer b(buf,3);
	while(sr.Read(b)>0)
	{
		int i;
		for(i=0;i<3;i++)
		{
			char bit;
			bit=((buf[i] & 1)!=0)?1:0;
			//cout << (int)bit << endl;
			tmp|=(bit << tmp_index);
			tmp_index++;
			if(tmp_index>=64)
			{
				sw.Write(&tmp,8);
				tmp_index=0;
				tmp=0;
			}
		}
	}*/
	if(argc>1 && strcmp(argv[1],"-e")==0)
	{
		Buffer b(4096);
		int br;
		char buf[9];
		buf[8]=' ';
		while((br=ss.Read(b))>0)
		{
			for(int a=0;a<br;a++)
			{
				for(int i=0;i<8;i++)
					buf[7-i]=(b.Data[a]&(1 << i))!=0?'1':'0';
				sw.Write(buf,9);
			}
		}
		sw.WriteLine();
	}
	else
	{
		Buffer buf(4096);
		//char b[4096];
		char* b=(char*)buf.Data;
		long long tmp=0;
		int tmp_index=0;
		int br;
		while((br=ss.Read(buf))>0)
		{
			for(int i=0;i<br;i++)
			{
				char bit;
				if(b[i]=='0')bit=0;
				else if(b[i]=='1')bit=1;
				else continue;
				tmp|=((long long)bit << ((int)(tmp_index/8)*8+(7-(tmp_index%8))));
				//tmp|=(long long)bit << tmp_index;
				tmp_index++;
				if(tmp_index>=64)
				{
					//write(1,&tmp,8);
					sw.Write(&tmp,8);
					tmp_index=0;
					tmp=0;
				}
			}
		}
		if(tmp_index>0)
			sw.Write(&tmp,tmp_index/8);
	}
	sw.Close();
	/*char buf[8];
	while(read(0,buf,8)>0)
	{
		char tmp=0;
		for(register int i=0;i<8;i++)
		{
			char bit=buf[7-i]=='1'?1:0;
			tmp|=(bit << i);
		}
		write(1,&tmp,1);
	}*/
	
	//sw.Close();
	/*
	FILE* f=fopen(argv[1],"rb");
	FILE* f1=fdopen(1,"wb");
	char buf[3];
	long tmp=0;
	char tmp_index=0;
	//Buffer b(buf,3);
	while(fread(buf,3,1,f)>0)
	{
		int i;
		for(i=0;i<3;i++)
		{
			char bit;
			bit=((buf[i] & 1)!=0)?1:0;
			//cout << (int)bit << endl;
			tmp|=(bit << tmp_index);
			tmp_index++;
			if(tmp_index>=64)
			{
				//sw.Write(&tmp,8);
				fwrite(&tmp,8,1,f1);
				tmp_index=0;
				tmp=0;
			}
		}
	}*/
}
