#include "Module.h"

ModuleRegistry ModuleRegistry::globalRegistry;

void ModuleRegistry::CalculateAllStates() {
	for(Module* module : entries) module->CalculateState();
}
void ModuleRegistry::PresentAllStates() {
	for (Module* module : entries) module->PresentState();
}

Module::Module(ModuleRegistry& registry) : parent(registry) {
	parent.Register(this);
}
Module::~Module() {
	parent.Unregister(this);
}