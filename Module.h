#pragma once

#include <SDL.h>
#include <vector>
#include <algorithm>
#include <iostream>

class Module {
private:
	static std::vector<Module*> registry;
protected:
	virtual void CalculateState() = 0;
	virtual void PresentState() = 0;
public:
	static SDL_mutex* registryLock;
	Module() {
		registry.push_back(this);
	}
	~Module() {
		Module* toFind = this;
		//Module** pos = 
		std::vector<Module*>::iterator it = std::find(registry.begin(), registry.end(), this);
		if (it != registry.end()) {
			Module* lastPtr = registry[registry.size() - 1];
			*it = lastPtr;
			registry.pop_back();
		}
	}
	static void CalculateAllModuleStates() {
		for (Module* m : registry) m->CalculateState();
	}
	static void PresentAllModuleStates() {
		for (Module* m : registry) m->PresentState();
	}
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


#define MakeModule(name) \
class name : public Module {\
	class name##Output : public Output<double> {\
		friend name;\
		name##Output(double d) : Output<double>(d) {}\
	};\
	void CalculateState();\
	void PresentState();

typedef Output<double>* Input;