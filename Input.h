#pragma once

#ifndef SDL_INPUT_HANDLED
#define SDL_INPUT_HANDLED

#include <SDL.h>
#include <string>
#include <stdexcept>
#include <map>

struct keystate {
	Uint32 down = 0;
	Uint32 up = 0;
};

struct buttonState {
	Uint32 down = 0;
	Sint32 downX = 0, downY = 0;
	Uint32 up = 0;
	Sint32 upX = 0, upY = 0;
};

struct keyboardData {
	Uint32 lastUpdated = 0;
	std::map<SDL_Scancode, keystate> last_keys_scancode;
	std::map<SDL_Keycode, keystate> last_keys_keycode;
	std::map<SDL_Scancode, keystate> keys_scancode;
	std::map<SDL_Keycode, keystate> keys_keycode;
};

extern keyboardData globalKeyboard;

extern std::map <Uint8, buttonState> last_mouse_buttons;
extern std::map <Uint8, buttonState> mouse_buttons;

extern int mouseX;
extern int mouseY;

extern int screenWidth, screenHeight;

extern bool running;

void getEvents();

static bool keyPressed(SDL_Keycode key, keyboardData* pipe = &globalKeyboard) {
	return pipe->keys_keycode[key].down > pipe->last_keys_keycode[key].down;
}
static bool keyReleased(SDL_Keycode key, keyboardData* pipe = &globalKeyboard) {
	return pipe->keys_keycode[key].up > pipe->last_keys_keycode[key].up;
}
static bool keyDown(SDL_Keycode key, keyboardData* pipe = &globalKeyboard) {
	return pipe->keys_keycode[key].down > pipe->keys_keycode[key].up;
}
static bool keyUp(SDL_Keycode key, keyboardData* pipe = &globalKeyboard) {
	return pipe->keys_keycode[key].up > pipe->keys_keycode[key].down;
}

static bool scancodePressed(SDL_Scancode key, keyboardData* pipe = &globalKeyboard) {
	return pipe->keys_scancode[key].down > pipe->last_keys_scancode[key].down;
}
static bool scancodeReleased(SDL_Scancode key, keyboardData* pipe = &globalKeyboard) {
	return pipe->keys_scancode[key].up > pipe->last_keys_scancode[key].up;
}
static bool scancodeDown(SDL_Scancode key, keyboardData* pipe = &globalKeyboard) {
	return pipe->keys_scancode[key].down > pipe->keys_scancode[key].up;
}
static bool scancodeUp(SDL_Scancode key, keyboardData* pipe = &globalKeyboard) {
	return pipe->keys_scancode[key].up > pipe->keys_scancode[key].down;
}

static bool buttonPressed(Uint8 button) {
	return mouse_buttons[button].down > last_mouse_buttons[button].down;
}
static bool buttonReleased(Uint8 button) {
	return mouse_buttons[button].up > last_mouse_buttons[button].up;
}
static bool buttonDown(Uint8 button) {
	return mouse_buttons[button].down > mouse_buttons[button].up;
}
static bool buttonUp(Uint8 button) {
	return mouse_buttons[button].up > mouse_buttons[button].down;
}

static bool inBounds(SDL_Rect area, int x, int y) {
	return x > area.x&& y > area.y&& x < area.x + area.w && y < area.y + area.h;
}

#endif
