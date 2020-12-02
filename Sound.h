#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Module.h"

#define MakeModule(name) \
class name : public Module {\
	class name##Output : public Output<double> {\
		friend name;\
		name##Output(double d) : Output<double>(d) {}\
	};\
	void CalculateState();\
	void PresentState();

using namespace std;
typedef Output<double>* Input;

MakeModule(Sine)
protected:
	double t = 0;
public:
	Input in_frequency;
	SineOutput out_output;
	Sine(Input frequency);
};

MakeModule(Square)
protected:
	double t = 0;
public:
	Input in_frequency;
	SquareOutput out_output;
	Square(Input frequency);
};

MakeModule(Sawtooth)
protected:
	double t = 0;
public:
	Input in_frequency;
	SawtoothOutput out_output;
	Sawtooth(Input frequency);
};

MakeModule(Triangle)
protected:
	double t = 0;
public:
	Input in_frequency;
	TriangleOutput out_output;
	Triangle(Input frequency);
};

MakeModule(Noise)
private:
	float progress = 0;
	float amplitude = 0;

	void SetRandomAmplitude();

public:
	Input in_frequency;
	NoiseOutput out_output;

	Noise(Input frequency);
};

MakeModule(Constant)
public:
	double value = 0;
	ConstantOutput out_output;
	Constant(double value);
};

MakeModule(Multiplier)
public:
	vector<Input> in_sources;
	MultiplierOutput out_output;
	Multiplier(vector<Input> sources);
};

MakeModule(Adder)
public:
	vector<Input> in_sources;
	AdderOutput out_output;
	Adder(vector<Input> sources);
};

MakeModule(Mapper)
public:
	Input in_input;
	Input in_m;
	Input in_c;
	MapperOutput out_output;
	// out = m * in + c
	Mapper(Input input, Input m, Input c);
};

MakeModule(Flipper)
public:
	Input in_input;
	FlipperOutput out_output;
	Flipper(Input input);
};

MakeModule(Volume)
protected:
	double internalVolume;
public:
	Input in_input;
	Input in_targetVolume;
	VolumeOutput out_output;
	Volume(Input input, Input targetVolume);
};

MakeModule(Limiter)
protected:
	double internalVolume;
public:
	Input in_input;
	Input in_max;
	LimiterOutput out_output;
	Limiter(Input input, Input max);
};

MakeModule(Fader)
protected:
	double volume = 0.0;
	bool state = false;
	bool changing = false;
public:
	Input in_input;
	Input in_attackRate;
	Input in_releaseRate;
	FaderOutput out_output;
	void Stop();
	void Start();
	bool IsOn();
	bool IsChanging();
	Fader(Input input, Input attack, Input release);
};

MakeModule(LowPass)
protected:
	double cutoff_freq;
	double dt;
	double RC;
	double alpha;

	double lastOutput = 0;
public:
	Input in_input;
	Input in_cutoff;
	LowPassOutput out_output;

	void SetCutoff(double cutoff);
	LowPass(Input input, Input cutoff);
};

MakeModule(HighPass)
protected:
	double cutoff_freq;
	double dt;
	double RC;
	double alpha;

	double lastRawOutput = 0;
	double lastOutput = 0;
public:
	Input in_input;
	Input in_cutoff;
	HighPassOutput out_output;

	void SetCutoff(double cutoff);
	HighPass(Input input, Input cutoff);
};

MakeModule(Delay)
protected:
	double* bufferStart = NULL;
	double* bufferEnd = NULL;
	double* bufferHead = NULL;
	int len = 0;
	void SetLen(int l);

public:
	Input in_input;
	Input in_decayRate;
	Input in_echoDuration;
	DelayOutput out_output;

	Delay(Input input, Input duration, Input decay);
	~Delay();
};

MakeModule(BitCrusher)
public:
	Input in_input;
	Input in_resolution;
	BitCrusherOutput out_output;
	BitCrusher(Input input, Input resolution);
};