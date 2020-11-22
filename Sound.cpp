#include "Sound.h"

const Octaves::Octave* Octaves::OctaveSet[] = { &o0, &o1, &o2, &o3, &o4, &o5, &o6, &o7, &o8 };

void soundMaker::reset() {};

filter::filter(Pipe<float> sound_src) {
	source = sound_src;
}
void filter::linkSource(Pipe<float> sound_src) {
	source = sound_src;
}
float filter::getSound() {
	return source->Get();
}
void filter::reset() {
	source->reset();
}

void blendFilter::addSource(Pipe<float> sound_src) {
	sources.push_back(sound_src);
}
float blendFilter::getSound() {
	return 0;
}
void blendFilter::reset() {
	for (auto src : sources) src->reset();
}

float blendMult::getSound() {
	float sound = 1;
	for (auto src : sources) sound *= src->Get();
	return sound;
};

dualFilter::dualFilter(Pipe<float> src1, Pipe<float> src2) {
	source1 = src1;
	source2 = src2;
}
float dualFilter::getSound() {
	return 0;
}
void dualFilter::reset() {
	source1->reset();
	source2->reset();
}

dualAdd::dualAdd(Pipe<float> s1, Pipe<float> s2) : dualFilter(s1, s2) {}
dualMultiply::dualMultiply(Pipe<float> s1, Pipe<float> s2) : dualFilter(s1, s2) {}
dualDivide::dualDivide(Pipe<float> s1, Pipe<float> s2) : dualFilter(s1, s2) {}

float dualAdd::getSound() {
	return source1->Get() + source2->Get();
}
float dualMultiply::getSound() {
	return source1->Get() * source2->Get();
}
float dualDivide::getSound() {
	return source1->Get() / source2->Get();
}

propertyWave::propertyWave(Pipe<float> freq, float offset, Pipe<float> min, Pipe<float> max) {
	this->min = min;
	this->max = max;
	frequency = freq;
	time = offset;
}
propertyWave::propertyWave(float freq, float offset, float min, float max) {
	this->min = new Val<float>(min);
	this->max = new Val<float>(max);
	frequency = new Val<float>(freq);
	time = offset;
}
float propertyWave::Get() {
	time += frequency->Get() / SOUND_FREQUENCY;
	time = fmod(time, 1.0f);
	float tmpMin = min->Get();
	return (float)((cos((double)time * 2.0 * M_PI) + 1.0f) / 2.0f) * (max->Get() - tmpMin) + tmpMin;
};
void propertyWave::reset() {
	time = 0;
	min->reset();
	max->reset();
	frequency->reset();
}

volumeFilter::volumeFilter(Pipe<float> src, Pipe<float> vol) : filter(src) {
	internalVolume = 0;
	targetVolume = vol;
}
float volumeFilter::getSound() {
	float tmpVolume = targetVolume->Get();
	float sound = source->Get();
	internalVolume = (internalVolume * 9.0f + tmpVolume) / 10.0f;
	return sound * internalVolume;
}
void volumeFilter::reset() {
	filter::reset();
	targetVolume->reset();
	internalVolume = targetVolume->Get();
	targetVolume->reset();
}

Modulator::Modulator(Pipe<float> f) {
	time = 0;
	frequency = f;
}
Modulator::Modulator(float f) {
	time = 0;
	frequency = new Val<float>(f);
}
float Modulator::Get() {
	float tmpFreq = frequency->Get();
	time += tmpFreq / SOUND_FREQUENCY;
	time = fmod(time, 1.0f);
	if (vals.empty()) return 0;
	return vals[floor((double)time * vals.size())] * (double)tmpFreq / SOUND_FREQUENCY;
}
void Modulator::reset() {
	frequency->reset();
	time = 0;
}

synthSound::synthSound(synth_func synth, Pipe<float>& f) {
	time = 0;
	frequency = f;
	sound = synth;
}
float synthSound::getSound() {
	time += frequency->Get() / SOUND_FREQUENCY;
	time = fmod(time, 1.0);
	float w = sound(time);
	return w;
}
void synthSound::reset() {
	time = 0;
	frequency->reset();
}

const synth_func sineFunc = [](float t) -> float {
	return sin(t * 2.0 * M_PI);
};
const synth_func squareFunc = [](float t) -> float {
	return t < 0.5 ? -1 : 1;
};
const synth_func triangleFunc = [](float t) -> float {
	return abs(2.0 - abs(1.0 - 4.0 * t)) - 1.0;
};
const synth_func sawtoothFunc = [](float t) -> float {
	return 2 * t - 1.0;
};

sineSound::sineSound(Pipe<float> f) : synthSound(sineFunc, f) {}
squareSound::squareSound(Pipe<float> f) : synthSound(squareFunc, f) {}
triangleSound::triangleSound(EasyPointer <Source<float>> f) : synthSound(triangleFunc, f) {}
sawtoothSound::sawtoothSound(EasyPointer <Source<float>> f) : synthSound(sawtoothFunc, f) {}

noiseSound::noiseSound(Pipe<float> f) {
	frequency = f;
}
float noiseSound::getSound() {
	progress += frequency->Get() / SOUND_FREQUENCY;
	if (progress > 0.25) {
		progress = fmodf(progress, 0.25);
		setRandomAmplitude();
	}
	return amplitude;
}
void noiseSound::reset() {
	progress = 0;
	setRandomAmplitude();
	frequency->reset();
}
void noiseSound::setRandomAmplitude() {
	amplitude = roundf(rand() / (float)RAND_MAX) * 2 - 1;
}

pulseSound::pulseSound(Pipe<float> f, Pipe<float> d) {
	frequency = f;
	duty = d;
}
float pulseSound::getSound() {
	time += frequency->Get() / SOUND_FREQUENCY;
	if (time > 1)
		time = fmodf(time, 1);
	return time > duty->Get() ? -1 : 1;
}
void pulseSound::reset() {
	time = 0;
	frequency->reset();
}

float fadeFilter::BlendWithFade(float v) {
	SDL_LockMutex(bufferLock);
	if (!resumeBuffer.empty()) {
		v += resumeBuffer.front();
		resumeBuffer.pop_front();
	}
	SDL_UnlockMutex(bufferLock);
	lastSample = v;

	return v;
}

fadeFilter::fadeFilter(Pipe<float> s, Pipe<float> threshold, Pipe<float> decay) : filter(s) {
	finishThreshold = threshold;
	decayRate = decay;
	bufferLock = SDL_CreateMutex();
}
fadeFilter::~fadeFilter() {
	SDL_LockMutex(bufferLock);
	SDL_DestroyMutex(bufferLock);
}

float fadeFilter::getSound() {
	// Fade in to full volume
	if (stopped) return BlendWithFade(0);

	volume += decayRate->Get() / SOUND_FREQUENCY;
	if (volume > 1) volume = 1;
	//return 0;

	return BlendWithFade(source->Get() * volume);
}

bool fadeFilter::isStopped() {
	return stopped;
}
bool fadeFilter::finished() {
	return stopped && resumeBuffer.empty();
}

void fadeFilter::stop() {
	stopped = true;
	SDL_LockMutex(bufferLock);
	for (int i = 0; volume > 0; i++, volume -= decayRate->Get() / SOUND_FREQUENCY) {
		if (i >= resumeBuffer.size())
			resumeBuffer.push_back(0);
		resumeBuffer[i] += source->Get() * volume;
	}
	SDL_UnlockMutex(bufferLock);
	volume = 0;
	source->reset();
	decayRate->reset();
}
void fadeFilter::start() {
	stopped = false;
}
void fadeFilter::restart() {
	stop();
	start();
	finishThreshold->reset();
}
void fadeFilter::reset() {
	restart();
}

LowPassFilter::LowPassFilter(Pipe<float> src, float cutoff) : filter(src) {
	SetCutoff(cutoff);
}

float LowPassFilter::getSound() {
	float output = lastOutput + (alpha * (source->Get() - lastOutput));
	lastOutput = output;
	return output;
}

void LowPassFilter::SetCutoff(float cutoff) {
	RC = 1.0 / (cutoff * 2 * M_PI);
	dt = 1.0 / SOUND_FREQUENCY;
	alpha = dt / (RC + dt);
}

HighPassFilter::HighPassFilter(Pipe<float> src, float cutoff) : filter(src) {
	SetCutoff(cutoff);
}

float HighPassFilter::getSound() {
	float rawOutput = source->Get();
	float output = alpha * (lastOutput + rawOutput - lastRawOutput);
	lastRawOutput = rawOutput;
	lastOutput = output;
	return output;
}

void HighPassFilter::SetCutoff(float cutoff) {
	RC = 1.0 / (cutoff * 2 * M_PI);
	dt = 1.0 / SOUND_FREQUENCY;
	alpha = RC / (RC + dt);
}

EchoFilter::EchoFilter(Pipe<float> src, Pipe<float> duration, Pipe<float> decay) : filter(src), echoDuration(duration) {
	SetLen(floor(duration->Get() * SOUND_FREQUENCY));
	decayRate = decay;
}

EchoFilter::~EchoFilter() {
	delete[] bufferStart;
}

float EchoFilter::getSound() {
	int l = floor(echoDuration->Get() * SOUND_FREQUENCY);
	if(l != len) SetLen(l);
	float d = decayRate->Get();
	float sound = source->Get();
	if (bufferHead) {
		*bufferHead = sound += *bufferHead * d;
		if (++bufferHead == bufferEnd) bufferHead = bufferStart;
	}
	return sound;
}

void EchoFilter::SetLen(int l) {
	float* buffer = NULL;
	if (l != 0) {
		buffer = new float[l];
		for (int i = 0; i < l; i++) buffer[i] = 0.0f;
	}
	if (bufferStart != NULL) {
		if (l < len) {
			int tl = ceil(l / 2.0f);
			int rl = floor(l / 2.0f);

			float* h1 = bufferHead;
			float* h2 = buffer;

			for (int i = 0; i < rl; i++) {
				if (h1 == bufferEnd) h1 = bufferStart;
				*h2 = *h1;
				h1++;
				h2++;
			}

			h1 = bufferHead - 1;
			h2 = buffer + l - 1;

			for (int i = 0; i < tl; i++) {
				if (h1 == bufferStart - 1) h1 = bufferEnd - 1;
				*h2 = *h1;
				h1--;
				h2--;
			}
		}
		else {
			int tl = ceil(len / 2.0f);
			int rl = floor(len / 2.0f);

			float* h1 = bufferHead;
			float* h2 = buffer;

			for (int i = 0; i < rl; i++) {
				if (h1 == bufferEnd) h1 = bufferStart;
				*h2 = *h1;
				h1++;
				h2++;
			}

			h1 = bufferHead - 1;
			h2 = buffer + l - 1;

			for (int i = 0; i < tl; i++) {
				if (h1 == bufferStart - 1) h1 = bufferEnd - 1;
				*h2 = *h1;
				h1--;
				h2--;
			}
		}
		delete[] bufferStart;
	}

	bufferStart = buffer;
	bufferHead = bufferStart;
	bufferEnd = bufferStart + l;
	len = l;
}