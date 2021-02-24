#pragma once

#ifndef RENDERABLE_ELEMENTS
#define RENDERABLE_ELEMENTS
#include <SDL.h>
#include "Batcher.h"

class Renderable;

struct padding {
	int top = 0, right = 0, bottom = 0, left = 0;
};

class RenderableRegistry : public BatchRegistry<Renderable> {
public:
	void UpdateAllElements(double);
	void RenderAllElements(SDL_Renderer*);

	static RenderableRegistry globalRegistry;
};

class Renderable {
	friend class RenderableRegistry;
protected:
	virtual void Update(double) {};
	virtual void Render(SDL_Renderer*) {};

	RenderableRegistry& parent;

public:
	bool active = true;
	bool visible = true;

	Renderable(RenderableRegistry& registry);
	~Renderable();
};
#endif