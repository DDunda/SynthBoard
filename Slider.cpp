#include "Slider.h"
#include "Helpers.h"

void Slider::CalculateState() {
	out_output.backValue = valueB * sliderPercent + valueA * (1 - sliderPercent);
}
void Slider::PresentState() {
	out_output.PresentValue();
}

void Slider::Update(double dT) {
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

	if (buttonPressed(SDL_BUTTON_LEFT) && !knobGrabbed) {
		if (inBounds(knob, mouseX, mouseY)) {
			knobGrabbed = true;
			grabOffset = mouseX - k.x;
		}
		else if (clickAnywhere && InArea(mouseX,mouseY)) {
			knobGrabbed = true;
			grabOffset = 0;
		}
	}
	if (knobGrabbed) {
		sliderPercent = (mouseX - grabOffset - a.x) / sliderWidth;
		if (sliderPercent > 1) sliderPercent = 1;
		if (sliderPercent < 0) sliderPercent = 0;
		if (buttonReleased(SDL_BUTTON_LEFT))
			knobGrabbed = false;
	}
}
void Slider::Render(SDL_Renderer* renderer) {
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

	SetColour(renderer, fillColour);
	SDL_RenderFillRect(renderer, &area);

	SetColour(renderer, boxColour);
	SDL_RenderDrawRect(renderer, &area);

	SetColour(renderer, lineColour);
	SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);

	SetColour(renderer, knobColour);
	SDL_RenderFillRect(renderer, &knob);
}

void Slider::recalculateArea() {
	float globalAnchorX = area.x + area.w * anchorX;
	float globalAnchorY = area.y + area.h * anchorY;

	area.w = sliderWidth + knobRadius * 2 + pad.left + pad.right + 1;
	area.h = knobRadius * 2 + pad.top + pad.bottom + 1;

	setPosition(globalAnchorX, globalAnchorY);
}

void Slider::setAnchor(double aX, double aY) {
	int posX = area.x + area.w * anchorX;
	int posY = area.y + area.h * anchorY;

	anchorX = aX;
	anchorY = aY;

	setPosition(posX, posY);
}
void Slider::setPosition(int x, int y) {
	area.x = x - area.w * anchorX;
	area.y = y - area.h * anchorY;
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
void Slider::setClickAnywhere(bool c) {
	clickAnywhere = c;
}

bool Slider::InArea(int x, int y) const {
	return inBounds(area, x, y);
}

Slider::Slider(double a, double b, double p, ModuleRegistry& mRegistry, InteractiveRegistry& iRegistry, RenderableRegistry& rRegistry) : Module(mRegistry), Interactive(iRegistry), Renderable(rRegistry), valueA(a), valueB(b), sliderPercent(p), out_output(0.0) {
	if (sliderPercent > 1) sliderPercent = 1;
	if (sliderPercent < 0) sliderPercent = 0;
	out_output.backValue = a + sliderPercent * (b - a);

	recalculateArea();
};