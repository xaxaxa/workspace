/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */

//RGC: simple reference counting based garbage collection framework
#ifndef __RGC_H
#define __RGC_H
#include <cstddef>
#include <utility>
#include <stdlib.h>

namespace RGC
{
	class Allocator;
	class Object
	{
	public:
		Allocator* allocator;
		int refCount; //reference counter
		Object() {
			allocator = NULL;
			refCount = 1;
		}
		Object(const Object& other) {
			refCount = 1;
		}
		virtual ~Object() {
		}
		inline void retain(int n = 1) {
			__sync_fetch_and_add(&refCount, n);
		}
		inline void destruct();
		inline bool release(int n = 1) { //returns whether or not destruction occurred
			if(__sync_fetch_and_sub(&refCount, n) == n) {
				destruct();
				return true;
			}
			return false;
		}
	};
	class Allocator: public virtual Object
	{
	public:
		virtual void* alloc(int s)=0;
		virtual void dealloc(void* obj)=0;
		void _setAllocator(void* v, Allocator* a) {
		} //not inherited from Object; do nothing
		void _setAllocator(Object* o, Allocator* a) {
			o->allocator = a;
		}
		template<class T, class ... A>
		T* New(A&&... a) {
			T* tmp = new (alloc(sizeof(T))) T(std::forward<A>(a)...);
			_setAllocator(tmp, this);
			return tmp;
		}
		template<class T>
		void del(T* obj) {
			obj->~T();
			dealloc(obj);
		}
	};
	class DefaultAllocator: public Allocator
	{
		void* alloc(int s) override final {
			return malloc(s);
		}
		void dealloc(void* obj) override final {
			return free(obj);
		}
	};
	static DefaultAllocator defaultAllocator;
	void Object::destruct() {
		if (allocator == NULL)
			delete this;
		else allocator->del(this);
	}
	//template<class T> struct Ref;
	template<class T> struct Ref
	{
		T* obj;
		inline Ref() :
				obj(NULL) {
		}
		//inline Ref(const Ref<T>& other);
		inline Ref(T* obj) :
				obj(obj) {
			if (obj != NULL) obj->retain();
		}
		inline Ref(T& obj) :
				obj(&obj) {
			if (&obj != NULL) obj.retain();
		}
		/*template<class ... ARGS> inline Ref(ARGS&& ... args)
		 :obj(new T(std::forward<ARGS>(args)...))
		 {
		 }*/
		inline ~Ref() {
			if (this->obj != NULL) this->obj->release();
		}
		Ref(const Ref<T>& other) :
				obj(other.obj) {
			if (obj != NULL) obj->retain();
		}
		T* operator=(T* obj) {
			if (this->obj != NULL) this->obj->release();
			this->obj = obj;
			if (obj != NULL) obj->retain();
			return obj;
		}
		T* operator=(T& obj) {
			return operator=(*obj);
		}
		Ref& operator=(const Ref& other) {
			if (this->obj != NULL) this->obj->release();
			this->obj = other.obj;
			if (obj != NULL) obj->retain();
			return *this;
		}
		inline T* operator()() const {
			return obj;
		}
		inline T& operator*() const {
			return *obj;
		}
		inline T* operator->() const {
			return obj;
		}
		inline T* get() const {
			return obj;
		}
		template<class A> Ref<A> staticCast() {
			return Ref<A>(static_cast<A*>(obj));
		}
		template<class A> Ref<A> dynamicCast() {
			return Ref<A>(dynamic_cast<A*>(obj));
		}
		template<class A> Ref<A> reinterpretCast() {
			return Ref<A>(reinterpret_cast<A*>(obj));
		}
	};
	//creates a new instance, but disowns it
	template<class T, class ... ARGS> inline T* newObj(ARGS&&... args) {
		T* tmp = new T(std::forward<ARGS>(args)...);
		tmp->refCount--;
		return tmp;
	}
	
	template<class T> struct FakePtr
	{
		T obj;
		template<class ... ARGS>
		FakePtr(ARGS&&... args): obj(std::forward<ARGS>(args)...) {}
		FakePtr(const FakePtr<T>& other)=delete;
		inline T& operator*() {
			return obj;
		}
		inline T* operator->() {
			return &obj;
		}
		inline T* get() const {
			return &obj;
		}
		FakePtr operator=(const FakePtr& other)=delete;
	};
}
#endif
