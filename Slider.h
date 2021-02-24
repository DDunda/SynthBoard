#pragma once

#ifndef SLIDER
#define SLIDER

#include "Input.h"
#include "InteractiveElement.h"
#include "RenderableElement.h"
#include "Module.h"

class Slider : public Module, public Interactive, public Renderable
{
	MakeModuleOutput(Slider)
private:
	double sliderWidth = 100;
	double knobRadius = 5;
	int grabOffset = 0;
	padding pad = {0,0,0,0};
	double anchorX = 0, anchorY = 0;
	SDL_Rect area = { 0,0,0,0 };
	bool knobGrabbed = false;
	double sliderPercent = 0;
	bool clickAnywhere = true;

	SDL_Colour fillColour { 63,  63,  63,  255 };
	SDL_Colour boxColour  { 127, 127, 127, 255 };
	SDL_Colour lineColour { 127, 127, 127, 255 };
	SDL_Colour knobColour { 127, 127, 127, 255 };

protected:
	void CalculateState();
	void PresentState();
	void Update(double);
	void Render(SDL_Renderer*);

public:
	double valueA;
	double valueB;

	SliderOutput<double> out_output;

	void recalculateArea();

	void setAnchor(double aX, double aY);
	void setPosition(int x, int y);
	void setPadding(padding pad);
	void setKnobRadius(int r);
	void setSliderWidth(int r);
	void setClickAnywhere(bool c);

	bool InArea(int x, int y) const;

	Slider(double a, double b, double p = 0.5, ModuleRegistry& mRegistry = ModuleRegistry::globalRegistry, InteractiveRegistry& iRegistry = InteractiveRegistry::globalRegistry, RenderableRegistry& rRegistry = RenderableRegistry::globalRegistry);
};

#endif