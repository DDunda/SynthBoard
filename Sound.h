#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Module.h"

using namespace std;

static ModuleRegistry soundRegistry;

MakeModule(Sine)
protected:
	double t = 0;

public:
	Input in_frequency;
	SineOutput out_output;

	Sine(Input frequency, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Square)
protected:
	double t = 0;

public:
	Input in_frequency;
	SquareOutput out_output;

	Square(Input frequency, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Sawtooth)
protected:
	double t = 0;

public:
	Input in_frequency;
	SawtoothOutput out_output;

	Sawtooth(Input frequency, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Triangle)
protected:
	double t = 0;

public:
	Input in_frequency;
	TriangleOutput out_output;

	Triangle(Input frequency, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Noise)
private:
	float progress = 0;
	float amplitude = 0;

	void SetRandomAmplitude();

public:
	Input in_frequency;
	NoiseOutput out_output;

	Noise(Input frequency, ModuleRegistry& registry = soundRegistry);
};

MakeModule(SquarePulse)
private:
	double t = 0;

public:
	Input in_frequency;
	Input in_duty;
	SquarePulseOutput out_output;

	SquarePulse(Input input, Input duty, ModuleRegistry& registry = soundRegistry);
};

MakeModule(TrianglePulse)
private:
	double t = 0;

public:
	Input in_frequency;
	Input in_duty;
	TrianglePulseOutput out_output;

	TrianglePulse(Input input, Input duty, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Constant)
public:
	double value = 0;
	ConstantOutput out_output;

	Constant(double value, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Multiplier)
public:
	vector<Input> in_sources;
	MultiplierOutput out_output;

	Multiplier(vector<Input> sources, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Adder)
public:
	vector<Input> in_sources;
	AdderOutput out_output;

	Adder(vector<Input> sources, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Mapper)
public:
	Input in_input;
	Input in_m;
	Input in_c;
	MapperOutput out_output;
	// out = m * in + c
	Mapper(Input input, Input m, Input c, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Flipper)
public:
	Input in_input;
	FlipperOutput out_output;

	Flipper(Input input, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Volume)
protected:
	double internalVolume;

public:
	Input in_input;
	Input in_targetVolume;
	VolumeOutput out_output;

	Volume(Input input, Input targetVolume, ModuleRegistry& registry = soundRegistry);
};

MakeModule(Limiter)
protected:
	double internalVolume;

public:
	Input in_input;
	Input in_max;
	LimiterOutput out_output;

	Limiter(Input input, Input max, ModuleRegistry& registry = soundRegistry);
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

	Fader(Input input, Input attack, Input release, ModuleRegistry& registry = soundRegistry);

	void Stop();
	void Start();
	bool IsOn();
	bool IsChanging();
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

	LowPass(Input input, Input cutoff, ModuleRegistry& registry = soundRegistry);

	void SetCutoff(double cutoff);
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

	HighPass(Input input, Input cutoff, ModuleRegistry& registry = soundRegistry);

	void SetCutoff(double cutoff);
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

	Delay(Input input, Input duration, Input decay, ModuleRegistry& registry = soundRegistry);
	~Delay();
};

MakeModule(BitCrusher)
public:
	Input in_input;
	Input in_resolution;
	BitCrusherOutput out_output;

	BitCrusher(Input input, Input resolution, ModuleRegistry& registry = soundRegistry);
};

MakeModule(BitCrusher2)
public:
	Input in_input;
	Input in_resolution;
	BitCrusher2Output out_output;

	BitCrusher2(Input input, Input resolution, ModuleRegistry& registry = soundRegistry);
};