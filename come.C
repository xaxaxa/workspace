#include <iostream>
#include <vector>
#include <string>
#include <cryptopp/des.h>
#include <cryptopp/modes.h>

using namespace std;
using namespace CryptoPP;

vector<pair<const char*,int>> chars{{"QWERTYUIOP",10},{"ASDFGHJKL",9},{"ZXCVBNM",7}};
char translate(int n1,int n2)
{
	if(n2==0)n2=10;
	if(n2>chars[n1-1].second)return '\x00';
	return chars[n1-1].first[n2-1];
}

string plain="\xB6\xB2\xB6\xAC\xAC\xA6\xB0\xAA";
string cipher="\x05\xD9\x12\xE7\xCC\xD9\xBB\xCA";
/*void dotest(const vector<int>& k, vector<int>& cur_keys, size_t xxx)
{
	size_t i;
	int sx;
	if(xxx==cur_keys.size())
	{
		//test for stds
		char key[cur_keys.size()+1];
		key[xxx]='\x00';
		for(i=0;i<cur_keys.size();i++)
		{
			key[i]=translate(cur_keys[i],k[i]);
		}
		ECB_Mode<DES>::Encryption asdf;
		asdf.SetKey((byte*)key,cur_keys.size());
		char out[plain.size()];
		asdf.ProcessData((byte*)out,(byte*)plain.data(),plain.size());
		if(memcmp(out,cipher.data(),cipher.size())==0)
		{
			cout << key << endl;
			for(i=0;i<cur_keys.size();i++)
			{
				cout << cur_keys[i] << k[i] << " ";
			}
			cout << endl;
		}
		//cout << key << endl;
		return;
	}
	for(sx=1;sx<4;sx++)
	{
		cur_keys[xxx]=sx;
		dotest(k,cur_keys,xxx+1);
	}
}*/
ECB_Mode<DES>::Encryption asdf;
void dotest(vector<char>& cur_keys, size_t xxx)
{
	size_t i;
	int sx;
	if(xxx==cur_keys.size())
	{
		//test for stds
		char* key=&cur_keys[0];
		asdf.SetKey((byte*)key,cur_keys.size());
		char out[plain.size()];
		asdf.ProcessData((byte*)out,(byte*)plain.data(),plain.size());
		if(memcmp(out,cipher.data(),cipher.size())==0)
		{
			cout << key << endl;
			/*for(i=0;i<cur_keys.size();i++)
			{
				cout << cur_keys[i] << k[i] << " ";
			}
			cout << endl;*/
		}
		//cout << key << endl;
		return;
	}
	for(sx=1;sx<128;sx++)
	{
		cur_keys[xxx]=sx;
		dotest(cur_keys,xxx+1);
	}
}
int main()
{
	//vector<int> k{8,9,9,4,3,5,9,5};
	//vector<int> asdf(k.size());
	vector<char> key(8);
	dotest(key,0);
	//cout << plain << endl;
}
