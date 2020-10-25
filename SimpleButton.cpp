#include "SimpleButton.h"
#include "Input.h"

void SimpleButton::focus() {
	TryCall(OnFocus);
	infocus = true;
}

void SimpleButton::unfocus() {
	TryCall(OnUnfocus);
	infocus = false;
}

bool SimpleButton::released() {
	return !state && lastState && infocus;
}
bool SimpleButton::pressed() {
	return state && !lastState && infocus;
}
bool SimpleButton::up() {
	return !state && infocus;
}
bool SimpleButton::down() {
	return state && infocus;
}

void SimpleButton::setArea(float w, float h) {
	float posX = clickArea.x + clickArea.w * anchorX;
	float posY = clickArea.y + clickArea.h * anchorY;

	clickArea.w = w;
	clickArea.h = h;

	setPosition(posX, posY);
}
void SimpleButton::setAnchor(float aX, float aY) {
	float posX = clickArea.x + clickArea.w * anchorX;
	float posY = clickArea.y + clickArea.h * anchorY;

	anchorX = aX;
	anchorY = aY;

	setPosition(posX, posY);
}
void SimpleButton::setPosition(int x, int y) {
	clickArea.x = x - clickArea.w * anchorX;
	clickArea.y = y - clickArea.h * anchorY;
}

void SimpleButton::update() {
	TryCall(OnUpdate);
	lastState = state;
	state = inBounds(clickArea,mouseX,mouseY) && buttonDown(SDL_BUTTON_LEFT);
	if (pressed())
		TryCall(OnLeftPress);
	if (released())
		TryCall(OnLeftRelease);
}
void SimpleButton::render(SDL_Renderer* r) {
	TryCall(OnRender);
	if (state) SDL_SetRenderDrawColor(r, 193, 193, 193, 255);
	else SDL_SetRenderDrawColor(r, 226, 226, 226, 255);
	SDL_RenderFillRect(r, &clickArea);
}