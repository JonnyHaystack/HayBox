# HayBox

HayBox is a modular, cross-platform firmware for digital or mixed analog/digital controllers, primarily targeted at [B0XX](https://b0xx.com)-style
controllers. Supported microcontrollers are Raspberry Pi Pico/RP2040 and 16MHz AVR microcontrollers. It was originally
based on Crane's
[DIYB0XX/GCCPCB code](https://github.com/Crane1195/GCCPCB/tree/master/code) for Arduinos,
but I ended up doing a complete rewrite and made things much more maintainable
and extensible.

Features include:
- Cross platform support:
  - RP2040 (e.g. Raspberry Pi Pico)
  - 16MHz AVR MCUs (e.g. ATMega32U4 which several Arduinos are based on)
- Supports many existing controllers/PCBs, e.g. B0XX, Smash Box, Crane's
  GCCPCB/Model S
- Melee mode up to date with B0XX V3 specifications
- Supports a variety of communication backends which can be used either separately or in conjunction with each other:
  - DInput
  - GameCube console
  - Nintendo 64 console
  - B0XX input viewer
- Supports a variety of "input sources" which can be used in conjunction to create mixed input controllers:
  - Buttons/switches wired directly to GPIO pins
  - Switch matrix (as typically found in keyboards)
  - Wii Nunchuk
  - GameCube controller
- Existing modes for popular games (e.g. Melee, Project M, Ultimate, Rivals of Aether, traditional fighting games)
- Easy to create new controller modes (or keyboard modes) for different games
- USB keyboard game modes for games that lack gamepad support
- Fully customisable SOCD cleaning, allowing you to configure SOCD button pairs (e.g. left/right, up/down) for each controller/keyboard mode, and also easily change the SOCD resolution method
- Switch modes on the fly without unplugging your controller
- Automatically detects whether plugged into console or USB
- Game modes and communication backends are independent entities, meaning you can use any game mode with any supported console without extra work
- Easily switch between different GameCube/N64 polling rates in order to have optimal latency on console, overclocked adapter, etc. Not necessary for Pico/RP2040.

[![GitHub issues](https://img.shields.io/github/issues/JonnyHaystack/HayBox)](https://github.com/JonnyHaystack/HayBox/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/JonnyHaystack/HayBox)](https://github.com/JonnyHaystack/HayBox/pulls)

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
  * [Project M/Project+ mode](#project-mproject-mode)
  * [Input sources](#input-sources)
  * [Using the Pico's second core](#using-the-picos-second-core)
* [Troubleshooting](#troubleshooting)
* [Contributing](#contributing)
* [Contributors](#contributors)
* [License](#license)

## Getting Started

### Requirements

- [PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode)

### Installation

Download and extract the
[latest HayBox release](https://github.com/JonnyHaystack/HayBox/releases),
or clone the repository if you have git installed (which makes it easier for you
to pull updates).

After that:
1. Open Visual Studio Code
2. Click File -> **Open Folder** and choose the HayBox folder (the one containing platformio.ini, not the folder above that)
3. Choose the appropriate build environment for your controller's PCB by
  clicking the environment selection button near the bottom left of the window
  
  ![Screenshot 2021-10-19 003017](https://user-images.githubusercontent.com/1266473/137824617-4a282217-c6cc-48fb-a55c-1ca40d460538.png)
  ![Screenshot 2021-10-19 003055](https://user-images.githubusercontent.com/1266473/137824641-4a21c8df-abe1-41fe-a15e-a6e9f5a95467.png)
  
4. If your controller has a different pinout than any of the existing configs, you may edit the button mappings and other pins at the top of the config (`config/<environment>/config.cpp`). Any buttons that your controller doesn't have can simply be deleted from the list.
5. Click **Build** (in the bottom left) to make sure everything compiles without
  errors
6. **For Pico-based controllers only**, the very first time you install this firmware you will have to hold the bootsel button while plugging it in and then drag and drop the file `HayBox/.pio/build/<environment>/firmware.uf2` onto the RPI-RP2 drive that comes up. For subsequent installs, only the following step is necessary.
7. Plug in your controller via USB and click **Upload** (next to the Build button)

## Usage

### Default button holds

#### Pico bootsel mode

To reboot Pico-based controllers into bootsel mode, hold Start on plugin.

#### Brook board passthrough mode

To switch to Brook board mode on GCCPCB2, GCCMX, or B0XX R2, hold B on
plugin.

#### Communication backends (console selection)

Communication backends are selected slightly differently depending on the type
of microcontroller used in the controller.

On Pico/RP2040, there are currently no button holds required on plugin. USB vs
GameCube vs Nintendo 64* is detected automatically. 

On Arduino/AVR, the DInput backend is selected if a USB connection is detected.
Otherwise, it defaults to GameCube backend, unless another backend is manually
selected by holding one of the following buttons on plugin:
- A - GameCube backend with polling rate fix disabled (used for GCC adapters)
- C-Left - Nintendo 64 backend (60Hz polling rate)

\* Nintendo 64 is not supported on Pico yet, but will be very soon.

#### Game mode selection

Unlike other similar firmwares, HayBox by default allows you to switch
modes on the fly without unplugging your controller. This is mainly useful on
PC, as opposed to console where you usually have to restart the console to
switch game anyway. It also serves the purpose of reducing the number of buttons
you have to hold with one hand while plugging in.

The default controller mode button combinations are:
- Mod X + Start + L - Melee mode (default)
- Mod X + Start + Left - Project M/Project+ mode
- Mod X + Start + Down - Ultimate mode
- Mod X + Start + Right - FGC mode (Hitbox style fighting game layout)
- Mod X + Start + B - Rivals of Aether mode

Default keyboard mode button combinations:
- Mod Y + Start + L - Default keyboard mode

### Dolphin setup

HayBox needs a different Dolphin controller profile than the official B0XX firmware, as it
uses different DInput mappings that make more sense for use across multiple games. To install
the profile:
1. Copy the .ini file corresponding to your operating system from the `profiles/` to the folder `<YourDolphinInstallation>\User\Config\Profiles\GCPad` (create it if it does not exist)
2. Plug in your controller, and configure a "Standard Controller" in Dolphin
3. Click **Refresh** next to the Device dropdown
4. Select the HayBox profile from the profile dropdown, and click **Load** (NOT Save)
5. Make sure the correct device is selected in the device dropdown
6. Click **Close**

## Configuration

### Console/gamemode selection bindings

#### Communication backends (console selection)

The communication backend (e.g. DInput, GameCube, or N64) is selected partly
through auto detection and partly based on the buttons held on plugin. This is
handled in `config/<environment>/config.cpp`, in the `setup()` function.
The logic is fairly simple, and even if you don't have programming experience it
shouldn't be too hard to see what's going on and change things if you wish.

The config folders corresponding to the Arduino environments are:
- `config/arduino_nativeusb/` for Arduino with native USB support (e.g. Leonardo, Micro)
- `config/arduino/` for Arduino without native USB support (e.g. Uno, Nano, Mega 2560)

For Arduino device configs you may notice that the number 125 is passed into
`GamecubeBackend()`. This lets you change the polling rate e.g. if your DIY
doesn't support native USB and you want to use it with an overclocked GameCube
controller adapter. In that example, you could pass in 1000 to sync up to the
1000Hz polling rate, or 0 to disable this lag fix completely.
Polling rate can be passed into the N64Backend constructor in the same way as this.

You may notice that 1000Hz polling rate works on console as well. Be aware
that while this works, it will result in more input lag. The point of setting
the polling rate here is so that the GameCube backend can delay until right
before the next poll before reading the inputs, so that the inputs are fresh and
not outdated.

For Pico/RP2040, it is not necessary to pass in a console polling rate, because
the Pico has enough processing power to read/process inputs after receiving the
console poll, so there is no need to predict when the poll will arrive and
prepare things in advance.

#### Input modes

To configure the button holds for input modes (controller/keyboard modes), edit
the `select_mode()` function in `config/mode_selection.hpp`. Each `if`
statement is a button combination to select an input mode.

All input modes support passing in a SOCD cleaning mode, e.g.
`socd::2IP_NO_REAC`. You can see the other available modes in
`src/include/socd.hpp`.

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
`UpdateKeys()` function to decide the outputs based on the input state. You could
create input layers (like the D-Pad layer in Melee mode that is activated when
holding Mod X and Mod Y), or other types of conditional inputs.

The list of available keycodes can be found [here](https://github.com/JonnyHaystack/ArduinoKeyboard/blob/master/include/keycodes.h).

#### Controller modes

A ControllerMode takes a digital button input state and transforms it into an
output state corresponding to a standard gamepad. Any ControllerMode will work
with any CommunicationBackend. A CommunicationBackend simply reads inputs from one or more input sources, uses the current ControllerMode to update the outputs based on those inputs, and handles the sending of the outputs to the console or PC.

To create a ControllerMode, you just need to implement the functions
`UpdateDigitalOutputs()` and `UpdateAnalogOutputs()`.

`UpdateDigitalOutputs()` is very similar to the `UpdateKeys()` function in keyboard
modes, with the difference that rather than calling a `Press()` function to
immediately send inputs, we are simply setting the output state for this
iteration. As the name indicates, we will only deal with the digital outputs in
this function.

`UpdateAnalogOutputs()` is a bit more complicated. Firstly, it has to call
`UpdateDirections()` before doing anything else. This function takes in values
indicating whether your left and right sticks are pointing left/right/up/down.
You also pass in the minimum, neutral (centre), and maximum stick analog values,
so you can configure these on a per-mode basis. All this information is used to
automatically set the stick analog values based on the inputs you passed in. This
is all you need to do unless you want to implement modifiers.

`UpdateDirections()` also populates the variable `directions` with values
indicating current stick direction, which can be 1, 0, or -1 for the X and Y
axes for both sticks. These values make it much easier to write modifier logic.

After calling `UpdateDirections()`, add any modifier handling logic that you want.
Remember that `UpdateDirections()` already set the default analog stick values,
so when handling modifiers you only need to manually set the values for the axes
that are actually being modified. Other than this, I can't teach how to write
your modifier logic, so just look at the examples and play around.

Finally, set any analog trigger values that you need.

Note: Analog trigger outputs could just as well be handled in
`UpdateDigitalOutputs()`, but I think it usually looks cleaner to keep them
along with the other analog outputs.

Also note: You don't need to worry about resetting the output state or clearing
anything from it. This is done automatically at the start of each iteration.

#### SOCD

In the constructor of each mode (for controller modes *and* keyboard modes), you
can configure pairs of opposing direction inputs to apply SOCD cleaning to.

For example, in `src/modes/Melee20Button.cpp`:
```
_socd_pair_count = 4;
_socd_pairs = new socd::SocdPair[_socd_pair_count]{
    socd::SocdPair{&InputState::left,    &InputState::right  },
    socd::SocdPair{ &InputState::down,   &InputState::up     },
    socd::SocdPair{ &InputState::c_left, &InputState::c_right},
    socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
};
```

This sets up left/right, down/up, C-Left/C-Right, and C-Down/C-Up as pairs of
opposing cardinal directions for which SOCD cleaning will be applied. The SOCD
cleaning is automatically done before `UpdateDigitalOutputs()` and
`UpdateAnalogOutputs()`, and you do not need to worry about it any further than
that.

Note that you do not have to write a `HandleSocd()` function like in the
Melee20Button and Melee18Button modes. It is only overridden in these two modes
so that we can check if left and right are both held *before* SOCD cleaning,
because when they are both held (without a vertical direction being held) we
need to override all modifiers.

### Mod X lightshield and R shield tilt

If your controller has no lightshield buttons, you may want to use Mod X for
lightshield and put shield tilt on R instead. You can do this by using the
Melee18Button mode instead of Melee20Button.

### Project M/Project+ mode

The ProjectM mode has some extra options to configure certain behaviours. See
the constructor signature below for reference.

```
ProjectM(socd::SocdType socd_type, bool ledgedash_max_jump_traj, bool true_z_press);
```

These options are configured by setting the relevant constructor parameter to
`true` or `false` in `config/mode_selection.hpp`.

Firstly, this allows you to enable or disable the behaviour borrowed from Melee
mode where holding left and right (and no vertical directions) will give a 1.0 
cardinal regardless of modifiers being held. This is controlled using the
`ledgedash_max_jump_traj` parameter.

If you change the SOCD mode to 2IP (with reactivation), you should also change
this option to false if you want a smooth gameplay experience.

Secondly, Project M/Project+ do not handle Z presses the same way as Melee does.
Melee interprets a Z press as lightshield + A, and thus it can be used for L
cancelling without locking you out of techs. In PM/P+, a Z press will trigger a
tech and thus cause unwanted tech lockouts if used to L cancel.
By default in HayBox, the ProjectM mode is set to use a macro of lightshield + A
in order to preserve expected behaviour from Melee. However, this macro does not
enable you to use tether/grapple attacks or grab items. To workaround this, you
can press Mod X + Z to send a true Z input.

If this bothers you, and you just want to send a true Z input by default when
pressing Z, you can set the `true_z_press` parameter to true.

### Input sources

HayBox supports several input sources that can be read from to update the input
state:
- `GpioButtonInput` - The most commonly used, for reading switches/buttons connected directly to GPIO pins. The input mappings are defined by an array of `GpioButtonMapping` as can be seen in almost all existing configs.
- `SwitchMatrixInput` - Similar to the above, but scans a keyboard style switch matrix instead of individual switches. A config for Crane's Model C<=53 is included at `config/c53/config.cpp` which serves as an example for how to for how to define and use a switch matrix input source.
- `NunchukInput` - Reads inputs from a Wii Nunchuk using i2c. This can be used for mixed input controllers (e.g. left hand uses a Nunchuk for movement, and right hand uses buttons for other controls)
- `GamecubeControllerInput` - Similar to the above, but reads from a GameCube controller. Can be instantiated similarly to GamecubeBackend. Currently only implemented for Pico, and you must either run it on a different pio instance (pio0 or pio1) than any instances of GamecubeBackend, or make sure that both use the same PIO instruction memory offset.

Each input source has a "scan speed" value which indicates roughly how long it takes for it to read inputs. Fast input sources are always read at the last possible moment (at least on Pico), resulting in very low latency. Conversely, slow input sources are typically read quite long before they are needed, as they are too slow to be read in response to poll. Because of this, it is more ideal to be constantly reading those inputs on a separate core. This is not possible on AVR MCUs as they are all single core, but it is possible (and easy) on the Pico/RP2040. The bottom of the default Pico config `config/pico/config.cpp` illustrates this by using core1 to read Nunchuk inputs while core0 handles everything else. See [the next section](#using-the-picos-second-core) for more information about using core1.


In each config's `setup()` function, we build up an array of input sources, and then pass it into a communication backend. The communication backend decides when to read which input sources, because inputs need to be read at different points in time for different backends. We also build an array of communication backends, allowing more than one backend to be used at once. For example, in most configs, the B0XX input viewer backend is used as a secondary backend whenever the DInput backend is used. In each iteration, the main loop tells each of the backends to send their respective reports. In future, there could be more backends for things like writing information to an OLED display.

### Using the Pico's second core

In each config, there are the functions `setup()` and `loop()`, where `setup()` runs first, and then `loop()` runs repeatedly until the device is powered off.

On Pico/RP2040, the `setup()` and `loop()` functions execute on core0, and you can add the functions `setup1()` and `loop1()` in order to run tasks on core1.

For example, to read GameCube controller inputs on core1:
```
GamecubeControllerInput *gcc = nullptr;

void setup1() {
    while (backends == nullptr) {
        tight_loop_contents();
    }

    gcc = new GamecubeControllerInput(gcc_pin, 2500, pio1);
}

void loop1() {
    if (backends != nullptr) {
        gcc->UpdateInputs(backends[0]->GetInputs());
    }
}
```

The `while` loop makes sure we wait until `setup()` on core0 has finished setting up the communication backends. We then create a GameCube controller input source with a polling rate of 2500Hz. We also run it on `pio1` as an easy way to avoid interfering with any GameCube/N64 backends, which use `pio0` unless otherwise specified. In `loop1()` we make the assumption that the primary backend is the first element of the `backends` array (which is configured in the same file anyway, so we aren't truly assuming anything we don't know) and directly scan the GameCube controller inputs into the backend's input state.

As a slightly crazier hypothetical example, one could even power all the controls for a two person arcade cabinet using a single Pico by creating two switch matrix input sources using say 10 pins each, and two GameCube backends, both on separate cores. The possibilities are endless.

## Troubleshooting

### Controller not working with console or GameCube adapter

If you are using an official adapter with an Arduino-based controller you will likely have to hold A on plugin which disables the polling latency optimisation by passing in a polling rate of 0 to the GamecubeBackend constructor.

If you are using an Arduino-based controller without a boost circuit, you will need 5V power so for Mayflash adapter you need both USB cables plugged in, and on console the rumble line needs to be intact. Pico works natively with 3.3V power so this isn't an issue.

## Contributing

I welcome contributions and if you make an input mode that you want to share,
feel free to make a pull request. Please install the clang-format plugin for
VS Code and use it to format any code you want added.

### Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available,
see the [tags on this repository](https://github.com/JonnyHaystack/HayBox/tags).

## Built With

* [https://github.com/MHeironimus/ArduinoJoystickLibrary]() - Used for DInput support for AVR
* [https://github.com/NicoHood/Nintendo]() - Used for GameCube and Nintendo 64 support for AVR
* [https://github.com/JonnyHaystack/ArduinoKeyboard]() - Used for keyboard modes on AVR
* [https://github.com/JonnyHaystack/arduino-nunchuk]() - Used for Nunchuk support
* [https://github.com/JonnyHaystack/joybus-pio]() - Used for GameCube and Nintendo 64 support for RP2040
* [https://github.com/earlephilhower/arduino-pico]() - Used for Arduino framework/PlatformIO support for Pico

## Contributors

* **Jonathan Haylett** - *Creator* - [@JonnyHaystack](https://github.com/JonnyHaystack)
* **Crane** - *Creator of the original DIYB0XX firmware* - [@Crane1195](https://github.com/Crane1195)

See also the list of [contributors](https://github.com/JonnyHaystack/HayBox/contributors) who participated in this project.

### Acknowledgments

* The B0XX team, for designing and creating an incredible controller
* [@Crane1195](https://github.com/Crane1195) - for his DIYB0XX and GCCPCB projects, and for hours of answering my questions when I was first writing this
* [@MHeironimus](https://github.com/MHeironimus) - for the Arduino Joystick library
* [@NicoHood](https://github.com/NicoHood) - for the Nintendo library
* [@GabrielBianconi](https://github.com/GabrielBianconi) - for the Arduino Nunchuk library
* [@earlephilhower](https://github.com/earlephilhower) - for the arduino-pico core
* [@maxgerhardt](https://github.com/maxgerhardt) - for adding PlatformIO support for arduino-pico
* The Arduino project and the Raspberry Pi Foundation - for all their open-source hardware and software

## License

This project is licensed under the GNU GPL Version 3 - see the [LICENSE](LICENSE) file for details
