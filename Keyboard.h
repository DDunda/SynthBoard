#pragma once
#include<SDL.h>
#include<SDL_image.h>

#include "Input.h"
#include "Sound.h"
#include "SoundConstants.h"
#include "AbstractedAccess.h"
#include "RenderableElement.h"
#include "Slider.h"

extern SDL_Rect waveformDrawArea;

class Keyboard;

extern void DrawRectWithBorder(SDL_Renderer* renderer, SDL_Rect area, int t, SDL_Colour inner, SDL_Colour border);

class Key {
protected:
	Keyboard& parent;
	Module* generator;

public:
	Constant frequency;
	Constant faderDecayRate;
	Fader fader;
	Constant cutoffFrequency;
	LowPass frequencyLimiter;
	Output<double>* output;

	int noteNum;
	SDL_Rect area;

	SDL_Scancode key;
	bool active = false;
	bool white = false;

	void SetFrequency(float f) {
		frequency.value = f;
		cutoffFrequency.value = 4 * f;
	}
	void SetOctave(int o) {
		SetFrequency(Octaves::OctaveSet[o]->notes[noteNum]);
	}
	void SetGenerator(Module* m, Input i) {
		if(generator) delete generator;
		generator = m;
		fader.in_input = i;
	}
	Key(Keyboard& p, int octave, int number, SDL_Scancode keycode) : parent(p),
		//frequency(new dualMultiply(new propertyWave(5.0f, 0.0f, 0.99f, 1.01f),rawFrequency)) 
		frequency(0.0),
		noteNum(number),
		faderDecayRate(100.0),
		fader(NULL,&faderDecayRate.out_output,&faderDecayRate.out_output),
		cutoffFrequency(0.0),
		frequencyLimiter(&fader.out_output, &cutoffFrequency.out_output)
	{
		key = keycode;

		white = (number <= 4 && !(number & 1)) || (number > 4 && (number & 1));

		Sine* tmp = new Sine(&frequency.out_output);

		SetGenerator(tmp, &tmp->out_output);

		output = &frequencyLimiter.out_output;

		SetOctave(octave);
	}
	~Key() {
		if(generator) delete generator;
	}
	void stop() {
		fader.Stop();
	}
	void start() {
		fader.Start();
	}
	void Draw() {

	}
};

class Keyboard : public RenderableElement {
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

	std::vector<Key*> keys;
	Adder adder;
	Constant targetVolume;
	Volume volume;
	BitCrusher crusher;
	Delay echo;

	Key& MakeKey(int o, int n, SDL_Scancode k) {
		keys.push_back(new Key(*this, o, n, k));
		return *keys[keys.size()-1];
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
				targetVolume.value = 1.0;
				for (int i = 0; i < numOctaves * 12; i++) {
					SDL_LockMutex(Module::registryLock);
					Sine* tmp = new Sine(&keys[i]->frequency.out_output);
					keys[i]->SetGenerator(tmp, &tmp->out_output);
					SDL_UnlockMutex(Module::registryLock);
				}
				break;
			case 1:
				targetVolume.value = 1.0;
				for (int i = 0; i < numOctaves * 12; i++) {
					SDL_LockMutex(Module::registryLock);
					Triangle* tmp = new Triangle(&keys[i]->frequency.out_output);
					keys[i]->SetGenerator(tmp, &tmp->out_output);
					SDL_UnlockMutex(Module::registryLock);
				}
				break;
			case 2:
				targetVolume.value = 0.3;
				for (int i = 0; i < numOctaves * 12; i++) {
					SDL_LockMutex(Module::registryLock);
					Square* tmp = new Square(&keys[i]->frequency.out_output);
					keys[i]->SetGenerator(tmp, &tmp->out_output);
					SDL_UnlockMutex(Module::registryLock);
				}
				break;
			case 3:
				targetVolume.value = 0.3;
				for (int i = 0; i < numOctaves * 12; i++) {
					SDL_LockMutex(Module::registryLock);
					Sawtooth* tmp = new Sawtooth(&keys[i]->frequency.out_output);
					keys[i]->SetGenerator(tmp, &tmp->out_output);
					SDL_UnlockMutex(Module::registryLock);
				}
				break;
			case 4:
				targetVolume.value = 0.3;
				for (int i = 0; i < numOctaves * 12; i++) {
					SDL_LockMutex(Module::registryLock);
					Noise* tmp = new Noise(&keys[i]->frequency.out_output);
					keys[i]->SetGenerator(tmp, &tmp->out_output);
					SDL_UnlockMutex(Module::registryLock);
				}
				break;
			}
		}
	}

	Slider durSlider;
	Slider decSlider;
	Slider resSlider;

public:
	SDL_Rect renderArea;
	static SDL_Scancode KeyMapping[36];
	int firstOctave = 2;
	int selectedSynth = 0;

	int borderThickness = 5;

	SDL_Texture* keyTexture = NULL;

	int pianoScale = 4;

	Output<double>* output;
	
	void update() {
		onUpdate();

		pianoScale = floor(screenWidth / 356);

		renderArea = {
			(screenWidth - PianoSize.x * pianoScale) / 2,
			screenHeight - PianoSize.y * pianoScale,
			PianoSize.x * pianoScale,
			PianoSize.y * pianoScale
		};

		waveformDrawArea = { 0,0,screenWidth, screenHeight - renderArea.h };

		if (keyPressed(SDLK_LEFT)) {
			if (firstOctave != 0) {
				firstOctave--;
				SetKeyFrequencies();
			}
		}

		if (keyReleased(SDLK_RIGHT)) {
			if (firstOctave != 4) {
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

		onRender(r);
	};

	Keyboard() :
		adder({}),
		targetVolume(1.0),
		volume(&adder.out_output, &targetVolume.out_output),
		resSlider(0.0,1.0,0.0),
		crusher(&volume.out_output, &resSlider.output),
		decSlider(0, 1, 0.25),
		durSlider(0, 1, 0.25),
		echo(&crusher.out_output, &durSlider.output, &decSlider.output),
		output(&echo.out_output) {
		renderArea = { 0,screenHeight / 2, screenWidth, screenHeight / 2 };

		for (int i = 0; i < numOctaves * 12; i++)
			adder.in_sources.push_back(MakeKey(firstOctave + i / 12, i % 12, KeyMapping[i]).output);

		resSlider.setPosition(25, 107);
		resSlider.setSliderWidth(100);
		resSlider.setKnobRadius(5);
		resSlider.setAnchor(0, 0);
		resSlider.setPadding({ 8,10,8,10 });

		decSlider.setPosition(25, 147);
		decSlider.setSliderWidth(100);
		decSlider.setKnobRadius(5);
		decSlider.setAnchor(0, 0);
		decSlider.setPadding({ 8,10,8,10 });

		durSlider.setPosition(25, 187);
		durSlider.setSliderWidth(100);
		durSlider.setKnobRadius(5);
		durSlider.setAnchor(0, 0);
		durSlider.setPadding({ 8,10,8,10 });
	}

	~Keyboard() {

	}
};