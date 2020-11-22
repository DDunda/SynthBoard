#include "FloatField.h"

extern Uint32 currentTime;

static std::map<SDL_Keycode, char> numberKeymapping
{
	{SDLK_0,'0'}, // Regular numbers
	{SDLK_1,'1'},
	{SDLK_2,'2'},
	{SDLK_3,'3'},
	{SDLK_4,'4'},
	{SDLK_5,'5'},
	{SDLK_6,'6'},
	{SDLK_7,'7'},
	{SDLK_8,'8'},
	{SDLK_9,'9'},

	{SDLK_KP_0,'0'}, // Keypad numbers
	{SDLK_KP_1,'1'},
	{SDLK_KP_2,'2'},
	{SDLK_KP_3,'3'},
	{SDLK_KP_4,'4'},
	{SDLK_KP_5,'5'},
	{SDLK_KP_6,'6'},
	{SDLK_KP_7,'7'},
	{SDLK_KP_8,'8'},
	{SDLK_KP_9,'9'}
};


void FloatField::recalculateArea() {
	float globalAnchorX = area.x + area.w * anchorX;
	float globalAnchorY = area.y + area.h * anchorY;

	area.w = visibleCharacters * dstDigitSize + (visibleCharacters - 1) * digitGap + pad.left + pad.right;
	area.h = dstDigitSize + pad.top + pad.bottom;

	setPosition(globalAnchorX, globalAnchorY);
}

void FloatField::focus() {
	//keyboardPipe = &keyPipe;
	flashCycleStart = currentTime;
}
void FloatField::unfocus() {
	//keyboardPipe = &globalKeyboard;
	if (!capturedData.empty()) {
		try
		{
			setValue(std::stof(capturedData));
		}
		catch (const std::out_of_range&)
		{
			setValue(FLT_MAX);
		}
	}
	else {
		setValue(0);
	}
}

void FloatField::setValue(float v) {
	output = v;
	capturedData = std::to_string(v);

	isNegative = v < 0;

	has_decimal = true;

	if (maxData != -1)
		capturedData.erase(maxData, capturedData.size() - maxData);

	for (long long i = (long long)capturedData.size() - 1; i >= 0; i--) {
		if (capturedData[i] == '.') {
			capturedData.erase(i, capturedData.size() - i);
			has_decimal = false;
			break;
		}
		else if (capturedData[i] != '0') {
			capturedData.erase(i + 1, capturedData.size() - i - 1);
			break;
		}
	}

	if (caret > capturedData.size())
		caret = capturedData.size();
}

void FloatField::setPosition(int x, int y) {
	area.x = x - area.w * anchorX;
	area.y = y - area.h * anchorY;
}
void FloatField::setAnchor(float aX, float aY) {
	int posX = area.x + area.w * anchorX;
	int posY = area.y + area.h * anchorY;

	anchorX = aX;
	anchorY = aY;

	setPosition(posX, posY);
}
void FloatField::setVisibleCharacters(int size) {
	if (size <= 0) return;

	visibleCharacters = size;
	recalculateArea();
}
void FloatField::setDigitSize(int size) {
	if (size <= 0) return;

	dstDigitSize = size;
	recalculateArea();
}
void FloatField::setDigitGap(int size) {
	if (size < 0) return;

	digitGap = size;
	recalculateArea();
}
void FloatField::setPadding(padding pad) {
	this->pad = pad;

	recalculateArea();
}

void FloatField::update() {
	if (inFocus()) {
		if (buttonPressed(SDL_BUTTON_LEFT)) {
			int caretsX = area.x + pad.left + dstDigitSize * 0.5f;
			int careteX = area.x + pad.left + dstDigitSize * 0.5f + (std::min(visibleCharacters, (int)capturedData.size()) - 1) * (dstDigitSize + digitGap);

			if (mouseX <= caretsX)
				caret = 0;
			else if (mouseX > careteX)
				caret = capturedData.size();
			else
				caret = (mouseX - dstDigitSize * 0.5 - area.x - pad.left) / ((long long)dstDigitSize + digitGap) + 1LL;
		}

		for (auto keypair : globalKeyboard.keys_keycode) {
			if (keyPressed(keypair.first)) {
				if (numberKeymapping.count(keypair.first)) { // Key just corresponds to a character - no special behaviour
					if (maxData == -1 || capturedData.size() < maxData) {
						if (caret == capturedData.size())
							capturedData += numberKeymapping[keypair.first];
						else
							capturedData.insert(caret, 1, numberKeymapping[keypair.first]);
						flashCycleStart = currentTime;
						caret++;
					}
				}
				else { // Special characters
					switch (keypair.first)
					{
					case SDLK_BACKSPACE:
						if (caret != 0) {
							if (capturedData[caret - 1] == '.') has_decimal = false;
							capturedData.erase(--caret, 1);
							flashCycleStart = currentTime;
							if (caret == 0 && isNegative) isNegative = false;
						}
						break;

					case SDLK_DELETE:
						if (capturedData[caret] == '.') has_decimal = false;
						if (caret != capturedData.size()) capturedData.erase(caret, 1);
						if (caret == 0 && isNegative) isNegative = false;
						break;

					case SDLK_LEFT:
						if (caret != 0) {
							caret--;
							flashCycleStart = currentTime;
						}
						break;

					case SDLK_RIGHT:
						if (caret != capturedData.size()) {
							caret++;
							flashCycleStart = currentTime;
						}
						break;

					case SDLK_HOME:
						if (caret != 0) flashCycleStart = currentTime;
						caret = 0;
						break;

					case SDLK_END:
						if (caret != capturedData.size()) flashCycleStart = currentTime;
						caret = capturedData.size();
						break;

					case SDLK_PERIOD:
					case SDLK_KP_PERIOD:
						if (!has_decimal) {
							if (maxData == -1 || capturedData.size() < maxData) {
								if (caret == capturedData.size())
									capturedData += '.';
								else
									capturedData.insert(caret, 1, '.');
								flashCycleStart = currentTime;
								caret++;
								has_decimal = true;
							}
						}
						break;

					case SDLK_MINUS:
					case SDLK_KP_MINUS:
						isNegative = !isNegative;
						if (isNegative) {
							caret++;
							capturedData.insert(0, 1, '-');
						}
						else {
							if (caret) caret--;
							capturedData.erase(0, 1);
						}
						break;

					case SDLK_RETURN:
						caret = capturedData.size();
						unfocus();
						break;
					}
				}
			}
		}
	}
}
void FloatField::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 45, 40, 38, 255);
	SDL_RenderFillRect(renderer, &area);
	SDL_SetRenderDrawColor(renderer, 206, 228, 234, 255);
	SDL_RenderDrawRect(renderer, &area);

	int start, end;
	if (inFocus()) {
		end = caret;
		if (end < visibleCharacters) end = visibleCharacters;
		if (end > capturedData.size()) end = capturedData.size();

		start = caret - visibleCharacters;
		if (start < 0) start = 0;
	}
	else {
		setValue(output);
		start = 0;
		end = visibleCharacters;
		if (visibleCharacters > capturedData.size()) end = capturedData.size();
	}


	for (int i = start; i < end; i++) {
		SDL_Rect src;
		switch (capturedData[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			src = {
				(capturedData[i] - '0') * srcDigitSize,
				0,
				srcDigitSize,
				srcDigitSize
			};
			break;
		case '.':
			src = {
				10 * srcDigitSize,
				0,
				srcDigitSize,
				srcDigitSize
			};
			break;
		case '-':
			src = {
				11 * srcDigitSize,
				0,
				srcDigitSize,
				srcDigitSize
			};
			break;
		default:
			src = {
				10 * srcDigitSize,
				0,
				srcDigitSize,
				srcDigitSize
			};
			break;
		}

		SDL_Rect dst{
			area.x + pad.left + (i - start) * (dstDigitSize + digitGap),
			area.y + pad.top,
			dstDigitSize,
			dstDigitSize
		};
		SDL_RenderCopy(renderer, digits, &src, &dst);
	}
	if (inFocus() && (currentTime - flashCycleStart) % (flashCycle * 2) < flashCycle) {
		if (caret >= visibleCharacters) {
			SDL_RenderDrawLine(renderer,
				area.x + pad.left + (dstDigitSize + digitGap) * visibleCharacters - digitGap / 2,
				area.y + pad.top,
				area.x + pad.left + (dstDigitSize + digitGap) * visibleCharacters - digitGap / 2,
				area.y + pad.top + dstDigitSize
			);
		}
		else {
			SDL_RenderDrawLine(renderer,
				area.x + pad.left + ((long long)dstDigitSize + digitGap) * caret - digitGap / 2,
				area.y + pad.top,
				area.x + pad.left + ((long long)dstDigitSize + digitGap) * caret - digitGap / 2,
				area.y + pad.top + dstDigitSize
			);
		}
	}
}

float FloatField::Get() {
	return output;
}
void FloatField::reset() {}

bool FloatField::inArea(int x, int y) {
	return inBounds(area, x, y);
}