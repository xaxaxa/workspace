
#include <cplib/cplib.hpp>
#include <iostream>

using namespace std;
using namespace xaxaxa;
int main()
{
	/*int asdf;
	cin >> asdf;
	//int x[6]={1,2,3,4,5,6};
	//int y[6]={6,5,4,3,2,1};
	int x[asdf];
	int y[asdf];
	cerr << (x>y) << endl;*/
	CircularQueue<int> tmp(3,2);
	auto i=tmp.BeginAppend();
	tmp.GetPointer(i)=123;
	tmp.EndAppend(i);
	i=tmp.BeginAppend();
	tmp.EndAppend(i);
	cout << tmp.BeginDequeue() << endl;
	cout << tmp.BeginDequeue() << endl;
}
