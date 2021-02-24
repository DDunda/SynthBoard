#pragma once

#ifndef FLOAT_FIELD
#define FLOAT_FIELD

#include "Input.h"
#include "InteractiveElement.h"
#include "RenderableElement.h"
#include "Module.h"

class FloatField : public Module, public Interactive, public Renderable
{
	MakeModuleOutput(FloatField)
private:
	int visibleCharacters = 5;
	int dstDigitSize = 8;
	int digitGap = 2;
	padding pad;
	float anchorX = 0, anchorY = 0;
	SDL_Rect area;
	bool typing = false;
	bool has_decimal = false;
	bool isNegative = false;

protected:
	void startTyping();
	void stopTyping();
	void PresentState();
	void Update(double);
	void Render(SDL_Renderer*);

public:
	std::string capturedData;
	int maxData = -1;
	size_t caret = 0; // The cursor thingy
	FloatFieldOutput<double> out_output = 0;

	SDL_Texture* digits = NULL;
	int srcDigitSize = 8;

	int flashCycleStart = 0;
	int flashCycle = 500;

	void recalculateArea();
	void setValue(double v);

	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);
	void setVisibleCharacters(int size);
	void setDigitSize(int size);
	void setDigitGap(int size);
	void setPadding(padding pad);

	bool InArea(int x, int y) const;

	FloatField(ModuleRegistry& mRegistry = ModuleRegistry::globalRegistry, InteractiveRegistry& iRegistry = InteractiveRegistry::globalRegistry, RenderableRegistry& rRegistry = RenderableRegistry::globalRegistry) : Module(mRegistry), Interactive(iRegistry), Renderable(rRegistry) {}
};

#endif