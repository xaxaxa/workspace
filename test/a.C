#include <iostream>
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;

int main(int c, char** v)
{
	int i(0);
	//StandardStream ss;
	
	/*StringBuilder s;
	for(i=0;i<1000000;i++)
	{
		s << "asdfghjklasdfghjk" << i;
	}
	s.Seek(0);
	i=0;
	
	StreamReader r(s);
	StringBuilder buffer;
	STRING delim[2]{"asdfghjkl","asdf"};
	while(r.Read(buffer,delim,2)>=0)
	{
		//ss.Write(buffer.ToBuffer());
		//ss.Write("\n");
		
		i++;
		buffer.Clear();
	}*/
	StreamReader r(*(newobj<StandardStream>()),1024*32);
	StringBuilder buffer;
	//int i2(0);
	/*struct:public Stream
	{
		virtual void Write(const BufferRef& buf)
		{
			//int tmp1=i2+1;
			//tmp1++;
		}
		virtual Int Read(const BufferRef& buf){return 0;}
		virtual void Close(){}
		virtual void Flush(){}
	} tmp_s;
	//FileStream s(File("/tmp/data1",O_RDONLY));
	StandardStream s;
	
	
	//NullStream ns;
	while(r.fast_readline(tmp_s)>=0)*/
	STRING delim[1]
			{ "as" };
	while(r.Read(buffer, delim, 1)>=0)
	{
		//r.Write(buffer.ToBuffer());
		//r.Write("\n");
		i++;
		buffer.Clear();
	}
	//r.Flush();
	
	/*string tmp;
	while(std::getline(cin,tmp))
	{
		//cout << tmp << "\n";
		i++;
	}*/
	
	/*
	FILE* f=fdopen(0,"rb");
	char* line=NULL;
	size_t tmp;
	while(getline(&line,&tmp,f)>=0)
	{
		i++;
		free(line);
		line=NULL;
	}
	//*/
	/*StandardStream s;
	StreamWriter w(s);
	for(i=0;i<10000000;i++)
	{
		//cout << i << "\n";
		//printf("%i\n",i);
		w << i << "\n";
	}
	w.Flush();*/
	cout << i << endl;
}
