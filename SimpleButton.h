#pragma once
#include <SDL.h>
#include "InteractiveElement.h"
#include "RenderableElement.h"
class SimpleButton : public Interactive, public Renderable
{
private:
	float anchorX = 0, anchorY = 0;
	bool lastState = false;
	bool state = false;
	SDL_Rect area;

	bool CanLoseFocus();
	void Update(double);
	void Render(SDL_Renderer*);

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

	void setArea(float w, float h);
	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);

	bool InArea(int x, int y) const;

	SimpleButton(InteractiveRegistry& iRegistry = InteractiveRegistry::globalRegistry, RenderableRegistry& rRegistry = RenderableRegistry::globalRegistry);
};

