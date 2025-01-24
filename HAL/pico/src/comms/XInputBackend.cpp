#include "comms/XInputBackend.hpp"

#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

#include <Adafruit_USBD_XInput.hpp>

XInputBackend::XInputBackend(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count
)
    : CommunicationBackend(inputs, input_sources, input_source_count),
      _xinput() {
    Serial.end();
    _xinput.begin();
    Serial.begin(115200);

    TinyUSBDevice.setID(0x0738, 0x4726);
}

CommunicationBackendId XInputBackend::BackendId() {
    return COMMS_BACKEND_XINPUT;
}

void XInputBackend::SendReport() {
    ScanInputs(InputScanSpeed::SLOW);
    ScanInputs(InputScanSpeed::MEDIUM);

    while (!_xinput.ready()) {
        tight_loop_contents();
    }

    ScanInputs(InputScanSpeed::FAST);

    UpdateOutputs();

    // Digital outputs
    _report.a = _outputs.a;
    _report.b = _outputs.b;
    _report.x = _outputs.x;
    _report.y = _outputs.y;
    _report.lb = _outputs.buttonL;
    _report.rb = _outputs.buttonR;
    _report.lt = _outputs.triggerLDigital ? 255 : _outputs.triggerLAnalog;
    _report.rt = _outputs.triggerRDigital ? 255 : _outputs.triggerRAnalog;
    _report.start = _outputs.start;
    _report.back = _outputs.select;
    _report.home = _outputs.home;
    _report.dpad_up = _outputs.dpadUp;
    _report.dpad_down = _outputs.dpadDown;
    _report.dpad_left = _outputs.dpadLeft;
    _report.dpad_right = _outputs.dpadRight;
    _report.ls = _outputs.leftStickClick;
    _report.rs = _outputs.rightStickClick;

    _report.lx = _outputs.leftStickX * 258 - 33024;
    _report.ly = _outputs.leftStickY * 258 - 33024;
    _report.rx = _outputs.rightStickX * 258 - 33024;
    _report.ry = _outputs.rightStickY * 258 - 33024;

    _xinput.sendReport(&_report);
}