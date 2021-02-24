#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Module.h"

using namespace std;

MakeModule(Sine)
protected:
	double t = 0;

public:
	Input<double> in_frequency;
	SineOutput<double> out_output;

	Sine(Input<double> frequency, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Square)
protected:
	double t = 0;

public:
	Input<double> in_frequency;
	SquareOutput<double> out_output;

	Square(Input<double> frequency, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Sawtooth)
protected:
	double t = 0;

public:
	Input<double> in_frequency;
	SawtoothOutput<double> out_output;

	Sawtooth(Input<double> frequency, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Triangle)
protected:
	double t = 0;

public:
	Input<double> in_frequency;
	TriangleOutput<double> out_output;

	Triangle(Input<double> frequency, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Noise)
private:
	float progress = 0;
	float amplitude = 0;

	void SetRandomAmplitude();

public:
	Input<double> in_frequency;
	NoiseOutput<double> out_output;

	Noise(Input<double> frequency, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(SquarePulse)
private:
	double t = 0;

public:
	Input<double> in_frequency;
	Input<double> in_duty;
	SquarePulseOutput<double> out_output;

	SquarePulse(Input<double> input, Input<double> duty, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(TrianglePulse)
private:
	double t = 0;

public:
	Input<double> in_frequency;
	Input<double> in_duty;
	TrianglePulseOutput<double> out_output;

	TrianglePulse(Input<double> input, Input<double> duty, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Constant)
public:
	double value = 0;
	ConstantOutput<double> out_output;

	Constant(double value, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Multiplier)
public:
	vector<Input<double>> in_sources;
	MultiplierOutput<double> out_output;

	Multiplier(vector<Input<double>> sources, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Adder)
public:
	vector<Input<double>> in_sources;
	AdderOutput<double> out_output;

	Adder(vector<Input<double>> sources, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Mapper)
public:
	Input<double> in_input;
	Input<double> in_m;
	Input<double> in_c;
	MapperOutput<double> out_output;
	// out = m * in + c
	Mapper(Input<double> input, Input<double> m, Input<double> c, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Flipper)
public:
	Input<double> in_input;
	FlipperOutput<double> out_output;

	Flipper(Input<double> input, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Volume)
protected:
	double internalVolume;

public:
	Input<double> in_input;
	Input<double> in_targetVolume;
	VolumeOutput<double> out_output;

	Volume(Input<double> input, Input<double> targetVolume, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Limiter)
protected:
	double internalVolume;

public:
	Input<double> in_input;
	Input<double> in_max;
	LimiterOutput<double> out_output;

	Limiter(Input<double> input, Input<double> max, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(Fader)
protected:
	double volume = 0.0;
	bool state = false;
	bool changing = false;

public:
	Input<double> in_input;
	Input<double> in_attackRate;
	Input<double> in_releaseRate;
	FaderOutput<double> out_output;

	Fader(Input<double> input, Input<double> attack, Input<double> release, ModuleRegistry& registry = ModuleRegistry::globalRegistry);

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
	Input<double> in_input;
	Input<double> in_cutoff;
	LowPassOutput<double> out_output;

	LowPass(Input<double> input, Input<double> cutoff, ModuleRegistry& registry = ModuleRegistry::globalRegistry);

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
	Input<double> in_input;
	Input<double> in_cutoff;
	HighPassOutput<double> out_output;

	HighPass(Input<double> input, Input<double> cutoff, ModuleRegistry& registry = ModuleRegistry::globalRegistry);

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
	Input<double> in_input;
	Input<double> in_decayRate;
	Input<double> in_echoDuration;
	DelayOutput<double> out_output;

	Delay(Input<double> input, Input<double> duration, Input<double> decay, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
	~Delay();
};

MakeModule(BitCrusher)
public:
	Input<double> in_input;
	Input<double> in_resolution;
	BitCrusherOutput<double> out_output;

	BitCrusher(Input<double> input, Input<double> resolution, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};

MakeModule(BitCrusher2)
public:
	Input<double> in_input;
	Input<double> in_resolution;
	BitCrusher2Output<double> out_output;

	BitCrusher2(Input<double> input, Input<double> resolution, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
};