#pragma once

#ifndef INT_FIELD
#define INT_FIELD

#include "InteractiveElement.h"
#include "Input.h"

class IntField : public InteractiveElement
{
private:
	int visibleCharacters = 5;
	int dstDigitSize;
	int digitGap;
	padding pad;
	float anchorX, anchorY;
	SDL_Rect area;

public:
	std::string capturedData;
	int maxData = -1;
	int caret = 0; // The line thingy
	int output = NULL;

	SDL_Texture* digits;
	int srcDigitSize;

	int flashCycleStart = 0;
	int flashCycle = 500;

	IntField() : InteractiveElement() {}


	void recalculateArea();

	void focus();
	void unfocus();

	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);
	void setVisibleCharacters(int size);
	void setDigitSize(int size);
	void setDigitGap(int size);

	void update();
	void render(SDL_Renderer* renderer);
};

#endif