#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <map>

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
#include "Waveform.h"

#define SOUND_BUFFER_SIZE 441
Sint32 soundBuffer[SOUND_BUFFER_SIZE];

void PushAudio(void* userdata, Uint8* stream, int len);

SDL_Texture* num_text;
SDL_Texture* char_text;

Uint32 lastTime;
Uint32 currentTime;
float deltaTime;

bool soundRunning = true;

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
	config.format = AUDIO_S32;
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
	volSlider = new Slider(0.0f, 1.0f, soundRegistry, 0.05f);
	volSlider->setPosition(25, 27);
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

	Volume volMod(board.output, &volSlider->output);

	Constant decay(1000.0);
	Fader fader(
		&volMod.out_output,
		&decay.out_output,
		&decay.out_output
	);

	//Constant frequencyCutoff(20000);
	//LowPass frequencyLimiter(&fader.out_output,&frequencyCutoff.out_output);
	LowPassButter frequencyLimiter(&fader.out_output, 8000.0, 2);

	Waveform waveform(&frequencyLimiter.out_output, 441);
	waveform.drawArea = { 0,0,640,360 };

	waveOutput = &waveform.out_output;

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
		RenderableElement::UpdateAllElements(deltaTime);
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

		waveform.drawArea = { 0,0,screenWidth, screenHeight - board.renderArea.h };

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

void PushAudio(void* userdata, Uint8* stream, int len) {
	int numSamples = len / sizeof(Sint32);
	if (waveOutput != NULL) {
		SDL_LockMutex(soundRegistry.lock);
		for (int i = 0; i < numSamples; i++) {
			soundRegistry.CalculateAllModuleStatesUnsafe();
			soundRegistry.PresentAllModuleStatesUnsafe();
			Sint64 t = *waveOutput * (Sint64)0x80000000;
			if (t > SDL_MAX_SINT32) t = SDL_MAX_SINT32;
			if (t < SDL_MIN_SINT32) t = SDL_MIN_SINT32;
			soundBuffer[i] = t;
		}
		SDL_UnlockMutex(soundRegistry.lock);
	}
	else {
		for (int i = 0; i < numSamples; i++) {
			soundBuffer[i] = 0;
		}
	}

	memcpy(stream, soundBuffer, len);
}