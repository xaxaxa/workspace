#include <iostream>
#include <list>
#include <vector>
#include <cplib/cplib.hpp>
using namespace std;


int main()
{
	/*list<int> l;
	int i;
	cout << "appending" << endl;
	for(i=0;i<256*256*256;i++)
		l.push_back(i);
	cout << "done." << endl;
	cout << " traversing list." << endl;
	for(auto it=l.begin();it!=l.end();it++)
	{
		int tmp=*it;
		if(tmp<0)break;
	}
	cout << "done." << endl;
	cout << "deleting list." << endl;
	while(true)
	{
		auto it=l.begin();
		if(it==l.end())break;
		l.erase(it);
	}
	cout << "done" << endl;*/
	
	/*list< vector<int> > l2;
	int i;
	cout << "appending" << endl;
	for(i=0;i<4096;i++)
	{
		l2.push_back(vector<int>());
		auto it=l2.end();
		it--;
		auto& v=*it;
		for(int ii=0;ii<4096;ii++)
			v.push_back(i*4096+ii);
	}
	cout << "done; traversing list." << endl;
	for(auto it=l2.begin();it!=l2.end();it++)
		for(int i=0;i<(int)(*it).size();i++)
		{
			int tmp=(*it)[i];
			if(tmp<0)break;
		}*/
	
	
	
	/*int i;
	xaxaxa::vectorlist<int> l(32);
	cout << "appending" << endl;
	for(i=0;i<256*256*256;i++)
		l.push_back(i);
	cout << "done." << endl;
	cout << " traversing list." << endl;
	for(auto it=l.begin();!it.is_end();it++)
	{
		int tmp=*it;
		if(tmp<0)break;
	}
	cout << "done." << endl;
	cout << "deleting list." << endl;
	while(true)
	{
		auto it=l.begin();
		if(it.is_end())break;
		l.erase(it);
	}
	cout << "done" << endl;*/
	
	//while(1)sleep(1);
}
