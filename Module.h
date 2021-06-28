#pragma once

#include "Batcher.h"
#include <vector>
#include <thread>
#include <queue>

class Module;

class ModuleRegistry : public BatchRegistry<Module> {
public:
	void CalculateAllStates();
	void PresentAllStates();

	static ModuleRegistry globalRegistry;
};

class Module {
	friend class ModuleRegistry;
protected:
	virtual void CalculateState() = 0;
	virtual void PresentState() = 0;

	ModuleRegistry& parent;

public:
	Module(ModuleRegistry& registry);
	~Module();
};

template <class T>
class Output {
private:
	T frontValue;
protected:
	T backValue;
	void PresentValue() {
		frontValue = backValue;
	}
public:
	Output(T t) : backValue(t), frontValue(t) {}
	operator T() const { return frontValue; } // This is probably a bad idea
};

template <class T>
using Input = Output<T>*;

#define MakeModuleOutput(name) template<typename T> class name##Output : public Output<T> {\
	friend class name;\
	name##Output(T d) : Output<T>(d) {}\
};
#define MakeModule(name) class name : public Module {\
MakeModuleOutput(name)\
protected:\
	void CalculateState();\
	void PresentState();
