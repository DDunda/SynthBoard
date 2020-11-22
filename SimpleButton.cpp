#include "SimpleButton.h"
#include "Input.h"

// Gets state of button

bool SimpleButton::isPressed() {
	return state && !lastState;
}
bool SimpleButton::isReleased() {
	return !state && lastState;
}
bool SimpleButton::isDown() {
	return state;
}
bool SimpleButton::isUp() {
	return !state;
}

// Setting graphics 

void SimpleButton::setArea(float w, float h) {
	float posX = area.x + area.w * anchorX;
	float posY = area.y + area.h * anchorY;

	area.w = w;
	area.h = h;

	setPosition(posX, posY);
}
void SimpleButton::setAnchor(float aX, float aY) {
	float posX = area.x + area.w * anchorX;
	float posY = area.y + area.h * anchorY;

	anchorX = aX;
	anchorY = aY;

	setPosition(posX, posY);
}
void SimpleButton::setPosition(int x, int y) {
	area.x = x - area.w * anchorX;
	area.y = y - area.h * anchorY;
}

// Dynamically running stuff

void SimpleButton::update() {
	lastState = state;
	if (!state && buttonDown(mouseButton) && inFocus()) {
		onPress();
		state = true;
	}
	if (state && buttonUp(mouseButton)) {
		onRelease();
		state = false;
	}

	onUpdate();
}
void SimpleButton::render(SDL_Renderer* r) {
	if (state) SDL_SetRenderDrawColor(r, 193, 193, 193, 255);
	else SDL_SetRenderDrawColor(r, 226, 226, 226, 255);
	SDL_RenderFillRect(r, &area);

	onRender(r);
}

// Checks if mouse is on button

bool SimpleButton::inArea(int x, int y) {
	return inBounds(area, x, y);
}