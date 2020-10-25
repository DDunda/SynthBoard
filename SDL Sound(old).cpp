#include <SDL.h>
#include <iostream>
#include <ctime>

// prototype for our audio callback
// see the implementation for more information
void PushAudio(void* userdata, Uint8* stream, int len);

// Creates a square wave
float MakeSquareWave(Uint32 s, Uint32 t, Uint32 p) {
	if ((((t - s) % p) / (float)p) < 0.5) return 1.0;
	return -1.0;
}

// Creates a sine wave
float MakeSineWave(Uint32 s, Uint32 t, Uint32 p) {
	return sin(((t - s) % p) / (float)p * 2.0 * M_PI);
}

// Creates a saw wave
float MakeSawWave(Uint32 s, Uint32 t, Uint32 p) {
	return (((t - s) % p) / p) * 2 - 1;
}

// Creates a saw wave
float MakeTriangleWave(Uint32 s, Uint32 t, Uint32 p) {
	if ((t - s) % p < p * 0.5) return 4 * (((t - s) % p) / (float)p) - 1;
	return 3 - 4 * (((t - s) % p) / (float)p);
}

Uint32 lastperiod = ~0; // fills all bits (max)
float lastVal = 0;

// Creates a saw wave
float MakeNoise(Uint32 s, Uint32 t, Uint32 p) {
	if ((t - s) / p != lastperiod) {
		lastperiod = (t - s) / p;
		lastVal = ((rand() % 1024) / 1024.0) * (float)((rand() % 3) - 1);
	}
	return lastVal;
}

void Add(float* dst, float src) {
	*dst += src;
}

void SetWave(float* buffer, Uint32 length, Uint32 period, float (*soundFunction)(Uint32 s, Uint32 t, Uint32 p), float volume = 0.005, Uint32 sampleRate = 44100) {
	for (int t = 0; t < length; t++) {
		buffer[t] = (*soundFunction)(0, t, period) * volume;
	}
}

void MixWave(float* buffer, Uint32 length, Uint32 period, float (*soundFunction)(Uint32 s, Uint32 t, Uint32 p), void (*MergeFunction)(float* dst, float src), float volume = 0.005, Uint32 sampleRate = 44100) {
	float* tmp = new float[period];
	for (int t = 0; t < period; t++) {
		tmp[t] = (*soundFunction)(0, t, period) * volume;
	}

	int pos = 0;
	while (pos < length) {
		for (int t = 0; pos < length && t < period; t++) {
			MergeFunction(buffer + pos++, tmp[t]);
		}
	}
	delete[] tmp;
}

/// Post processing ///

void AddWaaWaa(float* buffer, Uint32 length, float dampening, float period, float offset) {
	for (int t = 0; t < length; t++) {
		buffer[t] *= (sin((t + offset) * 2 * M_PI / period) + 1) * 0.5 * (1 - dampening) + dampening;
	}
}

void AddNoise(float* buffer, Uint32 length, float amount, Uint32 persistence, float offset) {
	int t = 0;
	while(t < length) {
		float val = ((rand() % 1000) / 1000.0) * amount * ((rand() % 3) - 1);
		for (int i = 0; t < length && i < persistence; i++) {
			buffer[t++] += val;
		}
	}
}

void SoftenEdges(float* buffer, Uint32 length, Uint32 fadeLen) {
	for (Uint32 t = 0; t < length && t < fadeLen; t++) {
		buffer[t]       *= (1 - cos(t * M_PI / fadeLen)) / 2.0;
		buffer[length - t - 1] *= (1 - cos(t * M_PI / fadeLen)) / 2.0;
	}
}

void CapVolume(float* buffer, Uint32 length, float max) {
	for (Uint32 t = 0; t < length; t++) {
		float val = buffer[t];
		if (val > max)  val = max;
		if (val < -max) val = -max;
		buffer[t] = val;
	}
}

void FitVolume(float* buffer, Uint32 length, float target) {
	float max = 0;
	for (Uint32 t = 0; t < length; t++) {
		float val = buffer[t];
		if (val > max)  max = val;
		if (-val > max) max = -val;
	}

	if (max == 0) return;

	float invMax = target / max;
	for (Uint32 t = 0; t < length; t++) {
		buffer[t] *= invMax;
	}
}

Uint8* audio_pos;
Uint32 audio_len;
float* soundBuffer;
float* sampleBuffer;

int main(int argc, char* argv[]) {

	std::time_t now = std::time(0);

	srand(now >> 32 ^ (now & 0xFFFFFFFF));

	/// Start SDL for just audio ///
	if (SDL_Init(SDL_INIT_AUDIO) < 0) return EXIT_FAILURE;

	/// Sound specifications ///
	unsigned int timelen = 5;
	unsigned int samplingRate = 44100;

	Uint32 length = timelen * samplingRate;
	soundBuffer = new float[length]();
	sampleBuffer = new float[1200]();

	SDL_AudioSpec config;

	config.freq = samplingRate;
	config.format = AUDIO_F32;
	config.channels = 1;
	// ~
	config.samples = 2048;
	// ?
	// ~
	config.callback = PushAudio;
	config.userdata = NULL;

	/// Open the audio device ///
	if (SDL_OpenAudio(&config, NULL) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	int sampleLength = 86 * 3;

	float f4 = 349.23;
	float a4 = 440;
	float b4 = 493.88;
	float c5 = 523.25;
	float d5 = 587.33;
	float e5 = 659.25;
	float g5 = 783.99;


	SetWave(soundBuffer, length, samplingRate / f4, MakeSineWave, 0.005, samplingRate);
	SetWave(sampleBuffer, 1200, samplingRate / f4, MakeSineWave, 0.005, samplingRate);
	MixWave(soundBuffer, length, samplingRate / f4, MakeTriangleWave, Add, 0.005, samplingRate);
	MixWave(sampleBuffer, 1200, samplingRate / f4, MakeTriangleWave, Add, 0.005, samplingRate);
	/*AddWave(soundBuffer, length, samplingRate / a4, MakeSineWave, 0.005, samplingRate);
	AddWave(sampleBuffer, 86 * 3, samplingRate / a4, MakeSineWave, 0.005, samplingRate);
	AddWave(soundBuffer, length, samplingRate / c5, MakeSineWave, 0.005, samplingRate);
	AddWave(sampleBuffer, 86 * 3, samplingRate / c5, MakeSineWave, 0.005, samplingRate);
	AddWaaWaa(soundBuffer, length, 0.9, samplingRate / 2, 0);
	AddWaaWaa(sampleBuffer, 86 * 3, 0.9, samplingRate / 2, 0);*/
	//SetWave(soundBuffer, length, 5, MakeNoise, 1, samplingRate);
	//SetWave(sampleBuffer, 86 * 3, 5, MakeNoise, 1, samplingRate);
	//AddNoise(soundBuffer, length, 0.2, 5, 0);
	//AddNoise(sampleBuffer, 86 * 3, 0.2, 5, 0);

	SoftenEdges(soundBuffer, length, 500);
	SoftenEdges(sampleBuffer, 1200, 500);

	//CapVolume(soundBuffer, length, 0.005);
	FitVolume(soundBuffer, length, 0.005);
	FitVolume(sampleBuffer, 1200, 0.005);


	float lower = -0.005;
	float upper = 0.005;
#define WAVEFORM_SAMPLES 44
	std::string lines[WAVEFORM_SAMPLES];

	/// Print basic waveform ///
	for (int t = 0; t < 300; t++) {
		float fitted = (sampleBuffer[t<<2] - lower) / (upper - lower);
		if (fitted < 0) fitted = 0;
		if (fitted > 1) fitted = 1;

		float val = fitted * (WAVEFORM_SAMPLES - 1);

		for (int i = 0; i < WAVEFORM_SAMPLES; i++) {
			if ((int)round(val) == i) lines[WAVEFORM_SAMPLES - 1 - i] += "_";
			else lines[WAVEFORM_SAMPLES - 1 - i] += " ";
		}
	}
	for (int i = 0; i < WAVEFORM_SAMPLES; i++) std::cout << lines[i] << std::endl;

	/// Set audio stream variables ///
	audio_pos = (Uint8*)soundBuffer;
	audio_len = length * sizeof(float);

	/// Start playing ///
	SDL_PauseAudio(0);

	/// Wait until audio finished ///
	while (audio_len > 0) {
		SDL_Delay(100);
	}

	/// Close resources ///
	SDL_CloseAudio();
	SDL_Quit();

	std::cout << *soundBuffer;

	/// Free buffer ///
	if (soundBuffer != NULL) delete[] soundBuffer;

	/// Exit ///
	return EXIT_SUCCESS;
}

void PushAudio(void* userdata, Uint8* stream, int len) {

	if (audio_len == 0)
		return;

	len = len > audio_len ? audio_len : len;
	SDL_memcpy (stream, audio_pos, len);

	audio_pos += len;
	audio_len -= len;
}