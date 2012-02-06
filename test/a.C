#include <iostream>
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;

int main(int c, char** v)
{
	int i(0);
	StandardStream s;
	StreamReader r(s);
	StringBuilder buffer;
	//NullStream ns;
	while(r.ReadLine(buffer)>=0)
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
		cout << tmp << "\n";
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
