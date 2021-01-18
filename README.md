# Synthboard
Synthboard is real-time digital synthesizer, controlled as a piano from your keyboard and implemented with the SDL-Sound pipeline.
SDL-Sound is an ongoing project exploring sound with SDL, and ways to play it at runtime.

![Synthboard](https://github.com/DDunda/PromotionalMaterials/blob/master/Synthboard.png)

## How to compile Synthboard
SDL-Sound, and Synthboard, are both written as standard C++ Visual Studio projects that can be compiled and ran.
SDL2, SDL2_image, and SDL2_mixer, are required for compilation; you can get these libraries from the following links,
and then simply copy them into the project directory to the folders called SDL2, and SDL2_image, and SDL2_mixer. Make sure to download the **development libraries** for both.
- SDL2: https://www.libsdl.org/download-2.0.php
- SDL2_image: https://www.libsdl.org/projects/SDL_image/
- SDL2_image: https://www.libsdl.org/projects/SDL_mixer/

## How to run Synthboard
Once you've compiled Synthboard you can find the program in the output folder. There will be a lot of files, but to run Synthboard just use the generated exe. If you wish to move the exe, the accompanying .dll files must be moved as well.

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
