#include "cpoll.H"
using namespace std;
using namespace CP;
int main()
{
	File f(0); //stdin
	char buf[4096];
	f.read(buf, 4096, [](int br){cerr << br << " bytes read" << endl;});
	
	File f2(1); //stdout
	f2.write("aaaaa\n", 6, [](int bw){cerr << bw << " bytes written" << endl;});
	//f.loop();
	Poll p;
	p.add(f);
	p.add(f2);
	p.loop();
	
	/*File f(0); //stdin
	char buf[4096];
	f.read(buf,4096,[](int br){cout << "read " << br << endl;});
	Poll poll;
	poll.add(f);
	Poll poll2;
	poll2.add(poll);
	poll2.loop();*/
}
