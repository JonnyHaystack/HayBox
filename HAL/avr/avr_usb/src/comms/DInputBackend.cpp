#include "comms/DInputBackend.hpp"

#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

#include <Joystick.h>

DInputBackend::DInputBackend(InputSource **input_sources, size_t input_source_count)
    : CommunicationBackend(input_sources, input_source_count) {
    _joystick = new Joystick_(
        JOYSTICK_DEFAULT_REPORT_ID,
        JOYSTICK_TYPE_GAMEPAD,
        16, // Button Count
        1, // Hat Switch Count
        true, // X Axis
        true, // Y Axis
        true, // Z Axis
        true, // Rx Axis
        true, // Ry Axis
        true, // Rz Axis
        false, // No rudder
        false, // No throttle
        false, // No accelerator
        false, // No brake
        false // No steering
    );

    _joystick->begin(false);
    _joystick->setXAxisRange(0, 255);
    _joystick->setYAxisRange(0, 255);
    _joystick->setRxAxisRange(0, 255);
    _joystick->setRyAxisRange(0, 255);
    _joystick->setZAxisRange(0, 255);
    _joystick->setRzAxisRange(0, 255);
}

DInputBackend::~DInputBackend() {
    _joystick->end();
    delete _joystick;
}

void DInputBackend::SendReport() {
    // Update inputs from all sources at once.
    ScanInputs();

    UpdateOutputs();

    // Digital outputs
    _joystick->setButton(0, _outputs.b);
    _joystick->setButton(1, _outputs.a);
    _joystick->setButton(2, _outputs.y);
    _joystick->setButton(3, _outputs.x);
    _joystick->setButton(4, _outputs.buttonR);
    _joystick->setButton(5, _outputs.triggerRDigital);
    _joystick->setButton(6, _outputs.buttonL);
    _joystick->setButton(7, _outputs.triggerLDigital);
    _joystick->setButton(8, _outputs.select);
    _joystick->setButton(9, _outputs.start);
    _joystick->setButton(10, _outputs.rightStickClick);
    _joystick->setButton(11, _outputs.leftStickClick);
    _joystick->setButton(12, _outputs.home);

    // Analog outputs
    _joystick->setXAxis(_outputs.leftStickX);
    _joystick->setYAxis(255 - _outputs.leftStickY);
    _joystick->setRxAxis(_outputs.rightStickX);
    _joystick->setRyAxis(255 - _outputs.rightStickY);
    _joystick->setZAxis(_outputs.triggerLAnalog + 1);
    _joystick->setRzAxis(_outputs.triggerRAnalog + 1);

    // D-pad Hat Switch
    _joystick->setHatSwitch(
        0,
        GetDpadAngle(_outputs.dpadLeft, _outputs.dpadRight, _outputs.dpadDown, _outputs.dpadUp)
    );

    _joystick->sendState();
}

int16_t DInputBackend::GetDpadAngle(bool left, bool right, bool down, bool up) {
    int16_t angle = -1;
    if (right && !left) {
        angle = 90;
        if (down)
            angle = 135;
        if (up)
            angle = 45;
    } else if (left && !right) {
        angle = 270;
        if (down)
            angle = 225;
        if (up)
            angle = 315;
    } else if (down && !up) {
        angle = 180;
    } else if (up && !down) {
        angle = 0;
    }
    return angle;
}
