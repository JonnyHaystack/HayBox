#include "comms/DInputBackend.hpp"

#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

#include <Joystick.h>

DInputBackend::DInputBackend(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count
)
    : CommunicationBackend(inputs, input_sources, input_source_count) {
    _joystick.begin();
}

DInputBackend::~DInputBackend() {
    _joystick.end();
}

void DInputBackend::SendReport() {
    // Update inputs from all sources at once.
    ScanInputs();

    UpdateOutputs();

    // Digital outputs
    _joystick.setButton(0, _outputs.b);
    _joystick.setButton(1, _outputs.a);
    _joystick.setButton(2, _outputs.y);
    _joystick.setButton(3, _outputs.x);
    _joystick.setButton(4, _outputs.buttonR);
    _joystick.setButton(5, _outputs.triggerRDigital);
    _joystick.setButton(6, _outputs.buttonL);
    _joystick.setButton(7, _outputs.triggerLDigital);
    _joystick.setButton(8, _outputs.select);
    _joystick.setButton(9, _outputs.start);
    _joystick.setButton(10, _outputs.rightStickClick);
    _joystick.setButton(11, _outputs.leftStickClick);
    _joystick.setButton(12, _outputs.home);

    // Analog outputs
    _joystick.setLeftXAxis(_outputs.leftStickX);
    _joystick.setLeftYAxis(255 - _outputs.leftStickY);
    _joystick.setRightXAxis(_outputs.rightStickX);
    _joystick.setRightYAxis(255 - _outputs.rightStickY);
    _joystick.setLeftTrigger(_outputs.triggerLAnalog + 1);
    _joystick.setRightTrigger(_outputs.triggerRAnalog + 1);

    // D-pad Hat Switch
    _joystick
        ->setHatSwitch(_outputs.dpadLeft, _outputs.dpadRight, _outputs.dpadDown, _outputs.dpadUp);

    _joystick.sendState();
}
