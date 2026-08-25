#include "modes/Rivals2.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

bool input_persist; // for angled tilts
int timer = 0; // for angled tilts

Rivals2::Rivals2() : ControllerMode() {}

void Rivals2::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.rt1;
    outputs.b = inputs.rf1;
    outputs.x = inputs.rf2;
    outputs.y = inputs.rf6;
    outputs.buttonR = inputs.rf3;
    if (inputs.nunchuk_connected) {
        // Lightshield with C button.
        if (inputs.nunchuk_c) {
            outputs.triggerLAnalog = 49;
        }
        outputs.triggerLDigital = inputs.nunchuk_z;
    } else {
        outputs.triggerLDigital = inputs.lf4;
    }
    outputs.triggerRDigital = inputs.rf5;
    outputs.start = inputs.mb1;
    outputs.select = inputs.mb3;
    outputs.home = inputs.mb2;
    outputs.leftStickClick = inputs.rf7;
    outputs.buttonL = inputs.rf8; // changed from rightStickClick to buttonL
    // only because buttonL is a default mapping in Rivals 2 (doesn't really matter)

    // Activate D-Pad layer by holding Mod X + Mod Y.
    if (inputs.lt1 && inputs.lt2) {
        outputs.dpadUp = inputs.rt4;
        outputs.dpadDown = inputs.rt2;
        outputs.dpadLeft = inputs.rt3;
        outputs.dpadRight = inputs.rt5;
    }
}

void Rivals2::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.lf3, // Left
        inputs.lf1, // Right
        inputs.lf2, // Down
        inputs.rf4, // Up
        inputs.rt3, // C-Left
        inputs.rt5, // C-Right
        inputs.rt2, // C-Down
        inputs.rt4, // C-Up
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    bool shield_button_pressed = inputs.lf4 || inputs.rf5; // if L or R are pressed

    // added this conditional to give joystick accurate diagonals
    // rather than (+/- 1.2, 1.2) should be (0.87~, 0.87~)
    if (directions.diagonal && !shield_button_pressed) {
        // 92 (0.78 in-game), reduced below 0.8 to allow crouch
        // tilts/crouch turn-around tilts
        outputs.leftStickX = 128 + (directions.x * 92);
        // Y value 0.83. >0.8 allows fast fall
        outputs.leftStickY = 128 + (directions.y * 96);
    }

    if (directions.diagonal && shield_button_pressed) {
        // (0.77~, 0.77~) to prevent spot dodging when pressing diagonal on the ground
        outputs.leftStickX = 128 + (directions.x * 92);
        outputs.leftStickY = 128 + (directions.y * 92);
    }

    // For MX Angled Tilts when input_persist is true
    //(x, y), (69, 53), (~0.506, ~0.31) [coords, code_values, in-game values]
    if (input_persist) { // input_persist becomes true if ModX + diagonal + A
        timer++;
        outputs.leftStickX = 128 + (directions.x * 69);
        outputs.leftStickY = 128 + (directions.y * 53);
    }

    if (timer == 150) { // 150 has a 90% success rate on pico
        timer = 0;
        input_persist = false;
    }

    if (inputs.lt1) { // if ModX is held
        outputs.leftStickY = ANALOG_STICK_MAX;
    }

    if (inputs.lt2) { // if ModY is held
       outputs.rightStickClick = 1;
    }

    // Shut off C-stick when using D-Pad layer.
    if (inputs.lt1 && inputs.lt2) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
