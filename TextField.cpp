#include "TextField.h"

extern Uint32 currentTime;
extern std::string* textEditingOutput;
extern Sint32* cursorOutput;

static std::map<SDL_Keycode, char> unshiftedKeymapping
{
	{SDLK_TAB, '\t'},
	{SDLK_SPACE, ' '},
	{SDLK_EXCLAIM, '!'},
	{SDLK_QUOTEDBL, '"'},
	{SDLK_HASH, '#'},
	{SDLK_PERCENT, '%'},
	{SDLK_DOLLAR, '$'},
	{SDLK_AMPERSAND, '&'},
	{SDLK_QUOTE, '\''},
	{SDLK_LEFTPAREN, '('},
	{SDLK_RIGHTPAREN, ')'},
	{SDLK_ASTERISK, '*'},
	{SDLK_PLUS, '+'},
	{SDLK_COMMA, ','},
	{SDLK_MINUS, '-'},
	{SDLK_PERIOD, '.'},
	{SDLK_SLASH, '/'},
	{SDLK_0, '0'},
	{SDLK_1, '1'},
	{SDLK_2, '2'},
	{SDLK_3, '3'},
	{SDLK_4, '4'},
	{SDLK_5, '5'},
	{SDLK_6, '6'},
	{SDLK_7, '7'},
	{SDLK_8, '8'},
	{SDLK_9, '9'},
	{SDLK_COLON, ':'},
	{SDLK_SEMICOLON, ';'},
	{SDLK_LESS, '<'},
	{SDLK_EQUALS, '='},
	{SDLK_GREATER, '>'},
	{SDLK_QUESTION, '?'},
	{SDLK_AT, '@'},
	{SDLK_LEFTBRACKET, '['},
	{SDLK_BACKSLASH, '\\'},
	{SDLK_RIGHTBRACKET, ']'},
	{SDLK_CARET, '^'},
	{SDLK_UNDERSCORE, '_'},
	{SDLK_BACKQUOTE, '`'},

	{SDLK_KP_PERIOD, '.'},
	{SDLK_KP_DIVIDE, '/'},
	{SDLK_KP_MULTIPLY, '*'},
	{SDLK_KP_MINUS, '-'},
	{SDLK_KP_PLUS, '+'},
	{SDLK_KP_EQUALS, '=' },
	{SDLK_KP_0, '0'},
    {SDLK_KP_1, '1'},
    {SDLK_KP_2, '2'},
    {SDLK_KP_3, '3'},
    {SDLK_KP_4, '4'},
    {SDLK_KP_5, '5'},
    {SDLK_KP_6, '6'},
    {SDLK_KP_7, '7'},
    {SDLK_KP_8, '8'},
    {SDLK_KP_9, '9'},
};

#undef concat
#undef sq

static std::map<SDL_Keycode, char> shiftedKeymapping
{
	{SDLK_TAB, '\t'},
	{SDLK_SPACE, ' '},
	{SDLK_EXCLAIM, '!'},
	{SDLK_QUOTEDBL, '"'},
	{SDLK_HASH, '#'},
	{SDLK_PERCENT, '%'},
	{SDLK_DOLLAR, '$'},
	{SDLK_AMPERSAND, '&'},
	{SDLK_QUOTE, '"'},
	{SDLK_LEFTPAREN, '('},
	{SDLK_RIGHTPAREN, ')'},
	{SDLK_ASTERISK, '*'},
	{SDLK_PLUS, '+'},
	{SDLK_COMMA, '<'},
	{SDLK_MINUS, '_'},
	{SDLK_PERIOD, '>'},
	{SDLK_SLASH, '?'},
	{SDLK_0, ')'},
	{SDLK_1, '!'},
	{SDLK_2, '@'},
	{SDLK_3, '#'},
	{SDLK_4, '$'},
	{SDLK_5, '%'},
	{SDLK_6, '^'},
	{SDLK_7, '&'},
	{SDLK_8, '*'},
	{SDLK_9, '('},
	{SDLK_COLON, ':'},
	{SDLK_SEMICOLON, ':'},
	{SDLK_LESS, '<'},
	{SDLK_EQUALS, '+'},
	{SDLK_GREATER, '>'},
	{SDLK_QUESTION, '?'},
	{SDLK_AT, '@'},
	{SDLK_LEFTBRACKET, '{'},
	{SDLK_BACKSLASH, '|'},
	{SDLK_RIGHTBRACKET, '}'},
	{SDLK_CARET, '^'},
	{SDLK_UNDERSCORE, '_'},
	{SDLK_BACKQUOTE, '~'},

	{SDLK_KP_DIVIDE, '/'},
	{SDLK_KP_MULTIPLY, '*'},
	{SDLK_KP_MINUS, '-'},
	{SDLK_KP_PLUS, '+'},
	{SDLK_KP_EQUALS, '='},
};

void TextField::setAnchor(float aX, float aY) {
	float posX = area.x + area.w * anchorX;
	float posY = area.y + area.h * anchorY;

	anchorX = aX;
	anchorY = aY;

	setPosition(posX, posY);
}
void TextField::setPosition(int x, int y) {
	area.x = x - area.w * anchorX;
	area.y = y - area.h * anchorY;
}

void TextField::recalculateArea() {
	float globalAnchorX = area.x + area.w * anchorX;
	float globalAnchorY = area.y + area.h * anchorY;

	area.w = visibleCharacters * dstCharacterSize + (visibleCharacters - 1) * characterGap + pad.left + pad.right;
	area.h = dstCharacterSize + pad.top + pad.bottom;

	setPosition(globalAnchorX, globalAnchorY);
}

void TextField::focus() {
	//keyboardPipe = &keyPipe;
	flashCycleStart = currentTime;
	textEditingOutput = &capturedData;
	cursorOutput = &caret;
	SDL_StartTextInput();
}

void TextField::unfocus() {
	//keyboardPipe = &globalKeyboard;
	SDL_StopTextInput();
	textEditingOutput = NULL;
	cursorOutput = NULL;
}

void TextField::setValue(const std::string& s) {
	capturedData = s;

	if (maxData != -1 && s.size() > maxData)
		capturedData.erase(maxData, capturedData.size() - maxData);

	if (caret > capturedData.size())
		caret = capturedData.size();
}

void TextField::update() {
	if (!inFocus()) return;

	if (buttonPressed(SDL_BUTTON_LEFT)) {
		int caretsX = area.x + pad.left + dstCharacterSize * 0.5;
		int careteX = area.x + pad.left + dstCharacterSize * 0.5 + (std::min(visibleCharacters, (int)capturedData.size()) - 1) * (dstCharacterSize + characterGap);

		if (mouseX <= caretsX)
			caret = 0;
		else if (mouseX > careteX)
			caret = capturedData.size();
		else
			caret = (mouseX - dstCharacterSize * 0.5 - area.x - pad.left) / (dstCharacterSize + characterGap) + 1;
	}

	SDL_Keymod mod = SDL_GetModState();
	std::map<SDL_Keycode, char>& selectedMap = 
		(mod & (KMOD_LSHIFT | KMOD_RSHIFT)) != 0
		? shiftedKeymapping
		: unshiftedKeymapping;

	bool shift = mod & (KMOD_LSHIFT | KMOD_RSHIFT);

	for (auto keypair : globalKeyboard.keys_keycode) {
		bool inputResolved = false;
		if (!keyPressed(keypair.first)) continue;

		/*if (maxData == -1 || capturedData.size() < maxData) {
			if (selectedMap.count(keypair.first)) { // Key just corresponds to a character - no special behaviour
				if (caret == capturedData.size())
					capturedData += selectedMap[keypair.first];
				else
					capturedData.insert(caret, 1, selectedMap[keypair.first]);
				caret++;
				inputResolved = true;
			} else if(keypair.first >= 'a' && keypair.first <= 'z') {
				char input = keypair.first;
				bool caps = mod & KMOD_CAPS;
				bool shift = mod & (KMOD_LSHIFT | KMOD_RSHIFT);
				if (caps && !shift || !caps && shift)
					input = input + ('A' - 'a');

				if (caret == capturedData.size())
					capturedData += input;
				else
					capturedData.insert(caret, 1, input);
				caret++;
				inputResolved = true;
			}
		}*/

		if (!inputResolved) {
			inputResolved = true;
			switch (keypair.first)
			{
			case SDLK_BACKSPACE:
				if (caret != 0) capturedData.erase(--caret, 1);
				break;

			case SDLK_KP_PERIOD:
				if (!shift) break;
			case SDLK_DELETE:
				if (caret != capturedData.size()) capturedData.erase(caret, 1);
				break;

			case SDLK_KP_4:
				if (!shift) break;
			case SDLK_LEFT:
				if (caret != 0) caret--;
				break;

			case SDLK_KP_6:
				if (!shift) break;
			case SDLK_RIGHT:
				if (caret != capturedData.size()) caret++;
				break;

			case SDLK_KP_7:
				if (!shift) break;
			case SDLK_HOME:
				caret = 0;
				break;

			case SDLK_KP_1:
				if (!shift) break;
			case SDLK_END:
				caret = capturedData.size();
				break;

			case SDLK_KP_ENTER:
			case SDLK_RETURN:
				unfocus();
				break;

			default:
				inputResolved = false;
			}
		}

		if (inputResolved)
			flashCycleStart = currentTime;
	}

	onUpdate();
}

void TextField::render(SDL_Renderer* renderer) {
	onRender(renderer);
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
		start = 0;
		end = visibleCharacters;
		if (visibleCharacters > capturedData.size()) end = capturedData.size();
	}


	for (int i = start; i < end; i++) {
		SDL_Rect src  {
				capturedData[i] * srcCharacterSize,
				0,
				srcCharacterSize,
				srcCharacterSize
			};

		SDL_Rect dst{
			area.x + pad.left + (i - start) * (dstCharacterSize + characterGap),
			area.y + pad.top,
			dstCharacterSize,
			dstCharacterSize
		};
		SDL_RenderCopy(renderer, characters, &src, &dst);
	}
	if (inFocus() && (currentTime - flashCycleStart) % (flashCycle * 2) < flashCycle) {
		if (caret >= visibleCharacters) {
			SDL_RenderDrawLine(renderer,
				area.x + pad.left + (dstCharacterSize + characterGap) * visibleCharacters - characterGap / 2,
				area.y + pad.top,
				area.x + pad.left + (dstCharacterSize + characterGap) * visibleCharacters - characterGap / 2,
				area.y + pad.top + dstCharacterSize
			);
		}
		else {
			SDL_RenderDrawLine(renderer,
				area.x + pad.left + (dstCharacterSize + characterGap) * caret - characterGap / 2,
				area.y + pad.top,
				area.x + pad.left + (dstCharacterSize + characterGap) * caret - characterGap / 2,
				area.y + pad.top + dstCharacterSize
			);
		}
	}
}

std::string TextField::Get() {
	return capturedData;
}
void TextField::reset() {}