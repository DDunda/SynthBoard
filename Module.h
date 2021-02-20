#pragma once

#include <SDL.h>
#include <vector>
#include <algorithm>
#include <iostream>

class Module;

class ModuleRegistry {
private:
	std::vector<Module*> modules;
public:
	SDL_mutex* lock;

	ModuleRegistry();
	~ModuleRegistry();
	void Register(Module* module);
	void Unregister(Module* module);
	void MovePosition(Module* module, size_t position);
	void CalculateAllModuleStates();
	void PresentAllModuleStates();

	void RegisterUnsafe(Module* module);
	void UnregisterUnsafe(Module* module);
	void MovePositionUnsafe(Module* module, size_t position);
	void CalculateAllModuleStatesUnsafe();
	void PresentAllModuleStatesUnsafe();
};

class Module {
	friend class ModuleRegistry;
protected:
	virtual void CalculateState() = 0;
	virtual void PresentState() = 0;
	ModuleRegistry& parent;
public:
	static SDL_mutex* registryLock;
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

typedef Output<double>* Input;

#define MakeModuleOutput(name) class name##Output : public Output<double> {\
	friend class name;\
	name##Output(double d) : Output<double>(d) {}\
};
#define MakeModule(name) class name : public Module {\
MakeModuleOutput(name)\
protected:\
	void CalculateState();\
	void PresentState();