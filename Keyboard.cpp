#include "Keyboard.h"

SDL_Scancode Keyboard::KeyMapping[36] = {
		SDL_SCANCODE_TAB,
		SDL_SCANCODE_1,
		SDL_SCANCODE_Q,
		SDL_SCANCODE_2,
		SDL_SCANCODE_W,
		SDL_SCANCODE_E,
		SDL_SCANCODE_4,
		SDL_SCANCODE_R,
		SDL_SCANCODE_5,
		SDL_SCANCODE_T,
		SDL_SCANCODE_6,
		SDL_SCANCODE_Y,
		SDL_SCANCODE_U,
		SDL_SCANCODE_8,
		SDL_SCANCODE_I,
		SDL_SCANCODE_9,
		SDL_SCANCODE_O,
		SDL_SCANCODE_P,
		SDL_SCANCODE_MINUS,
		SDL_SCANCODE_LEFTBRACKET,
		SDL_SCANCODE_EQUALS,
		SDL_SCANCODE_RIGHTBRACKET,
		SDL_SCANCODE_BACKSPACE,
		SDL_SCANCODE_BACKSLASH,
		SDL_SCANCODE_Z,
		SDL_SCANCODE_S,
		SDL_SCANCODE_X,
		SDL_SCANCODE_D,
		SDL_SCANCODE_C,
		SDL_SCANCODE_V,
		SDL_SCANCODE_G,
		SDL_SCANCODE_B,
		SDL_SCANCODE_H,
		SDL_SCANCODE_N,
		SDL_SCANCODE_J,
		SDL_SCANCODE_M,
};

void DrawRectWithBorder(SDL_Renderer* renderer, SDL_Rect area, int t, SDL_Colour inner, SDL_Colour border) {
	SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
	SDL_RenderFillRect(renderer, &area);

	// Border is bigger than the inside of the rectangle
	if (2 * t > area.w || 2 * t > area.h) return;

	SDL_Rect innerArea = { area.x + t, area.y + t, area.w - 2 * t, area.h - 2 * t };
	SDL_SetRenderDrawColor(renderer, inner.r, inner.g, inner.b, inner.a);
	SDL_RenderFillRect(renderer, &innerArea);
}

const SDL_Rect Keyboard::BlackInactive = {0,0,9,40};
const SDL_Rect Keyboard::BlackActive = {9,0,9,40};
const SDL_Rect Keyboard::WhiteInactive = {18,0,16,68};
const SDL_Rect Keyboard::WhiteActive = {34,0,16,68};
const SDL_Rect Keyboard::Background = {50,0,1,68};

const SDL_Point Keyboard::PianoSize = {356,68};