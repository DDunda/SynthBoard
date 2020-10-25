#pragma once

#ifndef TEXT_FIELD
#define TEXT_FIELD

#include "Input.h"
#include "InteractiveElement.h"
#include "AbstractedAccess.h"

class TextField;

class TextField : public Source<std::string>, public InteractiveElement
{
private:
	int visibleCharacters = 5;
	int dstCharacterSize = 8;
	int characterGap = 2;
	padding pad;
	float anchorX = 0, anchorY = 0;
public:

	std::string capturedData = "";
	int maxData = -1;
	int caret = 0; // The cursor thingy

	SDL_Texture* characters = NULL;
	int srcCharacterSize = 8;

	int flashCycleStart = 0;
	int flashCycle = 500;

	std::string Get();
	void reset();

	TextField() : InteractiveElement() {}

	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);

	void recalculateArea();

	void focus();
	void unfocus();

	void setValue(const std::string&);

	void setVisibleCharacters(int size) {
		if (size <= 0) return;

		visibleCharacters = size;
		recalculateArea();
	}

	void setCharacterSize(int size) {
		if (size <= 0) return;

		dstCharacterSize = size;
		recalculateArea();
	}

	void setCharacterGap(int size) {
		if (size < 0) return;

		characterGap = size;
		recalculateArea();
	}

	void setPadding(padding pad) {
		this->pad = pad;

		recalculateArea();
	}

	void update();
	void render(SDL_Renderer* renderer);
};

#endif