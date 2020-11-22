#pragma once
#include <SDL.h>
#include "InteractiveElement.h"
class SimpleButton : public InteractiveElement
{
private:
	float anchorX = 0, anchorY = 0;
	bool lastState = false;
	bool state = false;
	SDL_Rect area;

public:
	// Button used to press this button
	int mouseButton;

	// Returns if the button was pressed
	bool isPressed();
	// Returns if the button was released
	bool isReleased();
	// Returns if the button is down
	bool isDown();
	// Returns if the button is up
	bool isUp();

	// Called when the designated button is pressed down (up -> down)
	virtual void onPress() {};
	// Called when the designated button is released (down -> up)
	virtual void onRelease() {};
	// Called after the button is updated
	virtual void onUpdate() {};
	// Called after the button is rendered
	virtual void onRender(SDL_Renderer*) {};

	void setArea(float w, float h);
	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);

	bool inArea(int x, int y);

	void update();
	void render(SDL_Renderer*);
};

