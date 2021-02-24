#pragma once

#ifndef INT_FIELD
#define INT_FIELD

#include "Input.h"
#include "InteractiveElement.h"
#include "RenderableElement.h"
#include "Module.h"

class IntField : public Module, public Interactive, public Renderable
{
	MakeModuleOutput(IntField)
private:
	int visibleCharacters = 5;
	int dstDigitSize = 8;
	int digitGap = 2;
	padding pad = {0,0,0,0};
	float anchorX = 0, anchorY = 0;
	SDL_Rect area = {0,0,0,0};
	bool typing = false;
	std::string capturedData = "0";

protected:
	void startTyping();
	void stopTyping();
	void PresentState();
	void Update(double);
	void Render(SDL_Renderer*);

public:
	int maxData = -1;
	size_t caret = 0; // The line thingy

	SDL_Texture* digits = NULL;
	int srcDigitSize = 8;

	int flashCycleStart = 0;
	int flashCycle = 500;

	IntFieldOutput<int> out_output;

	void recalculateArea();

	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);
	void setPadding(padding pad);
	void setVisibleCharacters(int size);
	void setDigitSize(int size);
	void setDigitGap(int size);

	bool InArea(int x, int y) const;

	IntField(ModuleRegistry& mRegistry = ModuleRegistry::globalRegistry, InteractiveRegistry& iRegistry = InteractiveRegistry::globalRegistry, RenderableRegistry& rRegistry = RenderableRegistry::globalRegistry);
};

#endif