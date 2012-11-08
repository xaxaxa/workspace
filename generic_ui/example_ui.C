#include <iostream>
#include <string>
#include "generic_ui.H"
#include "cout_object.H"
using namespace GenericUI;
using namespace GenericUI::Aliases;
using namespace std;
int main()
{
	attach("main").attach("event1").attach([](O o) -> O {
		cout << "aaaaa" << endl;
		return O::undefined();
	});
	attach("main").attach("run").attach([](O o) -> O {
		while(true) {
			string s;
			getline(cin,s);
			//attach("main").attach("input").invoke(O());
		}
		return O::undefined();
	});
	//A a{O(2.1),O(123),O(A{5,6,7})};
	//A a{O(1),O(2.2),O(3),O(4),O(5),O({3,4,5}),O::undefined()};
	//A a1{O(A{1,2,3}),O(A{4,5,6}),O(3),O(1.23),O(A{A{A{245LL,123LL}}})};
	//auto arr=a.getArray();
	/*cerr << (int)a1.type() << endl;
	for(int i=0;i<a1.length();i++) {
		cerr << (int)a1[i].type << endl;//<< " " << a[i].getFloat64() << endl;
	}
	O o(a1);
	cerr << o << endl;
	o.serialize([](int8_t* buf, int len) {
		write(1,buf,len);
	});*/
	O tmp="fsck";
	cout << tmp.getArray().getArray<int8_t>() << endl;
	cout << tmp << endl;
	O tmp1{1LL,2LL,3LL};
	tmp1.getArray().append(22.9);
	cout << tmp1 << endl;
	return 0;
}
