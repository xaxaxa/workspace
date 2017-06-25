/*
 * compile UIs like this:
 * g++ somefile.C -shared -o somefile.so
 * 
 * for UIs, do the attaching in a function with this attribute:
 * 	__attribute__((constructor))
 * 
 * to load a UI, either call GenericUI::load(), or use LD_PRELOAD
 * 
 * */

#ifndef GENERIC_UI_H
#define GENERIC_UI_H
#include <map>
#include <exception>
#include <string>
#include <functional>
#include <stdint.h>
#include <cpoll/rgc.H>
#include <dlfcn.h>
#include <iostream>
#include <vector>
#include <boost/type_traits.hpp>
#include <string.h>
#include <arpa/inet.h>
#include <generic_struct.H>
#include <pthread.h>
#include <stdexcept>
using namespace std;
namespace GenericUI
{

	class Exception: public std::exception
	{
	public:
		string msg;
		Exception() throw ();
		Exception(string msg) throw ();
		const char* what() const throw ();
		virtual ~Exception() throw ();
	};
	typedef GenericStruct::Object Object;
	typedef GenericStruct::Array Array;
	namespace Aliases
	{
		typedef GenericStruct::Object O;
		typedef GenericStruct::Array A;
	}
	/*struct Item
	 {
	 public:
	 enum class Types
	 : uint8_t
	 {
	 undefined = 0, slotIn = 1, //from the point of view of the app
	 slotOut = 2, property = 3 //a combination of a slotIn (setter and getter)
	 //and a slotOut (property change notification)
	 };
	 virtual string name()=0;
	 virtual Types type()=0;
	 };*/
	//template<class... A>
	enum class EventTypes
		:int8_t {
			none = 0,
			//global
		newInterface,
		//interface
		newSlot,
		//slot
		newAttachment, invocation, invocationSync, ret,
		//module loading
		loadUI
	};
	struct Interface;
	struct Slot;
	struct EventData
	{
		string name;
		Interface* interface;
		Slot* slot;
		Object o; //argument(s) or return value(s)
		uint64_t scope; //user-modifiable field
		function<Object(Object)> cb;
	};

	struct Slot
	{
	public:
		Interface* interface;
		string name;
		function<bool(EventTypes, EventData&)> listener;
		//bool isOut;
		//function<void(const function<void*()>&, A...)> callback;
		function<Object(Object)> callback;
		//void invoke(const function<void*()>& cb, A&&... a...)
		Object invokeSync(Object a);
		Object invokeLocal(Object a);
		void invoke(Object a);
		void attach(const function<Object(Object)>& callback);
		void listen(const function<bool(EventTypes, EventData&)>& func);
	};
	/*class SlotItem: public Item
	 {
	 Slot<void*> slot;
	 };
	 class PropertyItem: public Item
	 {
	 Slot<bool, void*> in;
	 Slot<void*> out;
	 };*/

	struct Interface
	{
		function<bool(EventTypes, EventData&)> listener;
		map<string, Slot> items;
		string name;
		pthread_mutex_t mutex;
		Slot& attach(string name);
		void listen(const function<bool(EventTypes, EventData&)>& func);
		Interface();
		~Interface();
	};
	struct Instance
	{
		map<string, Interface> interfaces;
		pthread_mutex_t mutex;
		Interface& attach(string name);
		function<bool(EventTypes, EventData&)> listener;

		//currently only one listener is supported
		void listen(const function<bool(EventTypes, EventData&)>& func);
		void load(string fileName);
		Instance();
		~Instance();
	};
#ifdef IS_UI
	__attribute__ ((visibility("hidden"))) Instance* instance;
	extern "C" void generic_ui_init(void* v, Instance* inst) {
		//cout << inst << endl;
		//cout << "&instance=" << &instance << endl;
		instance=inst;
		void (*func)();
		func=(void(*)())dlsym(v, "init");
		if(func) func();
		func = (void(*)())dlsym(v, "_Z4initv");
		if(func)func();
	}
#else
	extern Instance* instance;
#endif
	/*Interface& attach(string name);
	 void listen(const function<bool(EventTypes, EventData&)>& func);
	 void load(string fileName);*/

	inline Interface& attach(string name) {
		return instance->attach(name);
	}
	inline void listen(const function<bool(EventTypes, EventData&)>& func) {
		instance->listen(func);
	}
	inline void load(string fileName) {
		instance->load(fileName);
	}
}

#endif
#ifdef COMPILE_GENERIC_UI
namespace GenericUI
{
	struct scopeLock
	{
		pthread_mutex_t& mutex;
		scopeLock(pthread_mutex_t& m):mutex(m)
		{	pthread_mutex_lock(&mutex);}
		~scopeLock()
		{	pthread_mutex_unlock(&mutex);}
	};
	static void initMutex(pthread_mutex_t& mutex) {
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		int e;
		if((e=pthread_mutex_init(&mutex,&attr))!=0) {
			const char* err=strerror(e);
			throw runtime_error("mutex initialization failed: "+string(err));
		}
		pthread_mutexattr_destroy(&attr);
	}
	Instance *instance=new Instance();
	Exception::Exception() throw () :
	msg("Exception occured") {

	}
	Exception::Exception(string msg) throw () :
	msg(msg) {

	}
	const char* Exception::what() const throw () {
		return msg.c_str();
	}
	Exception::~Exception() throw () {

	}

	map<string, Interface> _interfaces;
	map<string, Interface>& interfaces(_interfaces);
	Object Slot::invokeSync(Object a)
	{
		EventData ed;
		if(listener!=nullptr) {
			ed= {name, interface, this, a, 0};
			if(!listener(EventTypes::invocationSync, ed)) return ed.o;
		}
		if(callback!=nullptr) ed.o=callback(a);
		else ed.o=Object::undefined();
		if(listener!=nullptr) {
			listener(EventTypes::ret, ed);
		}
		return ed.o;
	}
	Object Slot::invokeLocal(Object a)
	{
		EventData ed;
		if(callback!=nullptr) ed.o=callback(a);
		else ed.o=Object::undefined();
		return ed.o;
	}
	void Slot::invoke(Object a)
	{
		EventData ed;
		if(listener!=nullptr) {
			ed= {name, interface, this, a, 0};
			if(!listener(EventTypes::invocation, ed)) return;
		}
		if(callback!=nullptr)
		ed.o=callback(a);
		if(listener!=nullptr) {
			if(!listener(EventTypes::ret, ed)) return;
		}
	}
	void Slot::attach(const function<Object(Object)>& callback)
	{
		if(listener!=nullptr) {
			EventData ed {name, interface, this, Object::undefined(), 0};
			if(!listener(EventTypes::newAttachment, ed)) return;
		}
		this->callback=callback;

	}
	void Slot::listen(const function<bool(EventTypes, EventData&)>& func)
	{
		listener=func;
	}
	Slot& Interface::attach(string name)
	{
		Slot* s;
		bool b;
		{
			scopeLock l(mutex);
			auto it=items.find(name);
			if((b=(it==items.end()))) {
				auto& tmp=items[name];
				tmp.interface=this;
				tmp.name=name;
				s=&tmp;
			}
			else return (*it).second;
		}
		if(listener!=nullptr) {
			EventData ed {name, this, s, Object::undefined(), 0};
			listener(EventTypes::newSlot, ed);
		}
		return *s;
	}
	void Interface::listen(const function<bool(EventTypes, EventData&)>& func) {
		listener=func;
	}
	Interface& Instance::attach(string name)
	{
		//cout << "attach: " << this << endl;
		Interface* iface;
		{
			scopeLock l(mutex);
			auto it=interfaces.find(name);
			if(it==interfaces.end()) {
				auto& tmp=interfaces[name];
				tmp.name=name;
				iface=&tmp;
			}
			else return (*it).second;
		}
		if(listener!=nullptr) {
			EventData ed {name, iface, NULL, Object::undefined(), 0};
			listener(EventTypes::newInterface, ed);
		}
		return *iface;
	}
	void Instance::load(string fileName)
	{
		if(listener!=nullptr) {
			EventData ed {fileName, NULL, NULL, Object::undefined(), 0};
			if(!listener(EventTypes::loadUI, ed)) return;
		}
		//cout << "fileName="<<fileName << endl;
		void* v=
		dlopen(fileName.c_str(),RTLD_LAZY|RTLD_LOCAL);
		if(v==NULL) return;
		void (*func)(void*,Instance*) = (void(*)(void*, Instance*))dlsym(v, "generic_ui_init");
		//cout << func << endl;
		if(func==NULL) return;
		func(v, this);
	}

	void Instance::listen(const function<bool(EventTypes, EventData&)>& func) {
		listener=func;
	}

	Interface::Interface() {
		initMutex(mutex);
	}
	Instance::Instance() {
		initMutex(mutex);
	}
	Interface::~Interface() {
		pthread_mutex_destroy(&mutex);
	}
	Instance::~Instance() {
		pthread_mutex_destroy(&mutex);
	}

}

#endif
