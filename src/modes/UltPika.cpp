/* Ultimate profile by Nathan "Sleepy" Koenigsmark for expermenting with weird pika ideas*/
#include "modes/UltPika.hpp"
#include <stdlib.h>
#include <time.h>

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

UltPika::UltPika(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    srand(time(NULL));
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::a      },
    };
}

void UltPika::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b || (inputs.lightshield && inputs.y);
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    outputs.buttonR = inputs.z;
    outputs.start = inputs.start;
    outputs.dpadUp = inputs.nunchuk_c;

    // Turn on D-Pad layer by holding Mod X + Mod Y.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.dpadUp = inputs.c_down;
        outputs.dpadDown = inputs.c_up;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }
}

void UltPika::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // set a tilt modifer
    auto set_analog_stick = [&](int x_percent, int y_percent) {
        outputs.leftStickX = 128 + directions.x * x_percent;
        outputs.leftStickY = 128 + directions.y * y_percent;
    };

    // set the left stick to a forced value
    auto force_analog_stick = [&](int x_value, int y_value) {
        outputs.leftStickX = x_value;
        outputs.leftStickY = y_value;
    };

    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up,
        inputs.c_left,
        inputs.c_right,
        inputs.c_up, // swapped up and down
        inputs.c_down,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    // tilt modifiers
    if (inputs.mod_y) {
        if (inputs.a) set_analog_stick(50, 50); // ftilt
        else set_analog_stick(35, 100); // slow walk
    } else if (inputs.mod_x) {
        if (inputs.z) set_analog_stick(88, 47); // wavedash 
        else if (inputs.a) {
            if (!directions.vertical) set_analog_stick(50, 50); // angle ftilt up
            else set_analog_stick(40, 49); // dtilt + uptilt
        } else set_analog_stick(56, 100); // reduced walk
    }

    // set the shield triggers
    if (inputs.r) {
        outputs.triggerRAnalog = 140;
        if (directions.diagonal) set_analog_stick(88, 47); // wavedash
    }
    if (inputs.l) outputs.triggerLAnalog = 140;

    // quick attack mode
    if (inputs.b) {
        if (inputs.c_up) {force_analog_stick(169, 128); outputs.rightStickY = 128;} // double up zip
        else if (inputs.c_left) {set_analog_stick(56, 83); outputs.rightStickX = 128;} // sliughtly steep
        else if (inputs.a) {set_analog_stick(88, 47); outputs.a = false;} // shallow
        else if (inputs.c_down) {set_analog_stick(93, 30); outputs.rightStickY = 128;} // very shallow
        else if (inputs.c_right) {set_analog_stick(83, 56); outputs.rightStickX = 128;} // slightly shallow
        else if (inputs.mod_x) set_analog_stick(56, 100); // very steep angle
        else if (inputs.mod_y) set_analog_stick(35, 100); // steep angle 
    }

    // Qa2 OS c-stick angles
    if (inputs.midshield) {
        outputs.rightStickY = 28; outputs.rightStickX = 128;
        // outputs.rightStickY = 57;
        // outputs.rightStickX = 128 + directions.x * -57;
    }

    // light sheild modifers
    if (inputs.lightshield) {
        if (directions.vertical) force_analog_stick(169, 128); // double up zip
        else set_analog_stick(28, 100); // force jolt
    }

    // Shut off A-stick when using D-Pad layer.
    if (inputs.mod_x && inputs.mod_y) outputs.rightStickX = 128, outputs.rightStickY = 128;
}
