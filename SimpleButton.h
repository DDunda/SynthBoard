#pragma once
#include <SDL.h>
#include "InteractiveElement.h"
class SimpleButton : public InteractiveElement
{
private:
	float anchorX = 0, anchorY = 0;
	bool lastState = false;
	bool state = false;
public:
	void update();
	void render(SDL_Renderer*);

	void unfocus();
	void focus();

	bool released();
	bool pressed();
	bool up();
	bool down();

	void setArea(float w, float h);
	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);
};

