#pragma once
#include<SDL.h>
#include<SDL_image.h>

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

	const static SDL_Rect BlackInactive;
	const static SDL_Rect BlackActive;
	const static SDL_Rect WhiteInactive;
	const static SDL_Rect WhiteActive;
	const static SDL_Rect Background;

	const static SDL_Point PianoSize;

	SDL_Rect CalculateBlackArea(int n) {
		int x = renderArea.x + (17 * n + 13) * pianoScale;
		int y = renderArea.y;

		int w = 7 * pianoScale;
		int h = 40 * pianoScale;

		return { x,y,w,h };
	}

	SDL_Rect CalculateWhiteArea(int n) {
		int x = renderArea.x + 17 * n * pianoScale;
		int y = renderArea.y;

		int w = 16 * pianoScale;
		int h = 68 * pianoScale;
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

	SDL_Texture* keyTexture = NULL;

	int pianoScale = 4;
	
	void update() {
		TryCall(OnUpdate);

		pianoScale = floor(screenWidth / 356);

		renderArea = {
			(screenWidth - PianoSize.x * pianoScale) / 2,
			screenHeight - PianoSize.y * pianoScale,
			PianoSize.x * pianoScale,
			PianoSize.y * pianoScale
		};

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
		if (keyTexture == NULL)	keyTexture = IMG_LoadTexture(r, "keys.png");

		TryCall(OnRender);

		//borderThickness = renderArea.w / ((long long)numOctaves * 7 * 10);

		SDL_SetRenderDrawColor(r, 51, 51, 86, 255);
		SDL_Rect bannerDst{
			0,
			screenHeight - PianoSize.y * pianoScale,
			screenWidth,
			PianoSize.y * pianoScale
		};
		SDL_RenderFillRect(r, &bannerDst);

		SDL_RenderCopy(r, keyTexture, &Background, &renderArea);

		SDL_Rect src;
		SDL_Rect dst;

		int whiteNum = 0;
		for (auto key : keys) {
			if (key->white) {
				dst = {
					renderArea.x + whiteNum * 17 * pianoScale,
					renderArea.y,
					16 * pianoScale,
					68 * pianoScale
				};

				if (key->active) src = WhiteActive;
				else src = WhiteInactive;

				SDL_RenderCopy(r, keyTexture, &src, &dst);

				whiteNum++;
			}
		}

		int blackNum = 0;
		for (auto key : keys) {
			if (!key->white) {
				dst = {
					renderArea.x + (blackNum * 17 + 12) * pianoScale,
					renderArea.y,
					 9 * pianoScale,
					40 * pianoScale
				};

				if (key->active) src = BlackActive;
				else src = BlackInactive;

				SDL_RenderCopy(r, keyTexture, &src, &dst);

				blackNum++;
				if ((blackNum % 7) == 2 || (blackNum % 7) == 6) blackNum++;
			}
		}

		/*int whiteNum = 0;
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
		}*/
	};

	Keyboard() {
		renderArea = { 0,screenHeight / 2, screenWidth, screenHeight / 2 };

		adder = new blendAdd();
		for (int i = 0; i < numOctaves * 12; i++) {
			adder->addSource(MakeKey(firstOctave + i / 12, i % 12, KeyMapping[i]));
		}

		echo = new EchoFilter(adder, 0.25, new fVal(0.25));
	}

	~Keyboard() {

	}

	float Get() {
		/*float v = 0;
		for (int i = 0; i < keys.size(); i++)
			v += keys[i]->Get();
		return v;*/
		return echo->getSound();
	}
};