#define IS_UI
#include <generic_ui.H>
using namespace GenericUI::Aliases;
using GenericStruct::Types;
int x=0;
void init()
{
	GenericUI::attach("main").attach("asdfg").attach([](O o) -> O{
		cout << "ui1 #" << (++x) << endl;
		return O(1);
	});
	GenericUI::attach("main").attach("zxcvb").invoke(1);
}
