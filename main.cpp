#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <atomic>

#define SOUND_FREQUENCY 44100.0

#include "Keyboard.h"
#include "Sound.h"
#include "Input.h"
#include "RenderableElement.h"
#include "InteractiveElement.h"
#include "FloatField.h"
#include "TextField.h"
#include "SimpleButton.h"
#include "Slider.h";

#define SOUND_BUFFER_SIZE 441
float soundBuffer[SOUND_BUFFER_SIZE];

void PushAudio(void* userdata, Uint8* stream, int len);

SDL_Texture* num_text;
SDL_Texture* char_text;

float soundMin = 0;
float soundMax = 0;
int samples = 441;

SDL_Rect waveformDrawArea = { 0,0,640,360 };
Uint32 lastTime;
Uint32 currentTime;
float deltaTime;

bool soundRunning = true;
std::atomic<bool> renderRequested(false);

Input waveOutput;
SDL_Renderer* renderer = NULL;

SDL_Window* window = NULL;

void initialiseSDL() {
	std::time_t now = std::time(0);

	srand(now >> 32 ^ (now & 0xFFFFFFFF));

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		exit(-1);
	}

	SDL_AudioSpec config;
	config.freq = (int)SOUND_FREQUENCY;
	config.format = AUDIO_F32;
	config.channels = 1;
	config.callback = PushAudio;
	config.samples = SOUND_BUFFER_SIZE;

	if (SDL_OpenAudio(&config, NULL) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	screenWidth = 640;
	screenHeight = 360;
	if (SDL_CreateWindowAndRenderer(screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE, &window, &renderer) != 0) {
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		exit(-1);
	}

	SDL_SetWindowTitle(window, "SynthBoard");

	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP)) {
		IMG_Quit();
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		exit(-1);
	}
}

Slider* volSlider;

void SetupVolSlider() {
	volSlider = new Slider(0.0f, 1.0f, 0.05f);
	volSlider->setPosition(25, 67);
	volSlider->setSliderWidth(100);
	volSlider->setKnobRadius(5);
	volSlider->setAnchor(0, 0);
	volSlider->setPadding({ 8,10,8,10 });
}

int main(int argc, char* argv[]) {
	initialiseSDL();	

	num_text = IMG_LoadTexture(renderer, "nums.png");
	char_text = IMG_LoadTexture(renderer, "chars.png");

	//SetupFileField();
	//SetupPlayButton();
	SetupVolSlider();
	//SetupFreqField();

	Keyboard board;
	Constant sineF(1000.0);
	Sine sine(&sineF.out_output);

	Volume volMod(board.output, &volSlider->output);

	Constant decay(1000.0);
	Fader fader(
		&volMod.out_output,
		&decay.out_output,
		&decay.out_output
	);
	waveOutput = &(fader.out_output);

	SDL_PauseAudio(0);

	lastTime = SDL_GetTicks();

	int lastFrameEnd = SDL_GetTicks();

	fader.Start();
	
	while (running || soundRunning) {
		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (float)(currentTime - lastTime) / 1000.0f;
		getEvents();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		InteractiveElement::UpdateElementFocus();
		RenderableElement::UpdateAllElements();
		Updater::updateAllSources();

		/*for (int i = 0; i < harmonics; i++) {
			*volumes[i]->targetVolume = 
				powf(10,F1vField->output / 10.0) / (powf((i + 1 - F1Field->output) / F1sField->output, 2.0) + 1.0) +
				powf(10,F2vField->output / 10.0) / (powf((i + 1 - F2Field->output) / F2sField->output, 2.0) + 1.0) +
				powf(10,F3vField->output / 10.0) / (powf((i + 1 - F3Field->output) / F3sField->output, 2.0) + 1.0);
		}*/

		if (!running && fader.IsOn())
			fader.Stop();

		if (!fader.IsOn() && !fader.IsChanging())
			soundRunning = false;

		float mult = deltaTime / 100;
		if (mult < 0) mult = 0;

		soundMax -= mult;
		
		renderRequested.store(true);
		while (renderRequested.load()) SDL_Delay(1);

		RenderableElement::RenderAllElements(renderer);
		SDL_RenderPresent(renderer);

		int frameEnd = SDL_GetTicks();
		int tmp = 16 - (frameEnd - lastFrameEnd);
		if (tmp > 0)
			SDL_Delay(tmp);
		lastFrameEnd = frameEnd;
	}

	SDL_DestroyTexture(num_text);

	IMG_Quit();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	/// Close resources ///
	SDL_CloseAudio();
	SDL_Quit();

	/// Exit ///
	return EXIT_SUCCESS;
}


int li = 0;
int soundDrawn = 0;
void PushAudio(void* userdata, Uint8* stream, int len) {
	SDL_LockMutex(Module::registryLock);
	if (waveOutput != NULL) {
		if (renderRequested.load()) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			for (int i = 0; i < len / sizeof(float); i++) {
				Module::CalculateAllModuleStates();
				Module::PresentAllModuleStates();
				soundBuffer[i] = *waveOutput;
				if (soundBuffer[i] > soundMax) soundMax = soundBuffer[i];
				if (-soundBuffer[i] > soundMax) soundMax = -soundBuffer[i];
				if (soundMax < 0.001f) soundMax = 0.001f;

				if (renderRequested.load()) {
					int lx = waveformDrawArea.x + (soundDrawn - 1) * waveformDrawArea.w / samples;
					int x = waveformDrawArea.x + soundDrawn * waveformDrawArea.w / samples;
					///if (!finalFilter->finished()) {
						float fitted = 1.0f - (soundBuffer[li] / 2.0f + soundMax) / 2.0f / soundMax;
						float nextFitted = 1.0f - (soundBuffer[i] / 2.0f + soundMax) / 2.0f / soundMax;
						SDL_RenderDrawLine(renderer,
							lx,
							waveformDrawArea.y + (int)(waveformDrawArea.h * fitted),
							x,
							waveformDrawArea.y + (int)(waveformDrawArea.h * nextFitted));
					/*}
					else {
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
						SDL_RenderDrawLine(renderer, x, waveformDrawArea.y, x, waveformDrawArea.y + waveformDrawArea.h);
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
						SDL_RenderDrawPoint(renderer, x, waveformDrawArea.y + waveformDrawArea.h / 2);
					}*/
					soundDrawn++;
				}

				if (soundDrawn == samples) {
					renderRequested.store(false);
					soundDrawn = 0;
				}
				li = i;
			}
		}
		else {
			for (int i = 0; i < len / sizeof(float); i++) {
				Module::CalculateAllModuleStates();
				Module::PresentAllModuleStates();
				soundBuffer[i] = *waveOutput;
				if (soundBuffer[i] > soundMax) soundMax = soundBuffer[i];
				if (-soundBuffer[i] > soundMax) soundMax = -soundBuffer[i];
				if (soundMax < 0.001f) soundMax = 0.001f;
				li = i;
			}
		}
	}
	else {
		if (renderRequested.load()) {
			for (int i = 0; i < len / sizeof(float); i++) {
				soundBuffer[i] = 0;
				if (renderRequested.load()) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderDrawLine(renderer, soundDrawn, 0, soundDrawn, 360);
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderDrawPoint(renderer, soundDrawn - 1, 179);
					soundDrawn++;
				}

				if (soundDrawn == samples) {
					renderRequested.store(false);
					soundDrawn = 0;
					break;
				}
				li = i;
			}
		}
		else {
			for (int i = 0; i < len / sizeof(float); i++) {
				soundBuffer[i] = 0;
				li = i;
			}
		}
	}

	SDL_UnlockMutex(Module::registryLock);
	SDL_memcpy(stream, soundBuffer, len);
}