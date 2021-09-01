# Synthboard
Synthboard is digital piano controlled by your keyboard, powered by a modular synthesizer pipeline.
The basis for this project comes from SDL-Sound, an older project of mine that evolved into Synthboard as I experimented with ways to manipulate my synths in realtime.

![Synthboard](https://github.com/DDunda/PromotionalMaterials/blob/master/Synthboard.png)

## How to compile Synthboard 
### Windows
Synthboard for windows uses a regular C++ Visual Studio project, as well as SDL2 and SDL2_image; for an explanation on how I link my SDL libraries, please see my guide [here](https://github.com/DDunda/InstallSDL2). Once you install both of these libraries you should be fine to press run within your project.

### Linux
Thanks to an addition by [@saur0x](https://github.com/saur0x), this project can compile for Linux with a makefile named 'Makefile'.

## How to run Synthboard
There are two options for running synthboard; either you can compile and run synthboard inside Visual Studio, or you can compile and run then run the exe. The exe will be in the output folder, but you can move it if you want; just remember to also move the dlls and images!

## How to use Synthboard
The controls are as follows:
- The piano keys are arranged across the keyboard, in a similar way to a real piano. The white keys are on the QWERTY and ZXCV rows, and the black keys are positioned just above those.
  - You can use your mouse, too.
- The **left and right arrows** transpose the piano up or down an octave.
- The **up and down arrows** cycle between the following synth types:
  - Sine wave
  - Triangle wave
  - Square wave
  - Triangle pulse
  - Square pulse
  - Sawtooth wave
  - Whitenoise
- The first slider controls volume.
- The second slider bitcrushes the volume levels of the sound.
- The third slider controls the decay of the echo effect.
- The fourth slider controls the delay of the echo effect.
- If you are using a pulse wave, a fith slider will appear that controls the duty cycle.

![Keyboard layout](https://github.com/DDunda/PromotionalMaterials/blob/master/Layout.png)
