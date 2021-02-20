#include "Slider.h"
#include "Helpers.h"

void Slider::CalculateState() {
	output.backValue = valueB * sliderPercent + valueA * (1 - sliderPercent);
}
void Slider::PresentState() {
	output.PresentValue();
}

void Slider::setPosition(int x, int y) {
	area.x = x - area.w * anchorX;
	area.y = y - area.h * anchorY;
}
void Slider::setAnchor(double aX, double aY) {
	int posX = area.x + area.w * anchorX;
	int posY = area.y + area.h * anchorY;

	anchorX = aX;
	anchorY = aY;

	setPosition(posX, posY);
}
void Slider::setPadding(padding p) {
	pad = p;

	recalculateArea();
}
void Slider::setKnobRadius(int s) {
	knobRadius = s;

	recalculateArea();
}
void Slider::setSliderWidth(int w) {
	sliderWidth = w;

	recalculateArea();
}

void Slider::recalculateArea() {
	float globalAnchorX = area.x + area.w * anchorX;
	float globalAnchorY = area.y + area.h * anchorY;

	area.w = sliderWidth + knobRadius * 2 + pad.left + pad.right + 1;
	area.h = knobRadius * 2 + pad.top + pad.bottom + 1;

	setPosition(globalAnchorX, globalAnchorY);
}

void Slider::update(double dT) {
	if (!inFocus() && !knobGrabbed) return;

	SDL_Point a = {
		area.x + pad.left + knobRadius,
		area.y + pad.top + knobRadius
	};
	SDL_Point b = {
		a.x + sliderWidth,
		a.y
	};
	SDL_Point k = {
		b.x * sliderPercent + a.x * (1 - sliderPercent),
		a.y
	};
	SDL_Rect knob {
		k.x - knobRadius,
		k.y - knobRadius,
		knobRadius * 2 + 1,
		knobRadius * 2 + 1
	};

	if (inBounds(knob, mouseX, mouseY) && buttonPressed(SDL_BUTTON_LEFT)) {
		knobGrabbed = true;
		grabOffset = mouseX - k.x;
	}
	if (knobGrabbed) {
		sliderPercent = (mouseX - grabOffset - a.x) / sliderWidth;
		if (sliderPercent > 1) sliderPercent = 1;
		if (sliderPercent < 0) sliderPercent = 0;
		if(buttonReleased(SDL_BUTTON_LEFT))
			knobGrabbed = false;
	}
}
void Slider::render(SDL_Renderer* r) {
	SDL_Point a = {
		area.x + pad.left + knobRadius,
		area.y + pad.top + knobRadius
	};
	SDL_Point b = {
		a.x + sliderWidth,
		a.y
	};
	SDL_Point k = {
		b.x * sliderPercent + a.x * (1 - sliderPercent),
		a.y
	};
	SDL_Rect knob{
		k.x - knobRadius,
		k.y - knobRadius,
		knobRadius * 2 + 1,
		knobRadius * 2 + 1
	};

	SetColour(r, fillColour);
	SDL_RenderFillRect(r, &area);

	SetColour(r, boxColour);
	SDL_RenderDrawRect(r, &area);

	SetColour(r, lineColour);
	SDL_RenderDrawLine(r, a.x, a.y, b.x, b.y);

	SetColour(r, knobColour);
	SDL_RenderFillRect(r, &knob);
}

bool Slider::inArea(int x, int y) {
	return inBounds(area, x, y);
}