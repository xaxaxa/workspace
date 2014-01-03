/*
 * util.H
 *
 *  Created on: Oct 29, 2013
 *      Author: xaxaxa
 */

#ifndef UTIL_H_
#define UTIL_H_
#ifndef NULL
#define NULL ((void*)0)
#endif
#include <stdlib.h>
#include <functional>
#include <exception>
#include <pthread.h>
#include <errno.h>
#include <stdexcept>
#include <string.h>
#include <map>
#include <stddef.h>
#include <stdio.h>
#include "types.h"

#ifndef likely
#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)
#endif
template<class T, class ... ARGS> inline T* New(ARGS&&... args) {
	T* tmp = (T*) malloc(sizeof(T));
	new (tmp) T(std::forward<ARGS>(args)...);
	return tmp;
}
//create an object and release it (without freeing it)
template<class T, class ... ARGS> inline T* RNew(ARGS&&... args) {
	T* tmp = (T*) malloc(sizeof(T));
	new (tmp) T(std::forward<ARGS>(args)...);
	tmp->refCount--;
	return tmp;
}
template<class T> inline void Delete(T* obj) {
	obj->~T();
	free(obj);
}
#define WARN(s,...) fprintf(stderr,s "\n",##__VA_ARGS__)
struct RefCount
{
	int refCount;
	RefCount() :
			refCount(1) {
	}
	virtual ~RefCount() {
	}
	inline int retain(int n = 1) {
		return __sync_add_and_fetch(&refCount, n);
	}
	inline void destruct() {
		Delete(this);
	}
	inline int release(int n = 1) { //returns final reference count
		int i = release_noDestruct(n);
		if (i == 0) destruct();
		return i;
	}
	inline int release_noDestruct(int n = 1) {
		return __sync_sub_and_fetch(&refCount, n);
	}
};
template<class T> struct Ref
{
	T* obj;
	inline Ref() :
			obj(NULL) {
	}
	inline Ref(T* obj) :
			obj(obj) {
		if (obj != NULL) obj->retain();
	}
	inline Ref(T& obj) :
			obj(&obj) {
		if (&obj != NULL) obj.retain();
	}
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

	template<class X> void init(R (*func)(X*, P...), X* th) {
		this->func = (void*) (R (*)(void*, P...))func;this->data=th;
	}
	template<class X>void init(R (X::*func)(P...), X* th) {
		this->func=(void*)(R(*)(void*,P...))func;
		this->data=th;
	}
	void init(R (*func)(void*, P...)) {
		this->func=(void*)func;
		this->data=NULL;
	}
	void init(const GenericDelegate& other) {
		this->func=other.func;
		this->data=other.data;
	}
	template<class X>void init(X* th) {
		this->func=(void*)(R(*)(void*,P...))&X::operator();
		this->data=th;
	}
	void init(std::nullptr_t n) {
		this->func=NULL;
	}
	void init() {
		this->func=NULL;
	}
	void assign(const GenericDelegate& other) {
		this->func=other.func;
		this->data=other.data;
	}
	void assign(R (*func)(void* data, P...)) {
		this->func=(void*)func;
	}
	void deinit() {
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
	template<class T>
	DelegateBase<R(P...)>& operator=(T other) {
		assign(other);
		return *this;
	}
};

template<class SIGNATURE> struct Delegate;
template<class R, class ... P>
struct Delegate<R(P...)> : public DelegateBase<R(P...)>
{
	Delegate() {
		this->init();
	}
	~Delegate() {
		this->deinit();
	}
	//Delegate(R (*func)(void* data, P... p...), void* data): DelegateBase<R(P...)>(func,data) {}
	template<class X> Delegate(R (*func)(X*, P...), X* th) {
		this->init(func, th);
	}
	template<class X> Delegate(R (X::*func)(P...), X* th) {
		this->init(func, th);
	}
	Delegate(R (*func)(void*, P...)) {
		this->init(func);
	}
	Delegate(const GenericDelegate& other) {
		this->init(other);
	}
	template<class X> Delegate(X* th) {
		this->init(th);
	}
	Delegate(std::nullptr_t n) {
		this->init(nullptr);
	}
};

template<class T>
struct Future
{
	Delegate<void(T, int)>* cbAddr;
	Future(Delegate<void(T, int)>* cbAddr) :
			cbAddr(cbAddr) {
	}
};
struct ErrorValue
{
	int e;
	ErrorValue(int e) :
			e(e) {
	}
};
template<class T> struct WithError
{
	T value;
	int error;
	T& operator()() {
		return value;
	}
	T& operator*() {
		return value;
	}
	T* operator->() {
		return &value;
	}
	operator bool() {
		return error == 0;
	}
	WithError(ErrorValue e) :
			error(e.e) {
	}
	WithError(const T& value) :
			value(value), error(0) {
	}
};

//this version of AsyncValue is VERY DIFFERENT FROM THE ONE IN CPOLL;
//it uses error codes instead of exceptions, and has a different api

//an AsyncValue represents a value that is either available right away (synchronous),
//or will be available in the future; generally use case is an async function that might
//sometimes be able to return the data right away (for example, an async read() function
//might search a cache first, and return immediately if found); this avoids stack blowup
//that could happen if the function were to call the callback within itself

//when you get an AsyncValue from a function invocation, you should check if it is
//immediately available or not; if it is not, you must call wait(), and pass in your
//callback; failure to call wait() on an async AsyncValue will result in a segfault
template<class T> class AsyncValue
{
public:
	//when the callback is called, err should NEVER be -1
	Delegate<void(T val, int err)>* cbAddr;
	T value;
	int error; //0=completed synchronously; -1=completing asynchronously;
	//other values: user defined error codes

	T& operator()() {
		return value;
	}
	T& operator*() {
		return value;
	}
	T* operator->() {
		return &value;
	}
	//returns whether or not a value or error is immediately available (synchronous)
	operator bool() {
		return error != -1;
	}
	AsyncValue<T>& operator=(const T& other) {
		value = other;
		error = 0;
		return *this;
	}
	AsyncValue<T>& operator=(const AsyncValue<T>& other) {
		if (-1 != (error = other.error)) value = other.value;
		else cbAddr = other.cbAddr;
		return *this;
	}
	//construct a immediately-available AsyncValue
	AsyncValue(const T& other) :
			value(other), error(0) {
	}
	AsyncValue(const WithError<T>& other) :
			value(other.value), error(other.error) {
	}
	AsyncValue(const AsyncValue<T>& other) :
			cbAddr(other.cbAddr), value(other.value), error(other.error) {
	}
	//construct an asynchronously available AsyncValue
	AsyncValue(Future<T> f) :
			cbAddr(f.cbAddr), error(-1) {
	}
	AsyncValue(ErrorValue e) :
			error(e.e) {
	}
	struct _placeholder1
	{
	};
	template<class ... A> AsyncValue(_placeholder1, A&&... a) :
			value(std::forward<A>(a)...), error(0) {

	}
	template<class ... A>
	static AsyncValue<T> create(A&&... a) {
		return AsyncValue<T>(_placeholder1(), std::forward<A>(a)...);
	}
	AsyncValue() :
			error(0) {
	}
	//wait for the asynchronous value to be available
	void wait(Delegate<void(T, int)> cb) {
		*cbAddr = cb;
	}
};
static inline void throwUNIXException() {
	throw std::runtime_error(strerror(errno));
}
struct Lock
{
	pthread_mutex_t m;
	Lock() {
		if (pthread_mutex_init(&m, NULL) < 0) throwUNIXException();
	}
	~Lock() {
		pthread_mutex_destroy(&m);
	}
	void lock() {
		if (pthread_mutex_lock(&m) < 0) throwUNIXException();
	}
	void unlock() {
		pthread_mutex_unlock(&m);
	}
};
struct ScopeLock
{
	Lock& l;
	ScopeLock(Lock& l) :
			l(l) {
		l.lock();
	}
	~ScopeLock() {
		l.unlock();
	}
};
class IDSet1
{
public:
	struct item
	{
		u32 nextFree;
		void* obj;
	};
	item* items;
	Lock lock;
	u32 length;
	u32 firstFree;
	IDSet1(int initialSize = 8) {
		length = initialSize;
		items = (item*) malloc(length * sizeof(item));
		if (items == nullptr) throwUNIXException();
		firstFree = 0;
		for (u32 i = 0; i < length; i++) {
			items[i].obj = NULL;
			items[i].nextFree = i + 1;
		}
		items[length - 1].nextFree = (u32) -1;
	}
	void* find(u32 id) {
		if (id >= length) return nullptr;
		return items[id].obj;
	}
	void extend() {
		u32 l = length * 2;
		ScopeLock lck(lock);
		items = (item*) realloc((void*) items, size_t(sizeof(item) * l));
		for (u32 i = length; i < l; i++) {
			items[i].obj = NULL;
			items[i].nextFree = i + 1;
		}
		items[l - 1].nextFree = (u32) -1;
		firstFree = length;
		length = l;
	}
	u32 add(void* obj) {
		u32 newid;
		{
			ScopeLock lck(lock);
			if (firstFree == (u32) -1) extend();
			newid = firstFree;
			firstFree = items[firstFree].nextFree;
		}
		items[newid].obj = obj;
		return newid;
	}
};
class IDSet
{
public:
	std::map<u32, void*> items;
	Lock lock;
	u32 nextID;
	IDSet() {
		nextID = 1;
	}
	void* find(u32 id) {
		ScopeLock lck(lock);
		auto it = items.find(id);
		if (it == items.end()) return nullptr;
		return (*it).second;
	}
	u32 add(void* obj) {
		ScopeLock lck(lock);
		u32 newid = nextID++;
		items[newid] = obj;
		return newid;
	}
	bool add(u32 newid, void* obj) {
		ScopeLock lck(lock);
		auto it = items.find(newid);
		if (it == items.end()) {
			items[newid] = obj;
			if (newid >= nextID) nextID = newid + 1;
			return true;
		}
		return false;
	}
	void remove(u32 id) {
		ScopeLock lck(lock);
		items.erase(id);
	}
};
template<class T>
struct ListItem
{
	ListItem* prev;
	ListItem* next;
	T data;
	template<class ... ARGS> ListItem(ARGS&&... args) :
			data(std::forward<ARGS>(args)...) {
	}
	static ListItem* getItem(T* item) {
		return (ListItem*) (((u8*) item) - offsetof(ListItem, data));
	}
};
struct ListItemHeader
{
	ListItemHeader* prev;
	ListItemHeader* next;
};
template<class Item>
class List
{
public:
	Item* first;
	Item* last;
	List() {
		first = last = nullptr;
	}
	void append(Item* it) {
		it->prev = last;
		it->next = nullptr;
		if (last) last->next = it;
		last = it;
		if (first == NULL) first = it;
	}
	void insertBefore(Item* it, Item* before) {
		if (before->prev) before->prev->next = it;
		it->next = before;
		it->prev = before->prev;
		before->prev = it;
		if (before == first) first = it;
		if (last == NULL) first = it;
	}
	void remove(Item* it) {
		if (it == first) first = (Item*) it->next;
		if (it == last) last = (Item*) it->prev;
		if (it->prev) it->prev->next = it->next;
		if (it->next) it->next->prev = it->prev;
	}
};
#endif /* UTIL_H_ */
