#include <iostream>
#include <string>
#include "generic_ui.H"
#include "cout_object.H"
using namespace GenericUI;
using namespace GenericUI::Aliases;
using namespace std;
int main()
{
	O o;
	o.deserialize([](int8_t* buf, int len) {
		read(0,buf,len);
	});
	/*A a1=o.getArray();
	
	cerr << (int)a1.type() << endl;
	for(int i=0;i<a1.length();i++) {
		cout << (int)a1[i].type << endl;//<< " " << a[i].getFloat64() << endl;
	}*/
	cout << o << endl;
	return 0;
}
