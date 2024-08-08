#include "comms/XInputBackend.hpp"

#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

#include <Adafruit_USBD_XInput.hpp>

XInputBackend::XInputBackend(InputSource **input_sources, size_t input_source_count)
    : CommunicationBackend(input_sources, input_source_count) {
    Serial.end();
    _xinput = new Adafruit_USBD_XInput();
    _xinput->begin();
    Serial.begin(115200);

    TinyUSBDevice.setID(0x0738, 0x4726);

    while (!_xinput->ready()) {
        delay(1);
    }
}

XInputBackend::~XInputBackend() {
    delete _xinput;
}

void XInputBackend::SendReport() {
    ScanInputs(InputScanSpeed::SLOW);
    ScanInputs(InputScanSpeed::MEDIUM);

    while (!_xinput->ready()) {
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

    _report.lx = (_outputs.leftStickX - 128) * 65535 / 255 + 128;
    _report.ly = (_outputs.leftStickY - 128) * 65535 / 255 + 128;
    _report.rx = (_outputs.rightStickX - 128) * 65535 / 255 + 128;
    _report.ry = (_outputs.rightStickY - 128) * 65535 / 255 + 128;

    _xinput->sendReport(&_report);
}