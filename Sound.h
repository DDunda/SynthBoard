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

namespace noteFrequencies {
	const float
		C0 = 16.35f,
		Cs0 = 17.32f,
		Df0 = 17.32f,
		D0 = 18.35f,
		Ds0 = 19.45f,
		Ef0 = 19.45f,
		E0 = 20.60f,
		F0 = 21.83f,
		Fs0 = 23.12f,
		Gf0 = 23.12f,
		G0 = 24.50f,
		Gs0 = 25.96f,
		Af0 = 25.96f,
		A0 = 27.50f,
		As0 = 29.14f,
		Bf0 = 29.14f,
		B0 = 30.87f,

		C1 = 32.70f,
		Cs1 = 34.65f,
		Df1 = 34.65f,
		D1 = 36.71f,
		Ds1 = 38.89f,
		Ef1 = 38.89f,
		E1 = 41.20f,
		F1 = 43.65f,
		Fs1 = 46.25f,
		Gf1 = 46.25f,
		G1 = 49.00f,
		Gs1 = 51.91f,
		Af1 = 51.91f,
		A1 = 55.00f,
		As1 = 58.27f,
		Bf1 = 58.27f,
		B1 = 61.74f,

		C2 = 65.41f,
		Cs2 = 69.30f,
		Df2 = 69.30f,
		D2 = 73.42f,
		Ds2 = 77.78f,
		Ef2 = 77.78f,
		E2 = 82.41f,
		F2 = 87.31f,
		Fs2 = 92.50f,
		Gf2 = 92.50f,
		G2 = 98.00f,
		Gs2 = 103.83f,
		Af2 = 103.83f,
		A2 = 110.00f,
		As2 = 116.54f,
		Bf2 = 116.54f,
		B2 = 123.47f,

		C3 = 130.81f,
		Cs3 = 138.59f,
		Df3 = 138.59f,
		D3 = 146.83f,
		Ds3 = 155.56f,
		Ef3 = 155.56f,
		E3 = 164.81f,
		F3 = 174.61f,
		Fs3 = 185.00f,
		Gf3 = 185.00f,
		G3 = 196.00f,
		Gs3 = 207.65f,
		Af3 = 207.65f,
		A3 = 220.00f,
		As3 = 233.08f,
		Bf3 = 233.08f,
		B3 = 246.94f,

		C4 = 261.63f,
		Cs4 = 277.18f,
		Df4 = 277.18f,
		D4 = 293.66f,
		Ds4 = 311.13f,
		Ef4 = 311.13f,
		E4 = 329.63f,
		F4 = 349.23f,
		Fs4 = 369.99f,
		Gf4 = 369.99f,
		G4 = 392.00f,
		Gs4 = 415.30f,
		Af4 = 415.30f,
		A4 = 440.00f,
		As4 = 466.16f,
		Bf4 = 466.16f,
		B4 = 493.88f,

		C5 = 523.25f,
		Cs5 = 554.37f,
		Df5 = 554.37f,
		D5 = 587.33f,
		Ds5 = 622.25f,
		Ef5 = 622.25f,
		E5 = 659.25f,
		F5 = 698.46f,
		Fs5 = 739.99f,
		Gf5 = 739.99f,
		G5 = 783.99f,
		Gs5 = 830.61f,
		Af5 = 830.61f,
		A5 = 880.00f,
		As5 = 932.33f,
		Bf5 = 932.33f,
		B5 = 987.77f,

		C6 = 1046.50f,
		Cs6 = 1108.73f,
		Df6 = 1108.73f,
		D6 = 1174.66f,
		Ds6 = 1244.51f,
		Ef6 = 1244.51f,
		E6 = 1318.51f,
		F6 = 1396.91f,
		Fs6 = 1479.98f,
		Gf6 = 1479.98f,
		G6 = 1567.98f,
		Gs6 = 1661.22f,
		Af6 = 1661.22f,
		A6 = 1760.00f,
		As6 = 1864.66f,
		Bf6 = 1864.66f,
		B6 = 1975.53f,

		C7 = 2093.00f,
		Cs7 = 2217.46f,
		Df7 = 2217.46f,
		D7 = 2349.32f,
		Ds7 = 2489.02f,
		Ef7 = 2489.02f,
		E7 = 2637.02f,
		F7 = 2793.83f,
		Fs7 = 2959.96f,
		Gf7 = 2959.96f,
		G7 = 3135.96f,
		Gs7 = 3322.44f,
		Af7 = 3322.44f,
		A7 = 3520.00f,
		As7 = 3729.31f,
		Bf7 = 3729.31f,
		B7 = 3951.07f,

		C8 = 4186.01f,
		Cs8 = 4434.92f,
		Df8 = 4434.92f,
		D8 = 4698.63f,
		Ds8 = 4978.03f,
		Ef8 = 4978.03f,
		E8 = 5274.04f,
		F8 = 5587.65f,
		Fs8 = 5919.91f,
		Gf8 = 5919.91f,
		G8 = 6271.93f,
		Gs8 = 6644.88f,
		Af8 = 6644.88f,
		A8 = 7040.00f,
		As8 = 7458.62f,
		Bf8 = 7458.62f,
		B8 = 7902.13f;
};

namespace Octaves {
	struct Octave {
		union {
			float notes[12];
			struct {
				float C;
				union { float Cs, Df; };
				float D;
				union { float Ds, Ef; };
				float E, F;
				union { float Fs, Gf; };
				float G;
				union { float Gs, Af; };
				float A;
				union { float As, Bf; };
				float B;
			};
		};
	};

#define octave_set(n) Octave o##n { \
	noteFrequencies::C##n,\
	noteFrequencies::Cs##n,\
	noteFrequencies::D##n,\
	noteFrequencies::Ds##n,\
	noteFrequencies::E##n,\
	noteFrequencies::F##n,\
	noteFrequencies::Fs##n,\
	noteFrequencies::G##n,\
	noteFrequencies::Gs##n,\
	noteFrequencies::A##n,\
	noteFrequencies::As##n,\
	noteFrequencies::B##n\
}

	const octave_set(0);
	const octave_set(1);
	const octave_set(2);
	const octave_set(3);
	const octave_set(4);
	const octave_set(5);
	const octave_set(6);
	const octave_set(7);
	const octave_set(8);

#undef octave_set

	const Octave* OctaveSet[];
};

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