/*
 * generic_struct.C
 *
 *  Created on: 2012-11-02
 *      Author: xaxaxa
 */

#include "generic_struct.H"

namespace GenericStruct
{
	/*struct dataWriter
	 {
	 function<void(const int8_t*, int32_t)> onWrite;
	 inline void writeInt8(int8_t i) {}
	 };*/
	const char* TypeNames[10] = { "undefined", "int8", "int16", "int32", "int64", "float32",
			"float64", "dict", "array", "object" };
	const char* getTypeName(Types type) {
		if ((int32_t) type > 9) return "unknown";
		return TypeNames[(int32_t) type];
	}
	uint64_t htonll(uint64_t value) {
		// The answer is 42
		static const int32_t num = 42;

		// Check the endianness
		if (*reinterpret_cast<const char*>(&num) == num) {
			const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
			const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

			return (static_cast<uint64_t>(low_part) << 32) | high_part;
		} else {
			return value;
		}
	}
	uint64_t ntohll(uint64_t value) {
		// The answer is 42
		static const int32_t num = 42;

		// Check the endianness
		if (*reinterpret_cast<const char*>(&num) == num) {
			const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
			const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

			return (static_cast<uint64_t>(low_part) << 32) | high_part;
		} else {
			return value;
		}
	}
	TypeException::TypeException() throw () :
			msg("Type conversion exception occured") {

	}
	TypeException::TypeException(string msg) throw () :
			msg(msg) {

	}
	TypeException::TypeException(Types t_from, Types t_to) throw () {
		msg = "Error converting from " + string(getTypeName(t_from)) + " to "
				+ string(getTypeName(t_to));
	}
	const char* TypeException::what() const throw () {
		return msg.c_str();
	}
	TypeException::~TypeException() throw () {

	}
	Array::Array(const Array& other) :
			arr(other.arr) {
		if (arr != NULL) arr->retain();
	}
	Array::Array(ArrayBase* a) :
			arr(a) {
		if (arr != NULL) arr->retain();
	}
	Array::Array(const char* str) {
		arr = new ArrayType<int8_t>((int8_t*) str, strlen(str) + 1);
	}
	Array::Array(Types type) {
		switch (type) {
			case Types::int8:
				arr = new ArrayType<int8_t>();
				break;
			case Types::int16:
				arr = new ArrayType<int16_t>();
				break;
			case Types::int32:
				arr = new ArrayType<int32_t>();
				break;
			case Types::int64:
				arr = new ArrayType<int64_t>();
				break;
			case Types::float32:
				arr = new ArrayType<float>();
				break;
			case Types::float64:
				arr = new ArrayType<double>();
				break;
			case Types::array:
				arr = new ArrayType<Array>();
				break;
			case Types::object:
				arr = new ArrayType<Object>();
				break;
			default:
				throw TypeException("cannot create an array of type " + string(getTypeName(type)));
		}
	}
	Array::Array() :
			arr(NULL) {
	}
	Types Array::type() const {
		return arr->type;
	}
	Array& Array::operator=(const Array& other) {
		if (arr != NULL) arr->release();
		arr = other.arr;
		if (arr != NULL) arr->retain();
		return *this;
	}
	void Array::length(int32_t newlen) {
		if (arr == NULL) return;
		arr->length(newlen);
	}
	int32_t Array::length() const {
		if (arr == NULL) return 0;
		return arr->length();
	}
	void Array::append(const GenericStruct::Object& o) {
		arr->append(o);
	}
	Object Array::operator[](int32_t i) const {
		return (*arr)[i];
	}
	Array::~Array() {
		if (arr != NULL) arr->release();
	}
	char* Array::getString() const {
		if((*arr)[arr->length()-1].toInt8()!=0)
			arr->append(Object((int8_t)0));
		return (char*) &(((ArrayType<int8_t>*) arr)->arr[0]);
	}
	
	void Array::serialize(const function<void(int8_t*, int32_t)>& cb) const {
		if (arr == NULL) {
			Types t = Types::undefined;
			cb((int8_t*) t, 1);
		} else {
			arr->serialize(cb);
		}
	}
	void Array::deserialize(const function<void(int8_t*, int32_t)>& cb) {
		if (arr != NULL) arr->release();
		ArrayBase::deserialize(arr, cb);
	}

	void __object_do_release_if_needed(Object* o) {
		if (o->type == Types::array && o->data.array != NULL) o->data.array->release();
	}

	int8_t Object::getInt8() const {
		return data.int8;
	}
	int16_t Object::getInt16() const {
		return data.int16;
	}
	int32_t Object::getInt32() const {
		return data.int32;
	}
	int64_t Object::getInt64() const {
		return data.int64;
	}
	float Object::getFloat32() const {
		return data.float32;
	}
	double Object::getFloat64() const {
		return data.float64;
	}
	Array Object::getArray() const {
		return Array(data.array);
	}
	char* Object::getString() const {
		return getArray().getString();
	}
#define GENERIC_UI_DATA_OBJECT_TO_NUMERIC(to_type) \
		switch(type) {\
			case Types::int8:\
			return (tmptype)data.int8;\
			case Types::int16:\
			return (tmptype)data.int16;\
			case Types::int32:\
			return (tmptype)data.int32;\
			case Types::int64:\
			return (tmptype)data.int64;\
			case Types::float32:\
			return (tmptype)data.float32;\
			case Types::float64:\
			return (tmptype)data.float64;\
			default:\
			throw TypeException(type, to_type);\
		}
	int8_t Object::toInt8() const {
		typedef int8_t tmptype;
		GENERIC_UI_DATA_OBJECT_TO_NUMERIC(Types::int8);
	}
	int16_t Object::toInt16() const {
		typedef int16_t tmptype;
		GENERIC_UI_DATA_OBJECT_TO_NUMERIC(Types::int16);
	}
	int32_t Object::toInt32() const {
		typedef int32_t tmptype;
		GENERIC_UI_DATA_OBJECT_TO_NUMERIC(Types::int32);
	}
	int64_t Object::toInt64() const {
		typedef int64_t tmptype;
		GENERIC_UI_DATA_OBJECT_TO_NUMERIC(Types::int64);
	}
	float Object::toFloat32() const {
		typedef float tmptype;
		GENERIC_UI_DATA_OBJECT_TO_NUMERIC(Types::float32);
	}
	double Object::toFloat64() const {
		typedef double tmptype;
		GENERIC_UI_DATA_OBJECT_TO_NUMERIC(Types::float64);
	}
	Array Object::toArray() const {
		if (type == Types::array)
			return Array(data.array);
		else throw TypeException(type, Types::array);
	}
	char* Object::toString() const {
		if (type == Types::array)
			return getArray().getString();
		else throw TypeException(type, Types::array);
	}
	ArrayBase* Object::getArrayBase() const {
		return data.array;
	}

	Object::Object() :
			type(Types::undefined) {

	}
	Object::Object(const Object& other) :
			data(other.data), type(other.type) {
		if (type == Types::array && data.array != NULL) data.array->retain();
	}
	Object::Object(Types type) :
			type(type) {
	}
	Object::~Object() {
		__object_do_release_if_needed(this);
	}
	Object::Object(int8_t o) :
			type(Types::int8) {
		data.int8 = o;
	}
	Object::Object(int16_t o) :
			type(Types::int16) {
		data.int16 = o;
	}
	Object::Object(int32_t o) :
			type(Types::int32) {
		data.int32 = o;
	}
	Object::Object(int64_t o) :
			type(Types::int64) {
		data.int64 = o;
	}
	Object::Object(float o) :
			type(Types::float32) {
		data.float32 = o;
	}
	Object::Object(double o) :
			type(Types::float64) {
		data.float64 = o;
	}
	Object::Object(const Array& o) :
			type(Types::array) {
		data.array = o.arr;
		if (data.array != NULL) data.array->retain();
	}
	Object::Object(const char* str) :
			type(Types::array) {
		data.array = new ArrayType<int8_t>((int8_t*) str, (int32_t) (strlen(str) + 1));
	}
	Object Object::int8(int8_t o) {
		Object tmp(Types::int8);
		tmp.data.int8 = o;
		return tmp;
	}
	Object Object::int16(int16_t o) {
		Object tmp(Types::int16);
		tmp.data.int16 = o;
		return tmp;
	}
	Object Object::int32(int32_t o) {
		Object tmp(Types::int32);
		tmp.data.int32 = o;
		return tmp;
	}
	Object Object::int64(int64_t o) {
		Object tmp(Types::int64);
		tmp.data.int64 = o;
		return tmp;
	}
	Object Object::float32(float o) {
		Object tmp(Types::float32);
		tmp.data.float32 = o;
		return tmp;
	}
	Object Object::float64(double o) {
		Object tmp(Types::float64);
		tmp.data.float64 = o;
		return tmp;
	}
	Object Object::array(const Array& o) {
		Object tmp(Types::array);
		tmp.data.array = o.arr;
		if (tmp.data.array != NULL) tmp.data.array->retain();
		return tmp;
	}
	Object Object::undefined() {
		Object tmp(Types::undefined);
		return tmp;
	}

	Object& Object::operator=(const Object& obj) {
		__object_do_release_if_needed(this);
		type = obj.type;
		data = obj.data;
		if (type == Types::array && data.array != NULL) data.array->retain();
		return *this;
	}
	int8_t Object::operator=(int8_t obj) {
		__object_do_release_if_needed(this);
		type = Types::int8;
		return data.int8 = obj;
	}
	int16_t Object::operator=(int16_t obj) {
		__object_do_release_if_needed(this);
		type = Types::int16;
		return data.int16 = obj;
	}
	int32_t Object::operator=(int32_t obj) {
		__object_do_release_if_needed(this);
		type = Types::int32;
		return data.int32 = obj;
	}
	int64_t Object::operator=(int64_t obj) {
		__object_do_release_if_needed(this);
		type = Types::int64;
		return data.int64 = obj;
	}
	float Object::operator=(float obj) {
		__object_do_release_if_needed(this);
		type = Types::float32;
		return data.float32 = obj;
	}
	double Object::operator=(double obj) {
		__object_do_release_if_needed(this);
		type = Types::float64;
		return data.float64 = obj;
	}
	const Array& Object::operator=(const Array& obj) {
		__object_do_release_if_needed(this);
		type = Types::array;
		data.array = obj.arr;
		if (data.array != NULL) data.array->retain();
		return obj;
	}

/*ArrayType<int8_t>::ArrayType(const char* s) {
 int32_t l=strlen(s);
 arr=vector<int8_t>(s,s+l);
 }
 ArrayType<int8_t>::ArrayType(const string& s) {
 int32_t l=s.length();
 const char* ch=s.data();
 arr=vector<int8_t>(ch,ch+l);
 }*/
}
/* namespace GenericStruct */
