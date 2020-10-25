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

	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);

	void recalculateArea();

	void focus();
	void unfocus();

	void setVisibleCharacters(int size) {
		if (size <= 0) return;

		visibleCharacters = size;
		recalculateArea();
	}

	void setDigitSize(int size) {
		if (size <= 0) return;

		dstDigitSize = size;
		recalculateArea();
	}

	void setDigitGap(int size) {
		if (size <= 0) return;

		digitGap = size;
		recalculateArea();
	}

	void update();
	void render(SDL_Renderer* renderer);
};

#endif