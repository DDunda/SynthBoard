#pragma once

#ifndef SOUND_H
#define SOUND_H

#ifndef SOUND_FREQUENCY
#define SOUND_FREQUENCY 44100.0
#endif

#include <SDL.h>
#include <vector>
#include <queue>
#include "AbstractedAccess.h"

// t: 0-1, progress of one wave
// Returns wave
typedef float(*synth_func)(float t);

template<class T>
using Pipe = EasyPointer<Source<T>>;

class soundMaker : public Source<float> {
public:
	virtual float getSound() {
		return 0;
	};
	float Get() {
		return getSound();
	}
	virtual void reset();
};

class filter : public soundMaker {
public:
	Pipe<float> source;

	filter(Pipe<float> sound_src);
	void linkSource(Pipe<float> sound_src);
	virtual float getSound();
	float Get() {
		return getSound();;
	}
	virtual void reset();
};

class blendFilter : public soundMaker {
public:
	std::vector<Pipe<float>> sources;
	void addSource(Pipe<float> sound_src);

	virtual float getSound();
	virtual void reset();
};

class blendAdd : public blendFilter {
public:
	float getSound() {
		float sound = 0;
		for (int i = 0; i < sources.size(); i++) sound += sources[i]->Get();
		return sound;
	};
};

class blendMult : public blendFilter {
public:
	float getSound();
};

class dualFilter : public soundMaker {
public:
	Pipe<float> source1;
	Pipe<float> source2;
	dualFilter(Pipe<float> src1, Pipe<float> src2);
	virtual float getSound();
	virtual void reset();
};

class dualAdd : public dualFilter {
public:
	dualAdd(Pipe<float> s1, Pipe<float> s2);
	float getSound();
};

class dualMultiply : public dualFilter {
public:
	dualMultiply(Pipe<float> s1, Pipe<float> s2);
	float getSound();
};

class dualDivide : public dualFilter {
public:
	dualDivide(Pipe<float> s1, Pipe<float> s2);
	float getSound();
};

class propertyWave : public Source<float> {
private:
	float time;
public:
	Pipe<float> min;
	Pipe<float> max;
	Pipe<float> frequency;

	// f, o, min, max 
	propertyWave(Pipe<float> freq, float offset, Pipe<float> min, Pipe<float> max);
	propertyWave(float freq, float offset, float min, float max);

	float Get();
	void reset();
};

class volumeFilter : public filter {
private:
	float internalVolume;

public:
	Pipe<float> targetVolume;
	volumeFilter(Pipe<float> src, Pipe<float> vol);
	float getSound();
	void reset();
};

class Modulator : public Source<float> {
private:
	float time;

public:
	std::vector<float> vals;
	Pipe<float> frequency;

	Modulator(Pipe<float> f);
	Modulator(float f);

	float Get();
	void reset();
};

class synthSound : public soundMaker {
private:
	float time;

public:
	synth_func sound;
	Pipe<float> frequency;
	synthSound(synth_func synth, Pipe<float>& f);
	virtual float getSound();
	void reset();
};

extern const synth_func sineFunc;
extern const synth_func squareFunc;
extern const synth_func triangleFunc;
extern const synth_func sawtoothFunc;

class sineSound : public synthSound {
public:
	sineSound(Pipe<float> f);
};

class squareSound : public synthSound {
public:
	squareSound(Pipe<float> f);
};

class triangleSound : public synthSound {
public:
	triangleSound(EasyPointer <Source<float>> f);
};

class sawtoothSound : public synthSound {
public:
	sawtoothSound(EasyPointer <Source<float>> f);
};

class noiseSound : public soundMaker {
private:
	float progress = 0;
	float amplitude = 0;

	void setRandomAmplitude();

public:
	Pipe<float> frequency;

	noiseSound(Pipe<float> f);

	float getSound();
	void reset();

};

class pulseSound : public soundMaker {
private:
	float time = 0;

public:
	Pipe<float> frequency;
	Pipe<float> duty;

	pulseSound(Pipe<float> f, Pipe<float> d);
	float getSound();

	void reset();

};

class fadeFilter : public filter {
private:
	float volume = 0;
	float lastSample = 0;
	bool stopped = true;
	std::deque<float> resumeBuffer;
	SDL_mutex* bufferLock;

	// "Fade" is the trail left after audio is stopped, a rudimentary fadeout
	float BlendWithFade(float v);

public:
	Pipe<float> finishThreshold;
	Pipe<float> decayRate;

	fadeFilter(Pipe<float> s, Pipe<float> threshold, Pipe<float> decay);
	fadeFilter(Pipe<float> s, float threshold, Pipe<float> decay) : fadeFilter(s, new Val<float>(threshold), decay) {}
	fadeFilter(Pipe<float> s, Pipe<float> threshold, float decay) : fadeFilter(s, threshold, new Val<float>(decay)) {}
	fadeFilter(Pipe<float> s, float threshold, float decay) : fadeFilter(s, new Val<float>(threshold), new Val<float>(decay)) {}
	~fadeFilter();

	bool isStopped();
	bool finished();
	float getSound();

	void stop();
	void start();
	void restart();
	void reset();
};

class LowPassFilter : filter {
protected:
	float cutoff_freq;
	float dt;
	float RC;
	float alpha;

	float lastOutput = 0;
public:
	LowPassFilter(Pipe<float> src, float cutoff);

	float getSound();

	void SetCutoff(float cutoff);
};

class HighPassFilter : filter {
protected:
	float cutoff_freq;
	float dt;
	float RC;
	float alpha;

	float lastRawOutput = 0;
	float lastOutput = 0;
public:
	HighPassFilter(Pipe<float> src, float cutoff);

	float getSound();

	void SetCutoff(float cutoff);
};

class EchoFilter : public filter {
protected:
	float* bufferStart = NULL;
	float* bufferEnd = NULL;
	float* bufferHead = NULL;
	int len = 0;
	Pipe<float> decayRate;
	Pipe<float> echoDuration;
	void SetLen(int l);

public:
	EchoFilter(Pipe<float> src, Pipe<float> duration, Pipe<float> decay);
	~EchoFilter();
	float getSound();
};

#endif