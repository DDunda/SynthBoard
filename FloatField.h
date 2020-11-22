#pragma once

#ifndef FLOAT_FIELD
#define FLOAT_FIELD

#include "Input.h"
#include "InteractiveElement.h"
#include "AbstractedAccess.h"

class FloatField : public Source<float>, public InteractiveElement
{
private:
	int visibleCharacters = 5;
	int dstDigitSize = 8;
	int digitGap = 2;
	bool has_decimal = false;
	padding pad;
	float anchorX = 0, anchorY = 0;
	bool isNegative = false;
	SDL_Rect area;

public:
	std::string capturedData;
	int maxData = -1;
	size_t caret = 0; // The cursor thingy
	float output = 0;

	SDL_Texture* digits = NULL;
	int srcDigitSize = 8;

	int flashCycleStart = 0;
	int flashCycle = 500;

	float Get();
	void reset();

	FloatField() : InteractiveElement() {}

	void recalculateArea();

	void focus();
	void unfocus();

	void setValue(float);

	void setPosition(int x, int y);
	void setAnchor(float aX, float aY);
	void setVisibleCharacters(int size);
	void setDigitSize(int size);
	void setDigitGap(int size);
	void setPadding(padding pad);

	void update();
	void render(SDL_Renderer* renderer);

	bool inArea(int x, int y);
};

#endif