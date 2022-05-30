#include "comms/DInputBackend.hpp"

#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

#include <TUGamepad.hpp>

DInputBackend::DInputBackend(InputSource **input_sources, size_t input_source_count)
    : CommunicationBackend(input_sources, input_source_count) {
    _gamepad = new TUGamepad();
    _gamepad->begin();

    while (!USBDevice.mounted()) {
        delay(1);
    }
}

DInputBackend::~DInputBackend() {
    _gamepad->releaseAll();
    delete _gamepad;
}

void DInputBackend::SendReport() {
    // Update inputs from all sources at once.
    ScanInputs();

    UpdateOutputs();

    // Digital outputs
    _gamepad->setButton(0, _outputs.b);
    _gamepad->setButton(1, _outputs.a);
    _gamepad->setButton(2, _outputs.y);
    _gamepad->setButton(3, _outputs.x);
    _gamepad->setButton(4, _outputs.buttonR);
    _gamepad->setButton(5, _outputs.triggerRDigital);
    _gamepad->setButton(6, _outputs.buttonL);
    _gamepad->setButton(7, _outputs.triggerLDigital);
    _gamepad->setButton(8, _outputs.select);
    _gamepad->setButton(9, _outputs.start);
    _gamepad->setButton(10, _outputs.rightStickClick);
    _gamepad->setButton(11, _outputs.leftStickClick);
    _gamepad->setButton(12, _outputs.home);

    // Analog _outputs
    _gamepad->leftXAxis(_outputs.leftStickX + 128);
    _gamepad->leftYAxis(128 - _outputs.leftStickY);
    _gamepad->rightXAxis(_outputs.rightStickX + 128);
    _gamepad->rightYAxis(128 - _outputs.rightStickY);
    _gamepad->triggerLAnalog(_outputs.triggerLAnalog + 129);
    _gamepad->triggerRAnalog(_outputs.triggerRAnalog + 129);

    // D-pad Hat Switch
    _gamepad->hatSwitch(
        GetDpadAngle(_outputs.dpadLeft, _outputs.dpadRight, _outputs.dpadDown, _outputs.dpadUp)
    );

    _gamepad->sendState();
}

hid_gamepad_hat_t DInputBackend::GetDpadAngle(bool left, bool right, bool down, bool up) {
    hid_gamepad_hat_t angle = GAMEPAD_HAT_CENTERED;
    if (right && !left) {
        angle = GAMEPAD_HAT_RIGHT;
        if (down)
            angle = GAMEPAD_HAT_DOWN_RIGHT;
        if (up)
            angle = GAMEPAD_HAT_UP_RIGHT;
    } else if (left && !right) {
        angle = GAMEPAD_HAT_LEFT;
        if (down)
            angle = GAMEPAD_HAT_DOWN_LEFT;
        if (up)
            angle = GAMEPAD_HAT_UP_LEFT;
    } else if (down && !up) {
        angle = GAMEPAD_HAT_DOWN;
    } else if (up && !down) {
        angle = GAMEPAD_HAT_UP;
    }
    return angle;
}
