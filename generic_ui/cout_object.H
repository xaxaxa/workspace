#ifndef __COUT_OBJECT_H
#define __COUT_OBJECT_H
#include <iostream>
#include <string>
#include "generic_ui.H"
using namespace GenericUI;
using namespace GenericStruct;
using namespace GenericUI::Aliases;
using namespace std;

template<class OSTREAM> OSTREAM& operator<<(OSTREAM& o, const Object& ob);

template<class OSTREAM> OSTREAM& operator<<(OSTREAM& o, const Array& a) {
	//o << "A<" << getTypeName(a.type());
	if(a.type()==Types::int8) {
		o << '\"' << a.getString() << '\"';
		return o;
	}
	o << '[';
	for(int i=0;i<a.length();i++) {
		if(i!=0) o << ", ";
		o << a[i];
	}
	o << ']';
	return o;
}
template<class OSTREAM> OSTREAM& operator<<(OSTREAM& o, const Object& ob) {
	//o << "O<" << Data::getTypeName(o.type) << ">"
	switch(ob.type) {
		case Types::int8:
		{
			o << (int)ob.getInt8();
			break;
		}
		case Types::int16:
		{
			o << ob.getInt16();
			break;
		}
		case Types::int32:
		{
			o << ob.getInt32();
			break;
		}
		case Types::int64:
		{
			o << ob.getInt64();
			break;
		}
		case Types::float32:
		{
			o << ob.getFloat32();
			break;
		}
		case Types::float64:
		{
			o << ob.getFloat64();
			break;
		}
		case Types::array:
		{
			o << ob.getArray();
			break;
		}
		case Types::undefined:
		{
			o << "undefined";
			break;
		}
		default:
		{
			o << "unknown";
			break;
		}
	}
	return o;
}
#endif
