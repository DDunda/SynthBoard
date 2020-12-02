#include "Sound.h"
#include <iostream>

void main() {
	Constant sineF(250.0 / 44100.0);
	Sine sine(&sineF.out_output);
	Constant attack(1000.0 / 44100.0);
	Constant release(200.0 / 44100.0);
	Fader limiter(&sine.out_output, &attack.out_output, &release.out_output);
	limiter.Start();
	for (int i = 0; i < 250; i++) {
		Module::CalculateAllModuleStates();
		Module::PresentAllModuleStates();
		std::cout << limiter.out_output << std::endl;
	}
	limiter.Stop();
	for (int i = 0; i < 250; i++) {
		Module::CalculateAllModuleStates();
		Module::PresentAllModuleStates();
		std::cout << limiter.out_output << std::endl;
	}
}