#include <generic_ui.H>
using namespace GenericUI::Aliases;
using GenericStruct::Types;
using namespace GenericUI;
int main()
{
	load("./libui1.so");
	Instance i2;
	i2.load("./libui2.so");
	Instance i3;
	i3.load("./libui1.so");
	attach("main").attach("asdfg").invoke(O::undefined());
	i2.attach("main").attach("asdfg").invoke(O::undefined());
	i3.attach("main").attach("asdfg").invoke(O::undefined());
}
