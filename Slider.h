#pragma once

#include <SDL.h>
#include "InteractiveElement.h"
#include "AbstractedAccess.h"
#include "Module.h"

class Slider : public Module, public InteractiveElement
{
	class SliderOutput : public Output<double>{
		friend Slider;
		SliderOutput(double d) : Output<double>(d) {}
	};
private:
	double sliderPercent = 0;
	double sliderWidth = 100;
	double knobRadius = 5;
	bool knobGrabbed = false;
	int grabOffset = 0;
	padding pad = {0,0,0,0};
	double anchorX = 0, anchorY = 0;

	SDL_Rect area = {0,0,0,0};

	SDL_Colour fillColour { 63,  63,  63,  255 };
	SDL_Colour boxColour  { 127, 127, 127, 255 };
	SDL_Colour lineColour { 127, 127, 127, 255 };
	SDL_Colour knobColour { 127, 127, 127, 255 };

protected:
	void CalculateState();
	void PresentState();

public:
	double valueA;
	double valueB;

	SliderOutput output;

	Slider(double a, double b, ModuleRegistry& registry, double p = 0.5) : Module(registry), valueA(a), valueB(b), sliderPercent(p), output(0.0) {
		if (sliderPercent > 1) sliderPercent = 1;
		if (sliderPercent < 0) sliderPercent = 0;
		output.backValue = a + sliderPercent * (b - a);

		recalculateArea();
	};

	void recalculateArea();

	void setPosition(int x, int y);
	void setAnchor(double aX, double aY);
	void setPadding(padding pad);
	void setKnobRadius(int r);
	void setSliderWidth(int r);

	void update(double);
	void render(SDL_Renderer*);

	void unfocus() {};
	void focus() {};

	bool inArea(int x, int y);
};