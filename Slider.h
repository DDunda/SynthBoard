#pragma once




#include <SDL.h>
#include "InteractiveElement.h"
#include "AbstractedAccess.h"

class Slider : public Source<float>, public InteractiveElement
{
private:
	float sliderPercent = 0;
	float sliderWidth = 100;
	float knobRadius = 5;
	bool knobGrabbed = false;
	int grabOffset = 0;
	padding pad = {0,0,0,0};
	float anchorX = 0, anchorY = 0;

	SDL_Rect area = {0,0,0,0};

	SDL_Colour fillColour { 63,  63,  63,  255 };
	SDL_Colour boxColour  { 127, 127, 127, 255 };
	SDL_Colour lineColour { 127, 127, 127, 255 };
	SDL_Colour knobColour { 127, 127, 127, 255 };

public:
	float valueA;
	float valueB;

	float Get();
	void reset() {};

	Slider(float a, float b, float p = 0.5) : valueA(a), valueB(b), sliderPercent(p) {
		if (sliderPercent > 1) sliderPercent = 1;
		if (sliderPercent < 0) sliderPercent = 0;

		recalculateArea();
	};

	void recalculateArea();

	void setPosition(int x, int y);
	void setAnchor(float aX, float aY);
	void setPadding(padding pad);
	void setKnobRadius(int r);
	void setSliderWidth(int r);

	void update();
	void render(SDL_Renderer*);

	void unfocus() {};
	void focus() {};

	bool inArea(int x, int y);
};