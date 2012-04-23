#include <iostream>
#include <list>
#include <vector>
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;

int main()
{
	const int buffers=50000000;
	
	cout << "constructing buffers" << endl;
	Buffer* b=new Buffer[buffers];
	cout << "done" << endl;
	cout << "creating buffers" << endl;
	//for(int i=0;i<buffers;i++)
	//	b[i]=Buffer(4096);
	cout << "done" << endl;
	cout << "copying buffer references" << endl;
	BufferRef* b2=new BufferRef[buffers];
	for(int i=0;i<buffers;i++)
		b2[i]=b[i];
	cout << "done" << endl;
	cout << "performing buffer clipping" << endl;
	for(int i=0;i<buffers;i++)
		b2[i].Clip(0);
	cout << "done" << endl;
	
	/*cout << "constructing buffers" << endl;
	char** b=new char*[buffers];
	cout << "done" << endl;
	cout << "creating buffers" << endl;
	//for(int i=0;i<buffers;i++)
	//	b[i]=new char[4096];
	cout << "done" << endl;
	cout << "copying buffer references" << endl;
	char** b2=new char*[buffers];
	for(int i=0;i<buffers;i++)
		b2[i]=b[i];
	cout << "done" << endl;
	cout << "performing buffer clipping" << endl;
	for(int i=0;i<buffers;i++)
		b2[i]+=100;
	cout << "done" << endl;*/
}
