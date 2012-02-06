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
	
	FileStream s(File("/tmp/data1",O_RDONLY));
	StreamReader r(s,1024*32);
	StringBuilder buffer;
	//NullStream ns;
	while(r.fast_readline(buffer)>=0)
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
	
	/*FILE* f=fdopen(0,"rb");
	char* line=NULL;
	size_t tmp;
	while(getline(&line,&tmp,f)>=0)
	{
		i++;
		free(line);
		line=NULL;
	}*/
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
