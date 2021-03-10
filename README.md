# Synthboard
Synthboard is real-time digital synthesizer, controlled as a piano from your keyboard and implemented with the SDL-Sound pipeline.
SDL-Sound is an ongoing project exploring sound with SDL, and ways to play it at runtime.

![Synthboard](https://github.com/DDunda/PromotionalMaterials/blob/master/Synthboard.png)

## How to compile Synthboard
Synthboard is written as standard C++ Visual Studio project that can be compiled and ran.
SDL2 and SDL2_image are required for compilation; please follow my guide for installing SDL libraries: https://github.com/DDunda/InstallSDL2

## How to run Synthboard
I recommend compiling and running the project from within Visual Studio, as it will start a debug environment that includes many necessary dlls. If you wish to use the actual exe though, you can find the program in the output folder. To run the exe you must copy the dlls from your SDL and SDL_image installations. If you wish to move the exe, the accompanying dlls must be moved as well.

## How to use Synthboard
The controls are quite simple.
- The piano keys are arranged on your keyboard, in the way a real piano would be.
  - You can also play the piano with your mouse.
- The **left and right arrows** move the piano left and right, changing the octave.
- The **up and down arrows** change the synthesizer between the following:
  - Sine wave
  - Triangle wave
  - Square wave
  - Triangle pulse
  - Square pulse
  - Sawtooth wave
  - Whitenoise
- The first slider controls volume.
- The second slider lowers the resolution of the sound, an optional effect.
- The third slider controls the decay of the echo effect.
- The fourth slider controls the delay of the echo effect.
- The fith slider controls the duty cycle of pulse waves.

![Keyboard layout](https://github.com/DDunda/PromotionalMaterials/blob/master/Layout.png)
