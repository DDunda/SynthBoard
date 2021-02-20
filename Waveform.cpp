#include "Waveform.h"

Waveform::Waveform(Input in, Uint32 samples, ModuleRegistry& registry) : Module(registry), in_input(in), out_output(0.0), rendering(false), bufferLock(SDL_CreateMutex()) {
	points = new SDL_Point[samples];
	buffer = new double[samples];
	bufferLength = samples;
}

Waveform::~Waveform() {
	rendering.store(false);
	SDL_LockMutex(bufferLock);
	delete[] points;
	delete[] buffer;
	points = NULL;
	buffer = NULL;
	SDL_UnlockMutex(bufferLock);
	SDL_DestroyMutex(bufferLock);
}

void Waveform::update(double dT) {
	float mult = dT / 100;
	if (mult < 0) mult = 0;

	max -= mult;
	if (max < 0.001f) max = 0.001f;
}

void Waveform::render(SDL_Renderer* renderer) {
	bufferIndex = 0;
	rendering.store(true);
	while (rendering.load()) SDL_Delay(1);

	SDL_LockMutex(bufferLock);
	if (buffer != NULL) {
		for (int i = 0; i < bufferLength; i++) {
			points[i] = {
			(int)(drawArea.x + i * drawArea.w / (bufferLength - 1)),
			(int)(drawArea.y + (1.0f - (buffer[i] + max) / 2.0f / max) * drawArea.h)
			};
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLines(renderer, points, bufferLength);
	}
	SDL_UnlockMutex(bufferLock);
}

void Waveform::CalculateState() {
	double sound = *in_input;
	if (sound > max) max = sound;
	if (-sound > max) max = -sound;

	if (rendering.load()) {
		SDL_LockMutex(bufferLock);
		if(buffer != NULL)
			buffer[bufferIndex++] = sound;
		SDL_UnlockMutex(bufferLock);

		if (bufferIndex == bufferLength) {
			rendering.store(false);
		}
	}
	out_output.backValue = sound;
}
void Waveform::PresentState() {
	out_output.PresentValue();
}