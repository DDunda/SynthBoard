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

	void TryCall(callback c) {
		if (c != NULL) c(this);
	}
public:
	callback OnUpdate = NULL;
	callback OnRender = NULL;

	bool active = true;
	bool visible = true;

	RenderableElement();
	~RenderableElement();

	virtual void update() {
		TryCall(OnUpdate);
	};
	virtual void render(SDL_Renderer*) {
		TryCall(OnRender);
	};

	static void UpdateAllElements();
	static void RenderAllElements(SDL_Renderer*);
};

#endif