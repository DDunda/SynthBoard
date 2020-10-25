#pragma once

#include <vector>

struct ValueContainer {
	size_t ownerships;
	ValueContainer() {
		ownerships = 1;
	}
	void addPtr() {
		ownerships++;
	}
	void removePtr() {
		ownerships--;
	}
};

template<class T>
class EasyPointer {
public:
	ValueContainer* internalPtr = NULL;
	T* ptr = NULL;

	EasyPointer() {}
	EasyPointer(T* val) : internalPtr(new ValueContainer()), ptr(val) {	}
	EasyPointer(const EasyPointer<T>& val) : internalPtr(val.internalPtr), ptr(val.ptr) {
		internalPtr->addPtr();
	}
	template <class T2>
	EasyPointer(const EasyPointer<T2>& val) : internalPtr(val.internalPtr), ptr((T*)val.ptr) {
		internalPtr->addPtr();
	}
	~EasyPointer() {
		RemovePointer();
	}

	T& operator* ()
	{
		return *ptr;
	}
	T* operator-> ()
	{
		return ptr;
	}
	operator T* () {
		return ptr;
	}

	void operator= (const EasyPointer<T>& p) {
		RemovePointer();

		internalPtr = p.internalPtr;
		ptr = p.ptr;

		if (internalPtr != NULL) internalPtr->addPtr();
		else ptr = NULL;
	}
	template <class T2>
	void operator= (const EasyPointer<T2>& p) {
		RemovePointer();

		internalPtr = p.internalPtr;
		ptr = (T*)p.ptr;

		if (internalPtr != NULL) internalPtr->addPtr();
		else ptr = NULL;
	}

	bool isSet() {
		return internalPtr != NULL;
	}
	void RemovePointer() {
		if (internalPtr == NULL) return;

		internalPtr->removePtr();

		if (internalPtr->ownerships == 0) {
			delete ptr;
			delete internalPtr;
		}
		internalPtr = NULL;
		ptr = NULL;
	}
};

template <class T>
class GetVal {
public:
	virtual T Get() = 0;
};

template <class T>
class SetVal {
public:
	virtual void Set(T input) = 0;
};

class Updater {
private:
	static std::vector<Updater*> sources;
public:
	Updater() {
		sources.push_back(this);
	}
	~Updater() {
		auto it = std::find(sources.begin(), sources.end(), this);
		sources.erase(it, it + 1);
	}
	virtual void reset() {}
	virtual void frameUpdate() {}

	static void updateAllSources() {
		for (auto s : sources) s->frameUpdate();
	}
};

template <class T>
class Source : public GetVal<T>, public Updater {};

template <class T>
class Val : public Source<float>, public SetVal<T> {
private:
	T val;
public:
	Val(T v) {
		val = v;
	}
	T Get() {
		return val;
	}
	void Set(T input) {
		val = input;
	};
	void reset() {}
};

template <class T>
class pVal : public Source<float>, public SetVal<T> {
private:
	T* val;
public:
	pVal(T* v) {
		val = v;
	}
	T Get() {
		return *val;
	}
	T& Get() {
		return *val;
	}
	void Set(T input) {
		*val = input;
	}
	void reset() {}
};

typedef Val<float> fVal;
typedef EasyPointer<Source<float>> epSource;

/*#pragma once

template <class T>
class GetVal {
public:
	virtual T Get() {
		return T();
	};
};

template <class T>
class SetVal {
public:
	virtual void Set(T input) = 0;
};

template <class T>
class Source : public GetVal<T> {
public:
	virtual void reset() = 0;
};

template <class T>
class Val : public Source<T>, public SetVal<T> {
private:
	T val;
public:
	Val(T v) {
		val = v;
	}
	T Get() {
		return val;
	}
	void Set(T input) {
		val = input;
	};
	void reset() {}
};

template <class T>
class pVal : public Source<T>, public SetVal<T> {
private:
	T* val;
public:
	pVal(T* v) {
		val = v;
	}
	T Get() {
		return *val;
	}
	T& Get() {
		return *val;
	}
	void Set(T input) {
		*val = input;
	}
	void reset() {}
};

template<class T>
struct ValueContainer {
	T* val;
	size_t ownerships;
	ValueContainer(T* val) {
		this->val = val;
		ownerships = 1;
	}
	~ValueContainer() {
		delete val;
	}
	void addPtr() {
		ownerships++;
	}
	void removePtr() {
		ownerships--;
		if (ownerships == 0) delete this;
	}
};

template<class T>
class EasyPointer {
public:
	ValueContainer<T>* internalPtr = NULL;
	EasyPointer() {}
	EasyPointer(T* val) {
		internalPtr = new ValueContainer<T>(val);
		internalPtr->addPtr();
	}
	template <class T2>
	EasyPointer(EasyPointer<T2>& val) {
		internalPtr = (ValueContainer<T>*)val.internalPtr;
		internalPtr->addPtr();
	}
	~EasyPointer() {
		if (internalPtr == NULL) return;

		internalPtr->removePtr();
		internalPtr = NULL;
	}
	void NewPointer(T* val) {
		if (internalPtr != NULL) internalPtr->removePtr();
		internalPtr = new ValueContainer<T>(val);
	}
	void NewPointer() {
		NewPointer(new T);
	}
	T& operator* ()
	{
		return *internalPtr->val;
	}
	T* operator-> ()
	{
		return internalPtr->val;
	}
	void operator= (const EasyPointer<T>& D) {
		if (internalPtr != NULL) internalPtr->removePtr();

		internalPtr = D.internalPtr;

		if (internalPtr != NULL) internalPtr->addPtr();
	}
	bool isSet() {
		return !(internalPtr == NULL);
	}
};*/