#pragma once
#include<SDL.h>

#include "Input.h"
#include "Sound.h"
#include "AbstractedAccess.h"
#include "RenderableElement.h"

extern SDL_Rect waveformDrawArea;

class Keyboard;

extern void DrawRectWithBorder(SDL_Renderer* renderer, SDL_Rect area, int t, SDL_Colour inner, SDL_Colour border);

class Key : public Source<float> {
protected:
	Keyboard& parent;

public:
	EasyPointer<fVal> rawFrequency;
	Pipe<float> frequency;
	EasyPointer<fadeFilter> sound;
	LowPassFilter* gate;
	int noteNum;
	SDL_Rect area;

	SDL_Scancode key;
	bool active = false;
	bool white = false;

	void SetFrequency(float f) {
		rawFrequency->Set(f);
		gate->SetCutoff(4 * f);
	}
	void SetOctave(int o) {
		SetFrequency(Octaves::OctaveSet[o]->notes[noteNum]);
	}
	Key(Keyboard& p, int octave, int number, SDL_Scancode keycode) : parent(p),
		noteNum(number),
		rawFrequency(new fVal(0.0f)),
		//frequency(new dualMultiply(new propertyWave(5.0f, 0.0f, 0.99f, 1.01f),rawFrequency)) 
		frequency(rawFrequency)
	{
		key = keycode;

		white = (number <= 4 && !(number & 1)) || (number > 4 && (number & 1));

		sound = new fadeFilter(new sineSound(frequency), 0.01f, 100.0f);

		gate = new LowPassFilter(sound, rawFrequency->Get());

		SetOctave(octave);
	}
	~Key() {
		delete sound;
	}
	float Get() {
		return gate->getSound();
	};
	void stop() {
		sound->stop();
	}
	void start() {
		sound->start();
	}
	void reset() {
		sound->reset();
	}

	void Draw() {

	}
};

class Keyboard : public Source<float>, public RenderableElement {
protected:
	int numOctaves = 3;

	SDL_Rect CalculateBlackArea(int n) {
		int w_left = renderArea.w * n / (numOctaves * 7);
		int w_right = renderArea.w * (n + 1) / (numOctaves * 7);

		int w_w = w_right - w_left;
		int w_h = renderArea.h;
		int w_x = renderArea.x + w_left;
		int w_y = renderArea.y;

		int b_w = w_w / 2;
		int b_h = w_h / 2;
		int b_x = w_x + w_w * 3 / 4;
		int b_y = w_y;

		return { b_x,b_y,b_w,b_h };
	}

	SDL_Rect CalculateWhiteArea(int n) {
		int left = renderArea.w * n / (numOctaves * 7);
		int right = renderArea.w * (n + 1) / (numOctaves * 7);

		int w = right - left;
		int h = renderArea.h;
		int x = left;
		int y = renderArea.y;
		return { x,y,w,h };
	}

	void renderBlack(SDL_Renderer* r, bool s, SDL_Rect area) {
		if(!s) return DrawRectWithBorder(r, area, borderThickness, { 56,56,56,255 }, { 25,25,25,255 });

		SDL_SetRenderDrawColor(r, 25, 25, 25, 255);
		SDL_RenderFillRect(r, &area);
	}
	void renderWhite(SDL_Renderer* r, bool s, SDL_Rect area) {
		if (!s) return DrawRectWithBorder(r, area, borderThickness, { 255,255,255,255 }, { 225,225,225,255 });

		SDL_SetRenderDrawColor(r, 225, 225, 225, 255);
		SDL_RenderFillRect(r, &area);
	}

	std::vector<EasyPointer<Key>> keys;
	EasyPointer<blendAdd> adder;
	EasyPointer<EchoFilter> echo;

	EasyPointer<Key> MakeKey(int o, int n, SDL_Scancode k) {
		EasyPointer<Key> key = EasyPointer<Key>(new Key(*this, o, n, k));
		keys.push_back(key);
		return key;
	}

	void SetKeyFrequencies() {
		for (int i = 0; i < numOctaves * 12; i++) {
			keys[i]->SetOctave(i/12 + firstOctave);
		}
	}

	void SetSynths(int synth) {
		synth = ((synth % 5) + 5) % 5;
		if (synth != selectedSynth) {
			// Sine Triangle Square Sawtooth Noise
			selectedSynth = synth;
			switch (selectedSynth)
			{
			case 0:
				for (int i = 0; i < numOctaves * 12; i++) {
					keys[i]->sound->source = new sineSound(keys[i]->frequency);
				}
				break;
			case 1:
				for (int i = 0; i < numOctaves * 12; i++) {
					keys[i]->sound->source = new triangleSound(keys[i]->frequency);
				}
				break;
			case 2:
				for (int i = 0; i < numOctaves * 12; i++) {
					//keys[i]->sound->source = new dualMultiply(new fVal(0.3f), new squareSound(keys[i]->frequency));
					keys[i]->sound->source = new squareSound(keys[i]->frequency);
				}
				break;
			case 3:
				for (int i = 0; i < numOctaves * 12; i++) {
					//keys[i]->sound->source = new dualMultiply(new fVal(0.3f), new sawtoothSound(keys[i]->frequency));
					keys[i]->sound->source = new sawtoothSound(keys[i]->frequency);
				}
				break;
			case 4:
				for (int i = 0; i < numOctaves * 12; i++) {
					//keys[i]->sound->source = new dualMultiply(new fVal(0.3f), new noiseSound(keys[i]->frequency));
					keys[i]->sound->source = new noiseSound(keys[i]->frequency);
				}
				break;
			}
		}
	}

public:
	SDL_Rect renderArea;
	static SDL_Scancode KeyMapping[36];
	int firstOctave = 3;
	int selectedSynth = 0;

	int borderThickness = 5;
	
	void update() {
		TryCall(OnUpdate);
		renderArea = { 0,screenHeight / 2, screenWidth, screenHeight / 2 };
		waveformDrawArea = { 0,0,screenWidth, screenHeight / 2 };

		if (keyPressed(SDLK_LEFT)) {
			if (firstOctave != 0) {
				firstOctave--;
				SetKeyFrequencies();
			}
		}

		if (keyReleased(SDLK_RIGHT)) {
			if (firstOctave != 6) {
				firstOctave++;
				SetKeyFrequencies();
			}
		}

		if (keyPressed(SDLK_UP)) {
			SetSynths(selectedSynth + 1);
		}

		if (keyPressed(SDLK_DOWN)) {
			SetSynths(selectedSynth - 1);
		}

		bool blackKeyClicked = false;

		int blackNum = 0;
		for (auto key : keys) {
			if (!key->white) {
				bool lastState = key->active;
				key->active = false;
				key->area = CalculateBlackArea(blackNum++);
				if ((blackNum % 7) == 2 || (blackNum % 7) == 6) blackNum++;

				if (scancodeDown(key->key)) key->active = true;
				if (buttonDown(SDL_BUTTON_LEFT) && inBounds(key->area, mouseX, mouseY)) {
					key->active = true;
					blackKeyClicked = true;
				}

				if (lastState == key->active) continue;

				if (key->active) key->start();
				else key->stop();

			}
		}

		int whiteNum = 0;
		for (auto key : keys) {
			if (key->white) {
				bool lastState = key->active;
				key->active = false;
				key->area = CalculateWhiteArea(whiteNum++);

				if (scancodeDown(key->key)) key->active = true;
				// White keys can be under black keys, so a black key being clicked means a white key cannot be
				if (buttonDown(SDL_BUTTON_LEFT) && inBounds(key->area, mouseX, mouseY) && !blackKeyClicked)	key->active = true;

				if (lastState == key->active) continue;

				if (key->active) key->start();
				else key->stop();
			}
		}
	};
	void render(SDL_Renderer* r) {
		TryCall(OnRender);

		borderThickness = renderArea.w / ((long long)numOctaves * 7 * 10);

		int whiteNum = 0;
		for (auto key : keys) {
			if (key->white) {
				renderWhite(r, key->active, key->area);
				whiteNum++;
			}
		}

		int blackNum = 0;
		for (auto key : keys) {
			if (!key->white) {
				renderBlack(r, key->active, key->area);

				blackNum++;
				if ((blackNum % 7) == 2 || (blackNum % 7) == 6) blackNum++;
			}
		}
	};

	Keyboard() {
		renderArea = { 0,screenHeight / 2, screenWidth, screenHeight / 2 };

		adder = new blendAdd();
		for (int i = 0; i < numOctaves * 12; i++) {
			adder->addSource(MakeKey(firstOctave + i / 12, i % 12, KeyMapping[i]));
		}

		echo = new EchoFilter(adder, 0.25, new fVal(0.25));
	}

	float Get() {
		/*float v = 0;
		for (int i = 0; i < keys.size(); i++)
			v += keys[i]->Get();
		return v;*/
		return echo->getSound();
	}
};