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
#ifndef DELEGATE_H
#define DELEGATE_H
#include <functional>
#include "rgc.H"
#ifndef NULL
#define NULL ((void*)0)
#endif

struct GenericDelegate
{
	void* func;
	void* data;
	template<class R, class ... P> R operator()(P && ...p) {
		return ((R (*)(void* data, P...)) func)(data, std::forward<P>(p)...);
	}
	
	void* operator=(void* func) {
		return this->func = func;
	}
	operator bool() {
		return func != NULL;
	}
};

template<class SIGNATURE> struct DelegateBase;
template<class R, class ... P>
struct DelegateBase<R(P...)> : public GenericDelegate
{
	/*R (*func)(void* data, P...);
	 void* data;*/
	template<class ... P2>
	R operator()(P2 && ...p) const {
		return ((R (*)(void* data, P...)) func)(data, std::forward<P2>(p)...);
	}
	
	R (*operator=(R (*func)(void* data, P...)))(void* data, P...) {
		this->func = (void*) func;
		return func;
	}
	bool operator==(void* other) {
		return (void*) func == other;
	}
	bool operator==(const DelegateBase<R(P...)>& other) {
		return func == other.func && data == other.data;
	}
	bool operator!=(void* other) {
		return (void*) func != other;
	}
	bool operator!=(const DelegateBase<R(P...)>& other) {
		return func != other.func && data == other.data;
	}

	bool ref;
	void init() {
		this->ref = false;
		this->func = NULL;
	}
	void deinit() {
		if (ref) ((RGC::Object*) this->data)->release();
	}
	//non-refcounting
	void init(R (*func)(void*, P...)) {
		this->func=(void*)func;
		this->data = NULL;
		this->ref = false;
	}
	template<class X> void init(R (*func)(X*, P...), X* th) {
		this->func = (void*) (R (*)(void*, P...))func;
		this->data = th;
		this->ref = false;
	}
	template<class X>void init(R (X::*func)(P...), X* th) {
		this->func = (void*)(R(*)(void*,P...))func;
		this->data = th;
		this->ref = false;
	}
	//refcounting
	template<class X> void init(R (*func)(X*, P...), RGC::Object* th) {
		this->func = (void*) (R (*)(void*, P...))func;
		this->data = th;
		this->ref = true;
		th->retain();
	}
	template<class X>void init(R (X::*func)(P...), RGC::Object* th) {
		this->func = (void*)(R(*)(void*,P...))func;
		this->data = th;
		this->ref = true;
		th->retain();
	}
	inline bool _isRGCObject(void* v) {
		return false;
	}
	inline bool _isRGCObject(RGC::Object* v) {
		return true;
	}
	template<class X>void init(X* th) {
		this->func=(void*)(R(*)(void*,P...))&X::operator();
		this->data=th;
		if((this->ref = _isRGCObject(th))) ((RGC::Object*)th)->retain();
	}

	void init(const DelegateBase<R(P...)>& other) {
		this->func=other.func;
		this->data=other.data;
		if((this->ref=other.ref)) ((RGC::Object*)this->data)->retain();
	}
	void init(std::nullptr_t n) {
		this->func=NULL;
		this->ref = false;
	};
	void assign(const DelegateBase<R(P...)>& other) {
		this->func=other.func;
		if(this->ref) ((RGC::Object*)this->data)->release();
		this->data=other.data;
		if((this->ref=other.ref)) ((RGC::Object*)this->data)->retain();
	}
	DelegateBase<R(P...)>& operator=(const DelegateBase<R(P...)>& other) {
		init(other);
		return *this;
	}
};

template<class SIGNATURE> struct Delegate;
template<class R, class ... P>
struct Delegate<R(P...)> : public DelegateBase<R(P...)>
{
	bool ref;
	Delegate() {
		this->init();
	}
	~Delegate() {
		this->deinit();
	}

	Delegate(R (*func)(void*, P...)) {
		this->init(func);
	}
	template<class X> Delegate(R (*func)(X*, P...), X* th) {
		this->init(func,th);
	}
	template<class X>Delegate(R (X::*func)(P...), X* th) {
		this->init(func,th);
	}
	template<class X>Delegate(X* th) {
		this->init(th);
	}

	Delegate(const DelegateBase<R(P...)>& other) {
		this->init(other);
	}
	Delegate(const Delegate<R(P...)>& other) {
		this->init(other);
	}
	Delegate<R(P...)>& operator=(const DelegateBase<R(P...)>& other) {
		this->assign(other);
		return *this;
	}
	Delegate(std::nullptr_t n) {
		this->init(nullptr);
	};
};
template<class SIGNATURE>
using DelegateRef=Delegate<SIGNATURE>;

template<class R, class ... P> static Delegate<R(P...)> nullDelegate() {
	struct
	{
		R operator()(P ... p...) {
			return R();
		}
	}tmp;
	return &tmp;
}

template<class SIGNATURE> class DelegateChain;
template<class R, class ... P>
class DelegateChain<R(P...)>
{
public:
	struct item
	{
		Delegate<R(P...)> func;
		item* prev;
		item* next;
		template<class ... P2>
		inline R operator()(P2 && ...p) const {
			return func(std::forward<P2>(p)...);
		}
	};
	item* last = NULL;
	DelegateChain() {

	}
	~DelegateChain() {
		item* it = last;
		while (it != nullptr) {
			item* tmp = it->prev;
			delete it;
			it = tmp;
		}
	}
	item* attach(Delegate<R(P...)> func) {
		item* it = new item { func, last, NULL };
		if (last != NULL) last->next = it;
		last = it;
		return it;
	}
	void detach(item* it) {
		if (it->next != NULL) it->next->prev = it->prev;
		if (it->prev != NULL) it->prev->next = it->next;
		if (last == it) last = it->prev;
		delete it;
	}
	inline R operator()(P ... p) const {
		return (*last)(std::forward<P>(p)...);
	}
};
#endif
