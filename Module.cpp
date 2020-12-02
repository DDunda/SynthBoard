#include "Module.h"

std::vector<Module*> Module::registry = {};
SDL_mutex* Module::registryLock = SDL_CreateMutex();