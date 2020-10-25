# Synthboard - SDL-Sound
SDL-Sound is an ongoing project exploring sound with SDL, and ways to play it at runtime.
This particular branch explores sound creation with a piano hooked up to some synthesizers, and your keyboard.

![Synthboard](https://cdn.discordapp.com/attachments/364021339486421004/736567447129423912/Synthboard.png)

## How to compile Synthboard
SDL-Sound, and Synthboard, are both written as standard Visual Studio projects that can be compiled and ran.
However, the libraries for SDL2, and SDL2_image, are required for compilation. You can get SDL2 and SDL2_image from the following links,
and then simply copy them into the project directory to the folders called SDL2, and SDL2_image. Make sure to download the **development libraries** for both.
- SDL2: https://www.libsdl.org/download-2.0.php
- SDL2_image: https://www.libsdl.org/projects/SDL_image/

*P.S. Don't worry about SDL2_mixer, even though it's in the project it's not being used right now.*

## How to run Synthboard
Once you've compiled Synthboard you can find the program output folder. There will be a lot of files, but to run Synthboard you can use the generated exe.

## How to use Synthboard
The controls are quite simple.
- **Tab-\\** and **z-m** control **white keys**, and keys **1-backspace** and **s-j** control **black keys**. They are arranged the way a real piano would be.
- You can also play the piano with your mouse.
- The **left and right arrows** move the piano left and right, changing the octave.
- The **up and down arrows** change the synthesizer between the following:
  - Sine wave
  - Triangle wave
  - Square wave
  - Sawtooth wave
  - Whitenoise
- The box in the upper-left controls volume. Be warned that 0.01 is a quite moderate volume, so anything above 1 can be very loud and peak your speakers!

![Keyboard layout](https://cdn.discordapp.com/attachments/364021339486421004/736566105702793256/Keyboard_piano_4x.png)
