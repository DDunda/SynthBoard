#pragma once

#include <SDL.h>
#include <atomic>
#include "RenderableElement.h"
#include "Sound.h"

class Waveform : public Module, public Renderable {
	MakeModuleOutput(Waveform)
private:
	SDL_Point* points = NULL;
	SDL_mutex* bufferLock;
	double* buffer = NULL;
	Uint32 bufferLength = 0;
	Uint32 bufferIndex = 0;
	double max = 0;

protected:
	void CalculateState();
	void PresentState();
	void Update(double);
	void Render(SDL_Renderer*);

public:
	SDL_Rect drawArea;

	Input<double> in_input;
	WaveformOutput<double> out_output;

	Waveform(Input<double> in, Uint32 samples, ModuleRegistry& moduleRegistry = ModuleRegistry::globalRegistry, RenderableRegistry& renderRegistry = RenderableRegistry::globalRegistry);
	~Waveform();
};