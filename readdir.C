#include <dirent.h>
#include <iostream>
#include <assert.h>
using namespace std;
int main(int argc, char** argv)
{
	DIR* d=opendir(".");
	assert(d!=NULL);
	dirent* ent;
	while((ent=readdir(d))!=NULL)
	{
		cout << ent->d_name << endl;
	}
}
