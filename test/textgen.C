#include <iostream>

using namespace std;
void gen_num(ostream &s, int digits)
{
	int i;
	for(i=1;i<=digits;i++)
	{
		int n=0;
		int max=10^i;
		do
		{
			s << n << endl;
		}
		while((++n)<max);
	}
}
int main()
{
	gen_num(cout,5);
}
