#include "IntField.h"

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




void IntField::startTyping() {
	flashCycleStart = currentTime;
	typing = true;

	int caretsX = area.x + pad.left + dstDigitSize * 0.5f;
	int careteX = area.x + pad.left + dstDigitSize * 0.5f + (std::min(visibleCharacters, (int)capturedData.size()) - 1) * (dstDigitSize + digitGap);

	if (mouseX <= caretsX)
		caret = 0;
	else if (mouseX > careteX)
		caret = capturedData.size();
	else
		caret = (mouseX - dstDigitSize * 0.5 - area.x - pad.left) / ((long long)dstDigitSize + digitGap) + 1LL;
}
void IntField::stopTyping() {
	typing = false;
	if (capturedData.empty())
		capturedData = "0";

	try
	{
		out_output.backValue = std::stoi(capturedData);
	}
	catch (const std::out_of_range&)
	{
		out_output.backValue = INT_MAX;
		capturedData = std::to_string(out_output.backValue);

		if (caret > capturedData.size())
			caret = capturedData.size();
	}
}

void IntField::PresentState() {
	out_output.PresentValue();
}

void IntField::Update(double dT) {
	if (!typing && buttonReleased(SDL_BUTTON_LEFT) && Interactive::parent.IsFocus(this)) {
		startTyping();
	}
	if (typing && buttonReleased(SDL_BUTTON_LEFT) && !Interactive::parent.IsFocus(this)) {
		stopTyping();
	}
	if (typing) {
		for (auto keypair : globalKeyboard.keys_keycode) {
			bool inputResolved = false;
			if (!typing) break;
			if (!keyPressed(keypair.first)) continue;

			if (numberKeymapping.count(keypair.first)) { // Key just corresponds to a character - no special behaviour
				if (maxData == -1 || capturedData.size() < maxData) {
					if (caret == capturedData.size())
						capturedData += numberKeymapping[keypair.first];
					else
						capturedData.insert(caret, 1, numberKeymapping[keypair.first]);
					flashCycleStart = currentTime;
					caret++;
				}
				inputResolved = true;
			}
			else { // Special characters
				inputResolved = true;
				switch (keypair.first)
				{
				case SDLK_BACKSPACE:
					if (caret != 0) {
						capturedData.erase(--caret, 1);
						flashCycleStart = currentTime;
					}
					break;

				case SDLK_DELETE:
					if (caret != capturedData.size()) capturedData.erase(caret, 1);
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

				case SDLK_KP_ENTER:
				case SDLK_RETURN:
					caret = capturedData.size();
					stopTyping();
					break;

				default:
					inputResolved = false;
				}
			}

			if (inputResolved)
				flashCycleStart = currentTime;
		}
	}
}
void IntField::Render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 45, 40, 38, 255);
	SDL_RenderFillRect(renderer, &area);
	SDL_SetRenderDrawColor(renderer, 206, 228, 234, 255);
	SDL_RenderDrawRect(renderer, &area);

	int start, end;
	if (typing) {
		end = caret;
		if (end < visibleCharacters) end = visibleCharacters;
		if (end > capturedData.size()) end = capturedData.size();

		start = caret - visibleCharacters;
		if (start < 0) start = 0;
	}
	else {
		start = 0;
		end = visibleCharacters;
		if (visibleCharacters > capturedData.size()) end = capturedData.size();
	}

	for (int i = start; i < end; i++) {
		SDL_Rect src{
			(capturedData[i] - '0') * srcDigitSize,
			0,
			srcDigitSize,
			srcDigitSize
		};
		SDL_Rect dst{
			area.x + pad.left + (i - start) * (dstDigitSize + digitGap),
			area.y + pad.top,
			dstDigitSize,
			dstDigitSize
		};
		SDL_RenderCopy(renderer, digits, &src, &dst);
	}
	if (Interactive::parent.IsFocus(this) && (currentTime - flashCycleStart) % (flashCycle * 2) < flashCycle) {
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
				area.x + pad.left + (dstDigitSize + digitGap) * caret - digitGap / 2,
				area.y + pad.top,
				area.x + pad.left + (dstDigitSize + digitGap) * caret - digitGap / 2,
				area.y + pad.top + dstDigitSize
			);
		}
	}
}

void IntField::recalculateArea() {
	float posX = area.x + area.w * anchorX;
	float posY = area.y + area.h * anchorY;

	area.w = visibleCharacters * dstDigitSize + (visibleCharacters - 1) * digitGap + pad.left + pad.right;
	area.h = dstDigitSize + pad.top + pad.bottom;

	setPosition(posX, posY);
}

void IntField::setAnchor(float aX, float aY) {
	float posX = area.x + area.w * anchorX;
	float posY = area.y + area.h * anchorY;

	anchorX = aX;
	anchorY = aY;

	setPosition(posX, posY);
}
void IntField::setPosition(int x, int y) {
	area.x = x - area.w * anchorX;
	area.y = y - area.h * anchorY;
}
void IntField::setPadding(padding pad) {
	this->pad = pad;

	recalculateArea();
}
void IntField::setVisibleCharacters(int size) {
	if (size <= 0) return;

	visibleCharacters = size;
	recalculateArea();
}
void IntField::setDigitSize(int size) {
	if (size <= 0) return;

	dstDigitSize = size;
	recalculateArea();
}
void IntField::setDigitGap(int size) {
	if (size <= 0) return;

	digitGap = size;
	recalculateArea();
}

bool IntField::InArea(int x, int y) const {
	return inBounds(area, x, y);
}

IntField::IntField(ModuleRegistry& mRegistry, InteractiveRegistry& iRegistry, RenderableRegistry& rRegistry) : Module(mRegistry), Interactive(iRegistry), Renderable(rRegistry), out_output(0) {}