/* Ultimate profile by Nathan "Sleepy" Koenigsmark */
#include "modes/SmashboxClone.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

SmashboxClone::SmashboxClone(socd::SocdType socd_type) : ControllerMode(socd_type) {
    RemappedInputState mapped;
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::a      },
    };
}

void SmashboxClone::HandleSocd(InputState &inputs) {
    InputMode::HandleSocd(inputs);
    mapped.start = inputs.start;
    mapped.select = inputs.select;
    mapped.home = inputs.home;
    mapped.y = inputs.lightshield;
    mapped.x = inputs.x;
    mapped.b = inputs.r;
    mapped.a = inputs.b;
    mapped.l = inputs.l;
    mapped.r = inputs.midshield;
    mapped.z = inputs.z;
    mapped.up = inputs.up;
    mapped.down = inputs.down;
    mapped.right = inputs.right;
    mapped.left = inputs.left;
    mapped.mod_x = inputs.mod_x;
    mapped.mod_y = inputs.mod_y;
    mapped.c_left = inputs.c_left;
    mapped.c_right = inputs.c_right;
    mapped.c_up = inputs.a;
    mapped.c_down = inputs.c_down;
    mapped.b2 = inputs.c_up;
    mapped.mode = inputs.y;
    mapped.dpad_up = inputs.nunchuk_c;
    mapped.dpad_toggle = inputs.mod_x && inputs.mod_y;
}

void SmashboxClone::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = mapped.a;
    outputs.b = mapped.b || mapped.b2;
    outputs.x = mapped.x;
    outputs.y = mapped.y;
    outputs.buttonR = mapped.z;
    outputs.start = mapped.start;
    outputs.dpadUp = mapped.dpad_up;

    // Turn on D-Pad layer by holding Mod X + Mod Y or Nunchuk C button.
    if (mapped.dpad_toggle) {
        outputs.dpadUp = mapped.c_up;
        outputs.dpadDown = mapped.c_down;
        outputs.dpadLeft = mapped.c_left;
        outputs.dpadRight = mapped.c_right;
    }
}

void SmashboxClone::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        mapped.left,
        mapped.right,
        mapped.down,
        mapped.up,
        mapped.c_left,
        mapped.c_right,
        mapped.c_down,
        mapped.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    // set the shield triggers
    if (mapped.l) outputs.triggerLAnalog = 140;
    if (mapped.r) outputs.triggerRAnalog = 140;

    // Shut off C-stick when using D-Pad layer.
    if (mapped.dpad_toggle) outputs.rightStickX = 128, outputs.rightStickY = 128;

    // handle standard modifiers
    if (mapped.mode) {
        if(!directions.horizontal && directions.vertical) outputs.leftStickX = 169, outputs.leftStickY = 128;
        else if(mapped.mod_x) outputs.leftStickX = 128 + directions.x * 35;
        else if (mapped.mod_y) {
            outputs.leftStickY = 128 + directions.y * 30;
            outputs.leftStickX = 128 + directions.x * 93;
        } else outputs.leftStickX = 128 + directions.x * 28;
    } else {
        if (mapped.mod_x) outputs.leftStickX = 128 + directions.x * 56;
        else if (mapped.mod_y) {
            outputs.leftStickY = 128 + directions.y * (mapped.b2 || mapped.z ? 47 : 49);
            outputs.leftStickX = 128 + directions.x * (mapped.b2 || mapped.z ? 88 : 40);
        }
    }

    // angled cstick ftilt
    if (directions.vertical && (mapped.c_left || mapped.c_right)) {
        outputs.rightStickX = 128 + (directions.cx * 68);
        outputs.rightStickY = 128 + (directions.y * 42);
    }

    // turnabround c-stick tilts
    if (directions.horizontal && (mapped.c_up || mapped.c_down)) {
        outputs.rightStickX = 128 + (directions.x * 42);
        outputs.rightStickY = 128 + (directions.cy * 68);
    }

    // Clean inputs on b2 to prevent polling error
    b2_counter = mapped.b2 ? b2_counter + 1 : 0;
    if (mapped.b2 && b2_counter < 3) {
        outputs.leftStickX = 128;
        outputs.leftStickY = 228;
    }
}