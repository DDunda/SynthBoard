#include "Sound.h"
#include "SoundConstants.h"

Sine::Sine(Input f) : in_frequency(f), out_output(0.0) {}
void Sine::CalculateState() {
	t += *in_frequency / SOUND_FREQUENCY;
	t = fmod(t, 1.0);
	out_output.backValue = sin(t * M_PI * 2.0);
}
void Sine::PresentState() {
	out_output.PresentValue();
}

Square::Square(Input f) : in_frequency(f), out_output(0.0) {}
void Square::CalculateState() {
	t += *in_frequency / SOUND_FREQUENCY;
	t = fmod(t, 1.0);
	out_output.backValue = t < 0.5 ? 1 : -1;
}
void Square::PresentState() {
	out_output.PresentValue();
}

Sawtooth::Sawtooth(Input f) : in_frequency(f), out_output(0.0) {}
void Sawtooth::CalculateState() {
	t += *in_frequency / SOUND_FREQUENCY;
	t = fmod(t, 1.0);
	out_output.backValue = (t * 2.0) - 1.0;
}
void Sawtooth::PresentState() {
	out_output.PresentValue();
}

Triangle::Triangle(Input f) : in_frequency(f), out_output(0.0) {}
void Triangle::CalculateState() {
	t += *in_frequency / SOUND_FREQUENCY;
	t = fmod(t, 1.0);

	//  /\
	// /  \
	//     \  /
	//      \/
	// 1  2  3
	out_output.backValue =
		t < 0.25 ? (       4.0 * t) : // 1 
		t < 0.75 ? ( 2.0 - 4.0 * t) : // 2
		           (-4.0 + 4.0 * t);  // 3

		// Alternatively:
		// return abs(2.0 - abs(1.0 - 4.0 * t)) - 1.0;
}
void Triangle::PresentState() {
	out_output.PresentValue();
}

SquarePulse::SquarePulse(Input f, Input d) : in_frequency(f), in_duty(d), out_output(0.0) {}
void SquarePulse::CalculateState() {
	t += *in_frequency / SOUND_FREQUENCY;
	t = fmod(t, 1.0);
	out_output.backValue = t < *in_duty ? 1.0 : -1.0;
}
void SquarePulse::PresentState() {
	out_output.PresentValue();
}

TrianglePulse::TrianglePulse(Input f, Input d) : in_frequency(f), in_duty(d), out_output(0.0) {}
void TrianglePulse::CalculateState() {
	t += *in_frequency / SOUND_FREQUENCY;
	t = fmod(t, 1.0);
	out_output.backValue = t < *in_duty ? 1.0 - 2.0 * t / *in_duty : -1.0 + 2.0 * (t - *in_duty) / (1.0 - *in_duty);
}
void TrianglePulse::PresentState() {
	out_output.PresentValue();
}

Noise::Noise(Input frequency) : in_frequency(frequency), out_output(0.0) {}
void Noise::SetRandomAmplitude() {
	amplitude = roundf(rand() / (float)RAND_MAX) * 2 - 1;
}
void Noise::CalculateState() {
	progress += *in_frequency * 4.0 / SOUND_FREQUENCY;
	if (progress > 1.0) {
		progress = fmodf(progress, 1.0);
		SetRandomAmplitude();
	}
	out_output.backValue = amplitude;
}
void Noise::PresentState() {
	out_output.PresentValue();
}

Constant::Constant(double v) : value(v), out_output(value) {}
void Constant::CalculateState() {
	out_output.backValue = value;
}
void Constant::PresentState() {
	out_output.PresentValue();
}

Multiplier::Multiplier(vector<Input> sources) : in_sources(sources), out_output(0.0) {};
void Multiplier::CalculateState() {
	double product = 1.0;
	for (auto source : in_sources) product *= *source;

	out_output.backValue = product;
}
void Multiplier::PresentState() {
	out_output.PresentValue();
}

Adder::Adder(vector<Input> sources) : in_sources(sources), out_output(0.0) {};
void Adder::CalculateState() {
	double sum = 0.0;
	for (auto source : in_sources) sum += *source;

	out_output.backValue = sum;
}
void Adder::PresentState() {
	out_output.PresentValue();
}

Mapper::Mapper(Input input, Input m, Input c) : in_input(input), in_m(m), in_c(c), out_output(0.0) {}
void Mapper::CalculateState() {
	out_output.backValue = *in_input * *in_m + *in_c;
}
void Mapper::PresentState() {
	out_output.PresentValue();
}

Flipper::Flipper(Input input) : in_input(input), out_output(0.0) {}
void Flipper::CalculateState() {
	out_output.backValue = -*in_input;
}
void Flipper::PresentState() {
	out_output.PresentValue();
}

Volume::Volume(Input input, Input targetVolume) : in_input(input), in_targetVolume(targetVolume), out_output(0.0) {}
void Volume::CalculateState() {
	internalVolume = (internalVolume * 9.0f + *in_targetVolume) / 10.0f;
	out_output.backValue = *in_input * internalVolume;
}
void Volume::PresentState() {
	out_output.PresentValue();
}

Limiter::Limiter(Input input, Input max) : in_input(input), in_max(max), out_output(0.0) {}
void Limiter::CalculateState() {
	double sample = *in_input;
	double max = abs(*in_max);
	if      (sample >  max) sample =  max;
	else if (sample < -max) sample = -max;
	out_output.backValue = sample;
}
void Limiter::PresentState() {
	out_output.PresentValue();
}

Fader::Fader(Input input, Input attack, Input release) : in_input(input), in_attackRate(attack), in_releaseRate(release), out_output(0.0) {}
void Fader::Stop() {
	if (state == true) {
		state = false;
		changing = true;
	}
}
void Fader::Start() {
	if (state == false) {
		state = true;
		changing = true;
	}
}
bool Fader::IsOn() {
	return state;
}
bool Fader::IsChanging() {
	return changing;
}
void Fader::CalculateState() {
	double volumeDelta = 0;
	if (changing) {
		if (state) { // Transitioning to playing sound
			volume += abs(*in_attackRate) / SOUND_FREQUENCY;
			if (volume > 1.0) {
				volume = 1.0;
				changing = false;
			}
		} else { // Transitioning to silence
			volume -= abs(*in_releaseRate) / SOUND_FREQUENCY;
			if (volume < 0.0) {
				volume = 0.0;
				changing = false;
			}
		}
	}
	out_output.backValue = *in_input * volume;
}
void Fader::PresentState() {
	out_output.PresentValue();
}

LowPass::LowPass(Input input, Input cutoff) : in_input(input), in_cutoff(cutoff), out_output(0.0) {
	SetCutoff(*cutoff);
}
void LowPass::SetCutoff(double cutoff) {
	RC = 1.0 / (cutoff * 2 * M_PI);
	dt = 1.0 / SOUND_FREQUENCY;
	alpha = dt / (RC + dt);
	cutoff_freq = *in_cutoff;
}
void LowPass::CalculateState() {
	if (cutoff_freq != *in_cutoff) SetCutoff(*in_cutoff);
	lastOutput = lastOutput + (alpha * (*in_input - lastOutput));
	out_output.backValue = lastOutput;
}
void LowPass::PresentState() {
	out_output.PresentValue();
}

HighPass::HighPass(Input input, Input cutoff) : in_input(input), in_cutoff(cutoff), out_output(0.0) {
	SetCutoff(*cutoff);
}
void HighPass::SetCutoff(double cutoff) {
	RC = 1.0 / (cutoff * 2 * M_PI);
	dt = 1.0 / SOUND_FREQUENCY;
	alpha = RC / (RC + dt);
}
void HighPass::CalculateState() {
	double rawOutput = *in_input;
	double output = alpha * (lastOutput + rawOutput - lastRawOutput);
	lastRawOutput = rawOutput;
	lastOutput = output;
	out_output.backValue = output;
}
void HighPass::PresentState() {
	out_output.PresentValue();
}

Delay::Delay(Input input, Input duration, Input decay) : in_input(input), in_echoDuration(duration), in_decayRate(decay), out_output(0.0) {}
Delay::~Delay() {
	if(bufferStart) delete[] bufferStart;
}
void Delay::SetLen(int l) {
	double* buffer = NULL;
	if (l != 0) {
		buffer = new double[l];
		for (int i = 0; i < l; i++) buffer[i] = 0.0f;
	}
	if (bufferStart != NULL) {
		if (l < len) {
			int tl = ceil(l / 2.0f);
			int rl = floor(l / 2.0f);

			double* h1 = bufferHead;
			double* h2 = buffer;

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

			double* h1 = bufferHead;
			double* h2 = buffer;

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
void Delay::CalculateState() {
	int l = floor(*in_echoDuration * SOUND_FREQUENCY);
	if (l != len) SetLen(l);
	double sound = *in_input;
	if (bufferHead) {
		*bufferHead = sound += *bufferHead * *in_decayRate;
		if (++bufferHead == bufferEnd) bufferHead = bufferStart;
	}
	out_output.backValue = sound;
}
void Delay::PresentState() {
	out_output.PresentValue();
}

BitCrusher::BitCrusher(Input input, Input resolution) : in_input(input), in_resolution(resolution), out_output(0.0) {}
void BitCrusher::CalculateState() {
	if (*in_resolution <= 0) out_output.backValue = *in_input;
	else out_output.backValue = round(*in_input / *in_resolution) * *in_resolution;
}
void BitCrusher::PresentState() {
	out_output.PresentValue();
}

BitCrusher2::BitCrusher2(Input input, Input resolution) : in_input(input), in_resolution(resolution), out_output(0.0) {}
void BitCrusher2::CalculateState() {
	if (*in_resolution <= 0) out_output.backValue = *in_input;
	else out_output.backValue = (round(*in_input / *in_resolution + 0.5) - 0.5) * *in_resolution;
}
void BitCrusher2::PresentState() {
	out_output.PresentValue();
}