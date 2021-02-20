#pragma once

#include <SDL.h>
#include <atomic>
#include "RenderableElement.h"
#include "Sound.h"

class Waveform : public Module, public RenderableElement {
	MakeModuleOutput(Waveform)
private:
	SDL_Point* points = NULL;
	SDL_mutex* bufferLock;
	std::atomic<bool> rendering;
	double* buffer = NULL;
	Uint32 bufferLength;
	Uint32 bufferIndex = 0;
	double max = 0;

protected:
	void CalculateState();
	void PresentState();

public:
	SDL_Rect drawArea;

	Input in_input;
	WaveformOutput out_output;

	Waveform(Input in, Uint32 samples, ModuleRegistry& registry = soundRegistry);
	~Waveform();

	virtual void update(double);
	virtual void render(SDL_Renderer*);
};