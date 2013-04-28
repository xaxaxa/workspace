/*
 * headerContainer.H
 *
 *  Created on: Apr 28, 2013
 *      Author: xaxaxa
 */

#ifndef HEADERCONTAINER_H_
#define HEADERCONTAINER_H_
#include <ctype.h>
#include <sys/types.h>
namespace cppsp
{
	typedef CP::String String;
	static int ci_compare(String s1, String s2) {
		if (s1.length() > s2.length()) return 1;
		if (s1.length() < s2.length()) return -1;
		if (s1.length() == 0) return 0;
		char a, b;
		for (int i = 0; i < s1.length(); i++) {
			a = tolower(s1.data()[i]);
			b = tolower(s2.data()[i]);
			if (a < b) return -1;
			if (a > b) return 1;
		}
		return 0;
	}
	struct Header
	{
		String name;
		String value;
	};
	struct headerContainer
	{
		//pointer-to-pointer so that the real buffer can be relocated
		//(for example during MemoryStream reallocations due to resize)
		char** buffer;
		CP::StringPool* sp;
		struct item
		{
			int nameStart;
			int nameLength;
			int valueStart;
			int valueLength;
		};
		static const int bucketSize = 8;
		struct bucket
		{
			bucket* next;
			item items[bucketSize];
			int length;
		};

		struct iterator
		{
			headerContainer* cont;
			bucket* b;
			int i;
			void operator+=(int i) {
				this->i += i;
				while (this->i > bucketSize && b != NULL) {
					b = b->next;
					this->i -= bucketSize;
				}
				if (b != NULL && this->i >= b->length) b = NULL;
			}
			void operator++(int) {
				operator+=(1);
			}
			bool operator==(const iterator& other) {
				if (b == NULL && other.b == NULL) return true;
				return b == other.b && i == other.i;
			}
			bool operator!=(const iterator& other) {
				return !operator==(other);
			}
			Header operator*() {
				item& it = b->items[i];
				return { {(*cont->buffer)+it.nameStart,it.nameLength},
					{	(*cont->buffer)+it.valueStart,it.valueLength}};
			}
			item& get() {
				return b->items[i];
			}
		};
		bucket* _first = NULL;
		bucket* _last = NULL;
		headerContainer(char** buffer, CP::StringPool* sp) :
				buffer(buffer), sp(sp) {
		}
		void add(item it) {
			if (_last == NULL || _last->length >= bucketSize) addBucket();
			_last->items[_last->length] = it;
			_last->length++;
		}
		void addBucket() {
			bucket* b = (bucket*) sp->add(sizeof(bucket));
			b->next = NULL;
			b->length = 0;
			if (_last != NULL) _last->next = b;
			_last = b;
			if (_first == NULL) _first = b;
		}
		String operator[](String name) {
			char* tmp = (char*) *buffer;
			for (bucket* b = _first; b != NULL; b = b->next) {
				for (int i = 0; i < b->length; i++)
					if (ci_compare(name, { tmp + b->items[i].nameStart, b->items[i].nameLength }) == 0) return {tmp+b->items[i].valueStart,b->items[i].valueLength};
			}
			return {(char*)nullptr,0};
		}
		iterator find(String name) {
			char* tmp = (char*) *buffer;
			for (bucket* b = _first; b != NULL; b = b->next) {
				for (int i = 0; i < b->length; i++)
					if (ci_compare(name, { tmp + b->items[i].nameStart, b->items[i].nameLength }) == 0) return {this,b,i};
			}
			return end();
		}
		iterator begin() {
			return {this,_first,0};
		}
		iterator end() {
			return {this,NULL,0};
		}
		void clear() {
			_first = _last = NULL;
		}
	};
	//different version that uses absolute pointers instead of relative positions
	struct headerContainer2
	{
		CP::StringPool* sp;
		struct item
		{
			const char* name;
			const char* value;
			int nameLength;
			int valueLength;
		};
		static const int bucketSize = 8;
		struct bucket
		{
			bucket* next;
			item items[bucketSize];
			int length;
		};
		struct iterator
		{
			bucket* b;
			int i;
			void operator+=(int i) {
				this->i += i;
				while (this->i > bucketSize && b != NULL) {
					b = b->next;
					this->i -= bucketSize;
				}
				if (b != NULL && this->i >= b->length) b = NULL;
			}
			void operator++(int) {
				operator+=(1);
			}
			bool operator==(const iterator& other) {
				if (b == NULL && other.b == NULL) return true;
				return b == other.b && i == other.i;
			}
			bool operator!=(const iterator& other) {
				return !operator==(other);
			}
			Header operator*() {
				item& it = b->items[i];
				return { {it.name,it.nameLength}, {it.value,it.valueLength}};
			}
			item& get() {
				return b->items[i];
			}
		};
		bucket* _first = NULL;
		bucket* _last = NULL;
		headerContainer2(CP::StringPool* sp) :
				sp(sp) {
		}
		void add(item it) {
			if (_last == NULL || _last->length >= bucketSize) addBucket();
			_last->items[_last->length] = it;
			_last->length++;
		}
		void add(String name, String value) {
			add( { name.data(), value.data(), name.length(), value.length() });
		}
		void addCopy(String name, String value) {
			name = sp->addString(name);
			value = sp->addString(value);
			add( { name.data(), value.data(), name.length(), value.length() });
		}
		void addBucket() {
			bucket* b = (bucket*) sp->add(sizeof(bucket));
			b->next = NULL;
			b->length = 0;
			if (_last != NULL) _last->next = b;
			_last = b;
			if (_first == NULL) _first = b;
		}
		String operator[](String name) {
			for (bucket* b = _first; b != NULL; b = b->next) {
				for (int i = 0; i < b->length; i++)
					if (ci_compare(name, { b->items[i].name, b->items[i].nameLength }) == 0) return {b->items[i].value,b->items[i].valueLength};
			}
			return {(char*)nullptr,0};
		}
		iterator find(String name) {
			for (bucket* b = _first; b != NULL; b = b->next) {
				for (int i = 0; i < b->length; i++)
					if (ci_compare(name, { b->items[i].name, b->items[i].nameLength }) == 0) return {b,i};
			}
			return end();
		}
		iterator begin() {
			return {_first,0};
		}
		iterator end() {
			return {NULL,0};
		}
		void clear() {
			_first = _last = NULL;
		}
	};
}

#endif /* HEADERCONTAINER_H_ */
