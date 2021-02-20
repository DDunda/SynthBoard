#pragma once

#ifndef RENDERABLE_ELEMENTS
#define RENDERABLE_ELEMENTS
#include <SDL.h>
#include <vector>
#include <algorithm>

class RenderableElement;

typedef void(*callback)(RenderableElement*);

struct padding {
	int top = 0, right = 0, bottom = 0, left = 0;
};

class RenderableElement {
protected:
	static std::vector<RenderableElement*> elements;

public:
	bool active = true;
	bool visible = true;

	RenderableElement();
	~RenderableElement();

	// Called after the button is updated
	virtual void onUpdate() {};
	// Called after the button is rendered
	virtual void onRender(SDL_Renderer*) {};

	virtual void update(double) = 0;
	virtual void render(SDL_Renderer*) = 0;

	static void UpdateAllElements(double);
	static void RenderAllElements(SDL_Renderer*);
};

#endif