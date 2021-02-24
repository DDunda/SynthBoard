#include "Waveform.h"

Waveform::Waveform(Input<double> in, Uint32 samples, ModuleRegistry& registry1, RenderableRegistry& registry2) : Module(registry1), Renderable(registry2), in_input(in), out_output(0.0), bufferLock(SDL_CreateMutex()) {
	points = new SDL_Point[samples];
	buffer = new double[samples];
	bufferLength = samples;
	drawArea = { 0,0,0,0 };
}

Waveform::~Waveform() {
	SDL_LockMutex(bufferLock);
	delete[] points;
	delete[] buffer;
	points = NULL;
	buffer = NULL;
	SDL_UnlockMutex(bufferLock);
	SDL_DestroyMutex(bufferLock);
}

void Waveform::CalculateState() {
	double sound = *in_input;
	if (sound > max) max = sound;
	if (-sound > max) max = -sound;

	SDL_LockMutex(bufferLock);
	if (buffer) {
		buffer[bufferIndex++] = sound;
		bufferIndex %= bufferLength;
	}
	SDL_UnlockMutex(bufferLock);

	out_output.backValue = sound;
}
void Waveform::PresentState() {
	out_output.PresentValue();
}

void Waveform::Update(double dT) {
	float mult = dT / 100;
	if (mult < 0) mult = 0;

	max -= mult;
	if (max < 0.001f) max = 0.001f;
}
void Waveform::Render(SDL_Renderer* renderer) {
	SDL_LockMutex(bufferLock);
	if (buffer) {
		for (int i = 0; i < bufferLength; i++) {
			points[i] = {
			(int)(drawArea.x + i * drawArea.w / (bufferLength - 1)),
			(int)(drawArea.y + (1.0f - (buffer[(bufferIndex + i) % bufferLength] + max) / 2.0f / max) * drawArea.h)
			};
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLines(renderer, points, bufferLength);
	}
	SDL_UnlockMutex(bufferLock);
}