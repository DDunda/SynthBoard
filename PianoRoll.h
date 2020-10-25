#pragma once

#include <SDL.h>
#include <fstream>
#include <vector> 
#include <string>
#include "Sound.h"

template <class T>
class PR_Val : public Source<T>, public SetVal<float> {
private:
	void** ptr;
	int o;
public:
	PR_Val(void*& data, int offset) {
		ptr = &data;
		o = offset;
	}
	T Get() {
		T val = *(T*)((char*)*ptr + o);
		return val;
	}
	void Set(T input) {
		*(T*)((char*)*ptr + o) = input;
	}
	void reset() {};
};

struct PR_Sample {
	float freq;
	float vol;
};

class PianoRoll : public soundMaker {
private:
	Uint64 version;
	float time;
	bool running = false;
	std::ifstream input;
	std::streamoff fileStart;

	Uint16 channelCount;
	void* sampleData;
	int sampleBytes = 0;

	bool looping = false;
	bool fileOpen = false;

	template <class T>
	void ReadVar(T* val) {
		input.read((char*)val, sizeof(T));
	}
	template <class T>
	void ReadVar(T& val) {
		input.read((char*)&val, sizeof(T));
	}

	void loadSamples() {
		if (running) {
			char* resetBuffer = new char[(size_t)(channelCount >> 3) + ((channelCount & 8) ? 1uLL : 0uLL)];
			input.read((char*)sampleData, sampleBytes);

			float tmp1 = ((float*)sampleData)[0];
			float tmp2 = ((float*)sampleData)[1];
			if (input.eof()) {
				if (!looping) {
					running = false;
					return;
				}
				input.clear();
				input.seekg(fileStart, std::ios::beg);
				input.read((char*)sampleData, sampleBytes);
			}
		}
	}
	template<class T>
	PR_Val<T>* registerField() {
		PR_Val<float>* value = new PR_Val<float>(sampleData, sampleBytes);
		sampleBytes += sizeof(T);
		return value;
	}

public:
	blendAdd blender;
	float SamplesPerSecond;

	bool FileOpened() {
		return fileOpen;
	}

	bool CanReadFile() {
		return !input.eof();
	}

	void CloseFile() {
		if (fileOpen) {
			input.close();
			input.clear();
			blender.sources.clear();
			delete[] sampleData;
			sampleData = NULL;
			sampleBytes = 0;
		}
		fileOpen = false;
		running = false;
		time = 0;
	}

	void OpenFile(std::string file) {
		if (fileOpen)
			CloseFile();

		input.open(file, std::ios::binary);
		if (input.fail() || !input.is_open() || input.eof()) {
			input.close();
			input.clear();
			return;
		}
		input.seekg(0, std::ios::beg);

		ReadVar(version);

		Setup();

		sampleData = new Uint8[sampleBytes];
		fileStart = input.tellg();

		running = true;
		loadSamples();
		fileOpen = true;
		if (!CanReadFile())
			CloseFile();
	}

	void ResetFile() {
		input.clear();
		input.seekg(fileStart, std::ios::beg);
	}

	Source<float>* MakeFloatSource() {
		switch (version)
		{
			case 0:
			default:
				Uint32 type;
				ReadVar(type);
				switch (type)
				{
					// Constant
					case 0: {
						float src;
						ReadVar<float>(src);
						return new Val<float>(src);
					}

					// On piano roll
					case 1: return registerField<float>();

					// Wave
					case 2: {
						Uint32 type;
						ReadVar(type);
						switch (type) {
							case 0: return new sineSound(MakeFloatSource());
							case 1: return new squareSound(MakeFloatSource());
							case 2: return new noiseSound(MakeFloatSource());
							case 3: return new triangleSound(MakeFloatSource());
							case 4: return new sawtoothSound(MakeFloatSource());
							case 5: { // Modulation of floats. As well as the type, this is followed by the number of floats, as well as the actual floats
								Modulator* FM = new Modulator(MakeFloatSource());
								Uint32 vals;
								ReadVar(vals);
								for (Uint32 i = 0; i < vals; i++) ReadVar(FM->vals[i]);
								return new sineSound(FM);
							}
							default: return new sineSound(MakeFloatSource());
						}
					}
					// Filter
					case 3: {
						Uint32 type;
						ReadVar(type);
						switch (type)
						{
							// Volume filter
							case 0: {
								EasyPointer<Source<float>> src = MakeFloatSource();
								EasyPointer<Source<float>> vol = MakeFloatSource();

								return new volumeFilter(src, vol);
							}
							// Combined filter
							case 1:
								Uint32 combineType;
								ReadVar(combineType);
								blendFilter* combiner;
								switch (combineType)
								{
									case 1: combiner = new blendMult(); break;
									case 0: default: combiner = new blendAdd(); break;
								}
								Uint16 sourceCount;
								ReadVar(sourceCount);

								for (int i = 0; i < sourceCount; i++) {
									EasyPointer<Source<float>> src = MakeFloatSource();
									combiner->addSource(src);
								}
								return combiner;
						}
					}
				}
		}
		return new fVal(0.0f);
	}

	void Setup() {
		switch(version) {
			case 0:
			default:
				ReadVar(SamplesPerSecond);
				ReadVar(looping);
				ReadVar(channelCount);
				
				for (int i = 0; i < channelCount; ++i) {
					EasyPointer<Source<float>>* source = new EasyPointer<Source<float>>(MakeFloatSource());
					blender.sources.push_back(*source);
				}

				break;
		}
	}

	PianoRoll(std::string file) {
		OpenFile(file);
	}

	~PianoRoll() {
		CloseFile();
	}

	float getSound() {
		if (!running && fileOpen)
			return 0;

		time += SamplesPerSecond / (float)SOUND_FREQUENCY;
		if (time > 1) {
			time = fmod(time, 1.0f);
			if(fileOpen)
				loadSamples();
		}
		float t = blender.getSound();
		return t;
	};
	virtual void reset() {
		if (fileOpen) {
			ResetFile();
			loadSamples();
		}
	};
	void start() {
		running = true;
	}
};