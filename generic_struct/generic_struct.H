/*
 * generic_struct.H
 *
 *  Created on: 2012-11-02
 *      Author: xaxaxa
 */

#ifndef GENERIC_STRUCT_H_
#define GENERIC_STRUCT_H_
#include <map>
#include <exception>
#include <string>
#include <functional>
#include <stdint.h>
#include <rgc.H>
#include <dlfcn.h>
#include <iostream>
#include <vector>
#include <boost/type_traits.hpp>
#include <string.h>
#include <arpa/inet.h>
namespace GenericStruct
{
	using namespace std;
	uint64_t htonll(uint64_t value);
	uint64_t ntohll(uint64_t value);
	struct Object;
	struct ArrayBase;
	
	enum class Types
		: int8_t
		{
			//						2	3		4		5		6		7		8		9
		undefined = 0, int8 = 1, int16, int32, int64, float32, float64, dict, array, object
	};
	extern const char* TypeNames[10];
	const char* getTypeName(Types type);
	class TypeException: public std::exception
	{
	public:
		string msg;
		TypeException() throw ();
		TypeException(string msg) throw ();
		TypeException(Types t_from, Types t_to) throw (); //conversion error
		const char* what() const throw ();
		virtual ~TypeException() throw ();
	};
	struct Array
	{
		ArrayBase* arr;
		template<class T> Array(std::initializer_list<T> list);
		template<class T> Array(const T* arr, int len);
		Array(const Array& other);
		Array(ArrayBase* a);
		Array(const char* str);
		Array(Types type);
		Array();
		Types type() const;

		Object operator[](int i) const;
		Array& operator=(const Array& other);
		void length(int newlen);
		int length() const;
		void append(const Object& o);
		template<class T> T* getArray() const;
		char* getString() const;
		~Array();
		void serialize(const function<void(int8_t*, int)>& cb) const;
		void deserialize(const function<void(int8_t*, int)>& cb);
	};
	struct Object
	{
		union
		{
			int8_t int8;
			int16_t int16;
			int32_t int32;
			int64_t int64;
			float float32;
			double float64;
			//Dict dict;
			ArrayBase* array;
		} data;
		Types type;
		int8_t getInt8() const;
		int16_t getInt16() const;
		int32_t getInt32() const;
		int64_t getInt64() const;
		float getFloat32() const;
		double getFloat64() const;
		Array getArray() const;
		char* getString() const;
		ArrayBase* getArrayBase() const;
		int8_t toInt8() const;
		int16_t toInt16() const;
		int32_t toInt32() const;
		int64_t toInt64() const;
		float toFloat32() const;
		double toFloat64() const;
		Array toArray() const;
		char* toString() const;
		Object();
		Object(const Object& other);
		Object(Types type);
		Object(int8_t o);
		Object(int16_t o);
		Object(int32_t o);
		Object(int64_t o);
		Object(float o);
		Object(double o);
		Object(const Array& o);
		template<class T> Object(std::initializer_list<T> list);
		Object(const char* str);
		~Object();
		static Object int8(int8_t o);
		static Object int16(int16_t o);
		static Object int32(int32_t o);
		static Object int64(int64_t o);
		static Object float32(float o);
		static Object float64(double o);
		static Object array(const Array& o);
		static Object undefined();
		Object& operator=(const Object& obj);
		int8_t operator=(int8_t obj);
		int16_t operator=(int16_t obj);
		int32_t operator=(int32_t obj);
		int64_t operator=(int64_t obj);
		float operator=(float obj);
		double operator=(double obj);
		const Array& operator=(const Array& obj);
		inline void serialize(const function<void(int8_t*, int)>& cb) const;
		inline void deserialize(const function<void(int8_t*, int)>& cb);
	};
	namespace Aliases
	{
		typedef GenericStruct::Object O;
		typedef GenericStruct::Array A;
	}
	struct ArrayBase: public RGC::Object
	{
		Types type;
		ArrayBase(Types type) :
				type(type) {
		}
		ArrayBase() {
		}
		virtual GenericStruct::Object operator[](int i) const=0;
		virtual void length(int newlen)=0;
		virtual int length() const=0;
		virtual void append(const GenericStruct::Object& o)=0;
		virtual void serialize(const function<void(int8_t*, int)>& cb) const=0;
		virtual void deserialize_array(const function<void(int8_t*, int)>& cb)=0;
		static inline void deserialize(ArrayBase*& arr, const function<void(int8_t*, int)>& cb);
	};
	template<class T> struct ArrayType: public ArrayBase
	{
		vector<T> arr;
		GenericStruct::Object operator[](int i) const;
		void length(int newlen);
		int length() const;
		void append(const GenericStruct::Object& o);
		ArrayType();
		ArrayType(std::initializer_list<T> list);
		ArrayType(const T* arr, int len);
		ArrayType(const char* s);
		ArrayType(const string& s);
		~ArrayType();ArrayType& operator=(const ArrayType& other)=delete;
		void serialize(const function<void(int8_t*, int32_t)>& cb) const;
		void deserialize_array(const function<void(int8_t*, int32_t)>& cb);
	};

} /* namespace GenericStruct */

namespace GenericStruct
{
	template<class T> Object::Object(std::initializer_list<T> list) :
			type(Types::array) {
		data.array = new ArrayType<T>(list);
	}

	template<class T> Array::Array(std::initializer_list<T> list) {
		arr = new ArrayType<T>(list);
	}
	template<class T> Array::Array(const T* arr, int32_t len) {
		this->arr = new ArrayType<T>(arr, len);
	}
	template<class T> T* Array::getArray() const {
		return &(((ArrayType<T>*) arr)->arr[0]);
	}

	template<class T> Object ArrayType<T>::operator[](int32_t i) const {
		return arr[i];
	}
	template<class T> void ArrayType<T>::length(int32_t newlen) {
		arr.resize(newlen);
	}
	template<class T> int32_t ArrayType<T>::length() const {
		return (int32_t) arr.size();
	}
	inline void __do_append(ArrayType<int8_t>* th, const Object& o) {
		th->arr.push_back(o.toInt8());
	}
	inline void __do_append(ArrayType<int16_t>* th, const Object& o) {
		th->arr.push_back(o.toInt16());
	}
	inline void __do_append(ArrayType<int32_t>* th, const Object& o) {
		th->arr.push_back(o.toInt32());
	}
	inline void __do_append(ArrayType<int64_t>* th, const Object& o) {
		th->arr.push_back(o.toInt64());
	}
	inline void __do_append(ArrayType<float>* th, const Object& o) {
		th->arr.push_back(o.toFloat32());
	}
	inline void __do_append(ArrayType<double>* th, const Object& o) {
		th->arr.push_back(o.toFloat64());
	}
	inline void __do_append(ArrayType<Array>* th, const Object& o) {
		th->arr.push_back(o.toArray());
	}
	inline void __do_append(ArrayType<Object>* th, const Object& o) {
		th->arr.push_back(o);
	}
	template<class T> void ArrayType<T>::append(const GenericStruct::Object& o) {
		__do_append(this, o);
	}

	inline void __do_init_array(ArrayType<int8_t>* th) {
		th->type = Types::int8;
	}
	inline void __do_init_array(ArrayType<int16_t>* th) {
		th->type = Types::int16;
	}
	inline void __do_init_array(ArrayType<int32_t>* th) {
		th->type = Types::int32;
	}
	inline void __do_init_array(ArrayType<int64_t>* th) {
		th->type = Types::int64;
	}
	inline void __do_init_array(ArrayType<float>* th) {
		th->type = Types::float32;
	}
	inline void __do_init_array(ArrayType<double>* th) {
		th->type = Types::float64;
	}
	inline void __do_init_array(ArrayType<Array>* th) {
		th->type = Types::array;
	}
	inline void __do_init_array(ArrayType<Object>* th) {
		th->type = Types::object;
	}
	/*template<class T> void __do_init_array(ArrayType<T>* th, const boost::false_type& sdfgh) {
	 if (boost::is_integral<T>::value) {
	 if (sizeof(T) == 8)
	 th->type = Types::int8;
	 else if (sizeof(T) == 16)
	 th->type = Types::int16;
	 else if (sizeof(T) == 32)
	 th->type = Types::int32;
	 else if (sizeof(T) == 64) th->type = Types::int64;
	 } else if (boost::is_floating_point<T>::value) {
	 if (sizeof(T) == 32)
	 th->type = Types::float32;
	 else if (sizeof(T) == 64) th->type = Types::float64;
	 }
	 }
	 template<class T> void __do_init_array(ArrayType<T>* th, const boost::true_type& sdfgh) {
	 if (boost::is_base_of<ArrayBase*, T>::value) {

	 th->type = Types::array;
	 for (int32_t i = 0; i < (int32_t) th->arr.size(); i++)
	 th->arr[i]->retain();
	 } else if (boost::is_base_of<GenericStruct::Object, T>::value) {
	 cout << "asdfzxcv" << endl;
	 th->type = Types::object;
	 }
	 }*/
	template<class T>
	ArrayType<T>::ArrayType() {
		__do_init_array(this);
	}
	template<class T>
	ArrayType<T>::ArrayType(std::initializer_list<T> list) :
			arr(list) {
		__do_init_array(this);
	}
	template<class T>
	ArrayType<T>::ArrayType(const T* arr, int32_t len) :
			arr(arr, arr + len) {
		__do_init_array(this);
	}
	template<class T> void __do_destruct_array(ArrayType<T>* th, const boost::true_type& sdfgh) {
		if (th->type == Types::array) {
			for (int32_t i = 0; i < (int32_t) th->arr.size(); i++)
				th->arr[i]->release();
		}
	}
	template<class T> void __do_destruct_array(ArrayType<T>* th, const boost::false_type& sdfgh) {

	}
	template<class T> ArrayType<T>::~ArrayType() {
		__do_destruct_array(this, boost::is_pointer<T>());

	}
	inline void __do_serialize(int8_t o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 1);
	}
	inline void __do_serialize(int16_t o, const function<void(int8_t*, int32_t)>& cb) {
		uint16_t tmp = htons((uint16_t) o);
		cb((int8_t*) &tmp, 2);
	}
	inline void __do_serialize(int32_t o, const function<void(int8_t*, int32_t)>& cb) {
		uint32_t tmp = htonl((uint32_t) o);
		cb((int8_t*) &tmp, 4);
	}
	inline void __do_serialize(int64_t o, const function<void(int8_t*, int32_t)>& cb) {
		uint64_t tmp = htonll((uint64_t) o);
		cb((int8_t*) &tmp, 8);
	}
	inline void __do_serialize(float o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 4);
	}
	inline void __do_serialize(double o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 8);
	}
	inline void __do_serialize(const Array& o, const function<void(int8_t*, int32_t)>& cb) {
		o.serialize(cb);
	}
	inline void __do_serialize(const Object& o, const function<void(int8_t*, int32_t)>& cb) {
		o.serialize(cb);
	}

	inline void __do_deserialize(int8_t& o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 1);
	}
	inline void __do_deserialize(int16_t& o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 2);
		o = (int16_t) ntohs((uint16_t) o);
	}
	inline void __do_deserialize(int32_t& o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 4);
		o = (int32_t) ntohl((uint32_t) o);
	}
	inline void __do_deserialize(int64_t& o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 8);
		o = (int64_t) ntohll((uint64_t) o);
	}
	inline void __do_deserialize(float& o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 4);
	}
	inline void __do_deserialize(double& o, const function<void(int8_t*, int32_t)>& cb) {
		cb((int8_t*) &o, 8);
	}
	inline void __do_deserialize(Array& o, const function<void(int8_t*, int32_t)>& cb) {
		o.deserialize(cb);
	}
	inline void __do_deserialize(Object& o, const function<void(int8_t*, int32_t)>& cb) {
		o.deserialize(cb);
	}

	template<class T> void ArrayType<T>::serialize(
			const function<void(int8_t*, int32_t)>& cb) const {
		int8_t tmp[5];
		tmp[0] = (int8_t) type;
		*(uint32_t*) (tmp + 1) = htonl((uint32_t) arr.size());
		cb(tmp, 5);
		for (int32_t i = 0; i < (int32_t) arr.size(); i++) {
			__do_serialize((T) arr[i], cb);
		}
	}
	template<class T> void ArrayType<T>::deserialize_array(
			const function<void(int8_t*, int32_t)>& cb) {
		uint32_t l;
		cb((int8_t*) &l, 4);
		l = ntohl(l);
		arr.resize(l);
		//*(uint32_t*) (tmp + 1) = htonl((uint32_t) arr.size());

		for (int32_t i = 0; i < (int32_t) arr.size(); i++) {
			__do_deserialize((T&) arr[i], cb);
		}
	}
	void __object_do_release_if_needed(Object* o);
	void Object::serialize(const function<void(int8_t*, int32_t)>& cb) const {
		cb((int8_t*) &type, 1);
		switch (type) {
			case Types::int8:
			{
				__do_serialize(data.int8, cb);
				break;
			}
			case Types::int16:
			{
				__do_serialize(data.int16, cb);
				break;
			}
			case Types::int32:
			{
				__do_serialize(data.int32, cb);
				break;
			}
			case Types::int64:
			{
				__do_serialize(data.int64, cb);
				break;
			}
			case Types::float32:
			{
				__do_serialize(data.float32, cb);
				break;
			}
			case Types::float64:
			{
				__do_serialize(data.float64, cb);
				break;
			}
			case Types::array:
			{
				data.array->serialize(cb);
				break;
			}
			default:
				break;
		}
	}
	void Object::deserialize(const function<void(int8_t*, int32_t)>& cb) {
		__object_do_release_if_needed(this);
		type = Types::undefined;
		Types _type;
		cb((int8_t*) &_type, 1);

		switch (_type) {
			case Types::int8:
			{
				__do_deserialize(data.int8, cb);
				break;
			}
			case Types::int16:
			{
				__do_deserialize(data.int16, cb);
				break;
			}
			case Types::int32:
			{
				__do_deserialize(data.int32, cb);
				break;
			}
			case Types::int64:
			{
				__do_deserialize(data.int64, cb);
				break;
			}
			case Types::float32:
			{
				__do_deserialize(data.float32, cb);
				break;
			}
			case Types::float64:
			{
				__do_deserialize(data.float64, cb);
				break;
			}
			case Types::array:
			{
				ArrayBase::deserialize(data.array, cb);
				break;
			}
			default:
				break;
		}
		type = _type;
	}

	void ArrayBase::deserialize(ArrayBase*& arr, const function<void(int8_t*, int32_t)>& cb) {
		Types type;
		cb((int8_t*) &type, 1);
		switch (type) {
			case Types::int8:
			{
				arr = new ArrayType<int8_t>();
				break;
			}
			case Types::int16:
			{
				arr = new ArrayType<int16_t>();
				break;
			}
			case Types::int32:
			{
				arr = new ArrayType<int32_t>();
				break;
			}
			case Types::int64:
			{
				arr = new ArrayType<int64_t>();
				break;
			}
			case Types::float32:
			{
				arr = new ArrayType<float>();
				break;
			}
			case Types::float64:
			{
				arr = new ArrayType<double>();
				break;
			}
			case Types::array:
			{
				arr = new ArrayType<Array>();
				break;
			}
			case Types::object:
			{
				arr = new ArrayType<GenericStruct::Object>();
				break;
			}
			default:
			{
				arr = NULL;
				return;
			}
		}
		arr->deserialize_array(cb);
	}
}

#endif /* GENERIC_STRUCT_H_ */
