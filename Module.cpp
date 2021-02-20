#include "Module.h"

ModuleRegistry::ModuleRegistry() {
	modules = {};
	lock = SDL_CreateMutex();
}
ModuleRegistry::~ModuleRegistry() {
	SDL_LockMutex(lock);
	modules.empty();
	SDL_UnlockMutex(lock);
}
void ModuleRegistry::Register(Module* module) {
	SDL_LockMutex(lock);
	modules.push_back(module);
	SDL_UnlockMutex(lock);
}
void ModuleRegistry::Unregister(Module* module) {
	SDL_LockMutex(lock);
	std::vector<Module*>::iterator it = std::find(modules.begin(), modules.end(), module);
	if (it != modules.end()) {
		*it = modules.back();
		modules.pop_back();
	}
	SDL_UnlockMutex(lock);
}
void ModuleRegistry::MovePosition(Module* module, size_t position) {
	SDL_LockMutex(lock);
	std::vector<Module*>::iterator it = std::find(modules.begin(), modules.end(), module);
	if (it != modules.end()) {
		if (position < 0) {
			if (modules.size() - position < 0) {
				*it = modules.front();
				modules.front() = module;
			}
			else {
				*it = modules[modules.size() - position];
				modules[modules.size() - position] = module;
			}
		}
		else {
			if (position < modules.size()) {
				*it = modules[position];
				modules[position] = module;
			}
			else {
				*it = modules.back();
				modules.back() = module;
			}
		}
	}
	SDL_UnlockMutex(lock);
}
void ModuleRegistry::CalculateAllModuleStates() {
	SDL_LockMutex(lock);
	for (Module* module : modules) module->CalculateState();
	SDL_UnlockMutex(lock);
}
void ModuleRegistry::PresentAllModuleStates() {
	SDL_LockMutex(lock);
	for (Module* module : modules) module->PresentState();
	SDL_UnlockMutex(lock);
}
void ModuleRegistry::RegisterUnsafe(Module* module) {
	modules.push_back(module);
}
void ModuleRegistry::UnregisterUnsafe(Module* module) {
	std::vector<Module*>::iterator it = std::find(modules.begin(), modules.end(), module);
	if (it != modules.end()) {
		*it = modules.back();
		modules.pop_back();
	}
}
void ModuleRegistry::MovePositionUnsafe(Module* module, size_t position) {
	std::vector<Module*>::iterator it = std::find(modules.begin(), modules.end(), module);
	if (it != modules.end()) {
		if (position < 0) {
			if (modules.size() - position < 0) {
				*it = modules.front();
				modules.front() = module;
			}
			else {
				*it = modules[modules.size() - position];
				modules[modules.size() - position] = module;
			}
		}
		else {
			if (position < modules.size()) {
				*it = modules[position];
				modules[position] = module;
			}
			else {
				*it = modules.back();
				modules.back() = module;
			}
		}
	}
}
void ModuleRegistry::CalculateAllModuleStatesUnsafe() {
	for (Module* module : modules) module->CalculateState();
}
void ModuleRegistry::PresentAllModuleStatesUnsafe() {
	for (Module* module : modules) module->PresentState();
}

Module::Module(ModuleRegistry& registry) : parent(registry) {
	parent.Register(this);
}
Module::~Module() {
	parent.Unregister(this);
}