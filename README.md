# HayB0XX

HayB0XX is a modular custom firmware for DIY [B0XX](https://b0xx.com)
controllers. It was originally based on Crane's
[DIYB0XX/GCCPCB code](https://github.com/Crane1195/GCCPCB/tree/master/code),
but I ended up doing a complete rewrite and made things much more maintainable
and extensible.

Features include:
- Supports all the usual DIY types with just one code release, only requiring a
  one line change
- Up to date with B0XX V3 specifications
- Supports DInput, GameCube, and Nintendo 64
- Easy to create new controller modes (or keyboard modes) for different games
- Fully customisable SOCD, allowing you to configure SOCD button pairs (e.g.
  left/right, up/down) for each controller/keyboard mode, and also easily change
  the SOCD resolution method
- Switch modes on the fly without reconnecting your controller
- Controller modes and communication backends (e.g. DInput, GameCube, N64) are
  separate, independent entities, meaning you can use all of your controller
  modes with all supported consoles without any extra work
- Very easily switch between different GameCube/N64 polling rates in order to
  have optimal latency on console, overclocked adapter, or anything else
- Nunchuk support upcoming

[![GitHub issues](https://img.shields.io/github/issues/JonnyHaystack/HayB0XX)](https://github.com/JonnyHaystack/HayB0XX/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/JonnyHaystack/HayB0XX)](https://github.com/JonnyHaystack/HayB0XX/pulls)

## Table of Contents

* [Getting Started](#getting-started)
  * [Requirements](#requirements)
  * [Installation](#installation)
* [Usage](#usage)
  * [Default button holds](#default-button-holds)
  * [Dolphin setup](#dolphin-setup)
* [Configuration](#configuration)
  * [Console/gamemode selection bindings](#consolegamemode-selection-bindings)
  * [Creating custom input modes](#creating-custom-input-modes)
  * [Mod X lightshield and R shield tilt](#mod-x-lightshield-and-r-shield-tilt)
  * [Project M/Project+ mode](#project-mproject+-mode)
* [Troubleshooting](#troubleshooting)
* [Contributing](#contributing)
* [Contributors](#contributors)
* [License](#license)

## Getting Started

### Requirements

- Arduino IDE
- Ideally a decent editor like VSCode if you're going to be changing any code

### Installation

Grab the [latest HayB0XX release](https://github.com/JonnyHaystack/HayB0XX/releases),
or clone the repository with `git clone --recursive` if you have git installed
(which makes it easier for you to pull updates).

After that:
1. Open the HayB0XX.ino sketch in the Arduino IDE
2. Change the `#include "setup_xxx.h"` line at the top to use the appropriate
   pinout/setup file for the type of DIY that you are using:
  - `setup_gccpcb1.h` for GCCPCB1
  - `setup_gccpcb2.h` for GCCPCB2
  - `setup_gccmx.h` for GCCMX
  - `setup_smashbox.h` for Smash Box
  - `setup_arduino_nativeusb.h` for Arduino with native USB support (Leonardo/Micro)
    - Edit `pinout_arduino.h` to match your wiring and the buttons that you have
  - `setup_arduino.h` for Arduino without native USB support (e.g. Nano or other)
    - Edit `pinout_arduino.h` to match your wiring and the buttons that you have
    - You will also have to delete the files `DInputBackend.h`,
      `DInputBackend.cpp`, `KeyboardMode.h`, `KeyboardMode.cpp`,
      `DefaultKeyboardMode.h`, and `DefaultKeyboardMode.cpp` otherwise the
      Arduino IDE will complain at you
3. For any buttons that your controller doesn't have (e.g. Select and Home),
   remove those lines from the pinout file and remove any references to them
   from `readInputs()` and `setup()` in `HayB0XX.ino`
4. Click Tools > Board and select your board type. For GCCPCB/GCCMX it should be
   Arduino Leonardo.
5. Click Upload

## Usage

### Default button holds

#### Brook board passthrough mode

To switch to Brook board mode on GCCPCB2 or GCCMX, hold Mod X + A on plugin.

#### Communication backends (console selection)

For all DIYs that support native USB it is configured to use the DInput backend
as the default. To select another backend on plugin, the following button holds
are available:
- C-Down - GameCube backend
- C-Left - Nintendo 64 backend

#### Game mode selection

Unlike other B0XX/DIYB0XX firmwares, HayB0XX by default allows you to switch
modes on the fly without unplugging your controller. This is mainly useful on
PC, as opposed to console where you usually have to restart the console to
switch game anyway. It also serves the purpose of reducing the number of buttons
you have to hold with one hand while plugging in.

The default controller mode button combinations are:
- Mod X + Start + L - Melee mode (default)
- Mod X + Start + Left - Project M/Project+ mode
- Mod X + Start + Down - FGC mode (Hitbox style fighting game layout)

Default keyboard mode button combinations:
- Mod Y + Start + L - Default keyboard mode

### Dolphin setup

HayB0XX needs a different controller profile than the normal B0XX one, as it
uses different outputs in an effort to be more compatible with different (mostly
older) games. To install the profile, copy the HayB0XX.ini file to the folder
`<YourDolphinInstallation>\User\Config\Profiles\GCPad`, then load it as usual in
the Dolphin controller config.

## Configuration

### Console/gamemode selection bindings

#### Communication backends (console selection)

The communication backend (DInput, GameCube, or N64) is selected based on the
buttons held on plugin. This is handled in your `setup_xxx.h` file, in the
`initialise()` function. The logic here is very simple, and even if you have no
programming experience you should be able to see what's going on and change
things if you wish.

You may notice that the number 125 is passed into `GamecubeBackend()`. This lets
you change the polling rate e.g. if your DIY doesn't support native input and
you want to use it with an overclocked GameCube controller adapter. In that
example, you would pass in 1000 to sync up to the 1000Hz polling rate.

If you want to be able to play both on console and on an overclocked adapter,
you can add another `if` statement to create a button hold to set polling rate
to 1000Hz.

Note: You may notice that 1000Hz polling rate works on console as well. Be aware
that while this works, it will result in more input lag. The point of setting
the polling rate here is so that the GameCube backend can delay until right
before the next poll before reading the inputs, so that the inputs are fresh and
not outdated.

Also note: Polling rate can be passed into the N64Backend constructor in the
same way as this

#### Input modes

To configure the button holds for input modes (controller/keyboard modes), go
into `HayB0XX.ino`, in the `selectInputMode()` function. Each `if` statement is
a button combination to select an input mode.

All input modes support passing in a SOCD handling mode, e.g.
`socd::2IP_NO_REAC`. You can see the other available modes in `socd.h`.

Every ControllerMode must have gInputState and gCurrentBackend passed into it,
as can be seen in the existing code here.

Keyboard modes only need to be passed a SOCD mode and gInputState.

### Creating custom input modes

For creating new input modes, it helps if you know some C++, or at least have
some programming experience. That said, you should be able to get by even
without prior experience if you just base your new mode off the existing ones
and refer to them as examples.

There are two types of input modes: ControllerMode and KeyboardMode

#### Keyboard modes

Keyboard modes are a little bit simpler so let's start there.

A KeyboardMode behaves as a standard keyboard and should work with any device
that supports keyboards.

You are free to use whatever logic and programming tricks you like in the
`SendKeys()` method to decide the outputs based on the input state. You could
create input layers (like the D-Pad layer in Melee mode that is activated when
holding Mod X and Mod Y), or other types of conditional inputs.

#### Controller modes

A ControllerMode takes the B0XX's button input state and transforms it into an
output state corresponding to a standard gamepad. Any ControllerMode will work
with any CommunicationBackend. A CommunicationBackend simply handles the sending
of the applicable outputs in the output state to the console or PC.

To create a ControllerMode, you just need to implement the methods
`UpdateDigitalOutputs()` and `UpdateAnalogOutputs()`.

`UpdateDigitalOutputs()` is very similar to the `SendKeys()` method in keyboard
modes, with the difference that rather than calling a `Press()` method to
immediately send inputs, we are simply setting the output state for this
iteration. As the name indicates, we will only deal with the digital outputs in
this method.

`UpdateAnalogOutputs()` is a bit more complicated. Firstly, it has to call
`HandleVectors()` before doing anything else. This method takes in values
indicating whether your left and right sticks are pointing left/right/up/down.
You also pass in the minimum, neutral (centre), and maximum stick analog values,
so you can configure these on a per-mode basis. All this information is used to
automatically set the stick analog values based on the inputs you passed in. This
is all you need to do unless you need to implement modifiers.

`HandleVectors()` also populates the variable mVectorState with values
indicating current stick direction, which can be 1, 0, or -1 for the X and Y
axes for both sticks. This so-called "vector state" makes it much easier to
write modifier handling logic if necessary.

After calling `HandleVectors()`, add any modifier handling logic that you want.
Remember that `HandleVectors()` already set up the analog stick values for us,
so when handling modifiers we only need to manually set the values for the axes
that are actually being modified. Other than this, I can't teach how to write
modifier logic, so just look at the examples, play around, and use your
imagination.

Finally, set any analog trigger values that you need.

Note: Analog trigger outputs could just as well be handled in
`UpdateDigitalOutputs()`, but I think it usually looks cleaner to keep them
along with the other analog stuff.

Also note: You don't need to worry about resetting the output state or clearing
anything from it. This is done automatically at the start of each iteration.

#### SOCD

In the constructor of each mode (for controller modes *and* keyboard modes), you
can configure pairs of opposing directions to apply SOCD handling to.

For example, in `Melee20Button.cpp`:
```
mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.up});
mSocdPairs.push_back(socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});
```

This sets up left/right, down/up, C-Left/C-Right, and C-Down/C-Up as pairs of
opposing cardinal directions for which SOCD cleaning will be applied. The SOCD
cleaning is automatically done before `UpdateDigitalOutputs()` and
`UpdateAnalogOutputs()`, and you do not need to worry about it any further than
that.

Note that you do not have to write a `HandleSocd()` method like in Melee20Button
and Melee18Button. It is only overridden in these two modes so that we can check
if left and right are both held *before* SOCD cleaning, because when they are
both held (without a vertical direction being held) we need to override all
modifiers.

### Mod X lightshield and R shield tilt

If your DIY B0XX has no lightshield buttons, you may want to use Mod X for
lightshield and put shield tilt on R instead. You can do this by using the
Melee18Button mode instead of Melee20Button.

### Project M/Project+ mode

The ProjectM mode has some extra options to configure certain behaviours. See
the constructor signature below for reference.

```
ProjectM(socd::SocdType socdType, state::InputState &rInputState,
           CommunicationBackend *communicationBackend,
           bool ledgedashMaxJumpTraj, bool trueZPress);
```

These options are configured by setting the relevant constructor parameter to
true or false in `HayB0XX.ino`.

Firstly, it allows you to enable or disable the behaviour borrowed from Melee
mode where holding left and right (and no vertical directions) will give a 1.0 
cardinal regardless of modifiers being held. This is controlled using the
`ledgedashMaxJumpTraj` parameter.

If you change the SOCD mode to 2IP (with reactivation), you should also change
this option to false if you want a smooth gameplay experience.

Secondly, Project M/Project+ do not handle Z presses the same way as Melee does.
Melee interprets a Z press as lightshield + A, and thus it can be used for L
cancelling without locking you out of techs. In PM/P+, a Z press will trigger a
tech and thus cause unwanted tech lockouts if used to L cancel.
By default in HayB0XX, the ProjectM mode is set to use a macro of lightshield + A
in order to preserve expected behaviour from Melee. However, this macro does not
enable you to use tether/grapple attacks or grab items. To workaround this, you
can press Mod X + Z to send a true Z input.

If this bothers you, and you just want to send a true Z input by default when
pressing Z, you can set the `trueZPress` parameter to true.

## Troubleshooting



## Contributing

I welcome contributions and if you make an input mode that you want to share just
let me know and it can be put it in the contrib folder.

### Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available,
see the [tags on this repository](https://github.com/JonnyHaystack/HayB0XX/tags).

## Built With

* [Arduino_Vector](https://github.com/zacsketches/Arduino_Vector) - Vector library used for SOCD pairs and state
* [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary) - Used for the DInput communication backend
* [Arduino Nintendo Library](https://github.com/NicoHood/Nintendo) - Used for the GameCube and Nintendo 64 communication backends
* [Keyboard Library for Arduino](https://github.com/arduino-libraries/Keyboard) - Used for keyboard input modes

## Contributors

* **Jonathan Haylett** - *Creator* - [@JonnyHaystack](https://github.com/JonnyHaystack)
* **Crane** - *Creator of fragmented remnants of GCCPCB code that still remain* - [@Crane1195](https://github.com/Crane1195)

See also the list of [contributors](https://github.com/JonnyHaystack/HayB0XX/contributors) who participated in this project.

### Acknowledgments

* The B0XX team, for designing and creating an incredible controller
* [@Crane1195](https://github.com/Crane1195) - for his DIYB0XX and GCCPCB projects, and for hours of answering my constant questions
* [@zacsketches](https://github.com/zacsketches) - for the Arduino_Vector library
* [@MHeironimus](https://github.com/MHeironimus) - for the Arduino Joystick library
* [@NicoHood](https://github.com/NicoHood) - for the Nintendo library
* The Arduino project - for all their open-source hardware and software

## Related projects

For those interested, in creating a DIY B0XX controller, check out Crane's
GitHub repositories and Discord:
- [DIYB0XX](https://github.com/Crane1195/DIYB0XX) - Crane's DIY B0XX build logs and code
- [GCCPCB](https://github.com/Crane1195/GCCPCB) - Crane's GCCPCB project and code
- [Crane's Lab on Discord](https://discord.gg/S3qgZWD)

## License

This project is licensed under the GNU GPL Version 3 - see the [LICENSE](LICENSE) file for details
