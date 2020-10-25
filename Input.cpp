#pragma once

#include "Input.h"

extern Uint32 currentTime;

keyboardData globalKeyboard;

std::map <Uint8, buttonState> last_mouse_buttons;
std::map <Uint8, buttonState> mouse_buttons;

int mouseX;
int mouseY;

int screenWidth, screenHeight;

bool running = true;

std::string* textEditingOutput = NULL;
Sint32* cursorOutput;

void getEvents() {
	static SDL_Event e;

	for (const auto& pair : mouse_buttons)
		last_mouse_buttons[pair.first] = mouse_buttons[pair.first];

	for (const auto& pair : globalKeyboard.keys_scancode)
		globalKeyboard.last_keys_scancode[pair.first] = globalKeyboard.keys_scancode[pair.first];

	for (const auto& pair : globalKeyboard.keys_keycode)
		globalKeyboard.last_keys_keycode[pair.first] = globalKeyboard.keys_keycode[pair.first];

	while (SDL_PollEvent(&e)) {
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;

		case SDL_KEYDOWN:
			globalKeyboard.keys_scancode[e.key.keysym.scancode].down = e.key.timestamp;
			globalKeyboard.keys_keycode[e.key.keysym.sym].down = e.key.timestamp;
			globalKeyboard.lastUpdated = e.key.timestamp;
			break;

		case SDL_KEYUP:
			globalKeyboard.keys_scancode[e.key.keysym.scancode].up = e.key.timestamp;
			globalKeyboard.keys_keycode[e.key.keysym.sym].up = e.key.timestamp;
			globalKeyboard.lastUpdated = e.key.timestamp;
			break;

		case SDL_MOUSEMOTION:
			mouseX = e.motion.x;
			mouseY = e.motion.y;
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[e.button.button].down = e.button.timestamp;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[e.button.button].up = e.button.timestamp;
			break;

		case SDL_TEXTINPUT:
			if (textEditingOutput != NULL)
				*textEditingOutput += e.text.text;

			if(cursorOutput != NULL)
				*cursorOutput += strlen(e.text.text);
			
			break;

		case SDL_WINDOWEVENT:
			switch (e.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				screenWidth = e.window.data1;
				screenHeight = e.window.data2;
				break;
			}
			break;

		default:
			break;
		}
	}
}