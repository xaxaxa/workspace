#define IS_UI
#include <generic_ui.H>
using namespace GenericUI::Aliases;
using GenericStruct::Types;
void init()
{
	GenericUI::attach("main").attach("asdfg").attach([](O o) -> O{
		cout << "ui2" << endl;
		return O(2);
	});
	GenericUI::attach("main").attach("zxcvb").invoke(2);
}
