
#ifndef DELEGATE_H
#define DELEGATE_H
#include <functional>
template<class SIGNATURE> struct DelegateBase;
template<class R, class... P>
struct DelegateBase<R(P...)>
{
	R (*func)(void* data, P...);
	void* data;
	template<class... P2>
	R operator() (P2 && ...p) const {
		return func(data,std::forward<P2>(p)...);
	}
	DelegateBase() {}
	DelegateBase(R (*func)(void* data, P... p...), void* data): func(func),data(data) {}
	template<class X>DelegateBase(R (*func)(X*,P...), X* th)
		:func((R(*)(void*,P...))func),data(th) {}
	template<class X>DelegateBase(R (X::*func)(P...), X* th)
		:func((R(*)(void*,P...))func),data(th) {}
	template<class X>DelegateBase(X* th)
		:func((R(*)(void*,P...))&X::operator()),data(th) {}
	DelegateBase(std::nullptr_t n): func(NULL){};
	
	R (*operator=(R (*func)(void* data, P...)))(void* data, P...) {
		return this->func=func;
	}
	bool operator==(void* other) {
		return (void*)func == other;
	}
	bool operator==(const DelegateBase<R(P...)>& other) {
		return func == other.func && data==other.data;
	}
	bool operator!=(void* other) {
		return (void*)func != other;
	}
	bool operator!=(const DelegateBase<R(P...)>& other) {
		return func != other.func && data==other.data;
	}
};

template<class SIGNATURE> struct Delegate;
template<class R, class... P>
struct Delegate<R(P...)>:public DelegateBase<R(P...)>
{
	Delegate() {
		this->func=NULL;
	}
	Delegate(R (*func)(void* data, P... p...), void* data): DelegateBase<R(P...)>(func,data) {}
	template<class X>Delegate(R (*func)(X*,P...), X* th)
		:DelegateBase<R(P...)>(func,th) {}
	template<class X>Delegate(R (X::*func)(P...), X* th)
		:DelegateBase<R(P...)>(func,th) {}
	template<class X>Delegate(X* th)
		:DelegateBase<R(P...)>(th) {}
	Delegate(std::nullptr_t n): DelegateBase<R(P...)>(n){};
	R (*operator=(R (*func)(void* data, P...)))(void* data, P...) {
		return this->func=func;
	}
};

#endif
