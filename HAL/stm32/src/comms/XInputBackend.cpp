#include "comms/XInputBackend.hpp"

#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

#include <USBComposite.h>

XInputBackend::XInputBackend(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count
)
    : CommunicationBackend(inputs, input_sources, input_source_count),
      _xbox360(0x0738, 0x4726) {
    // Disconnect the maple core's default USB serial device first.
    USBComposite.clear();
    _xbox360.registerComponent();
    USBComposite.begin();
    while (!USBComposite)
        ;
    // Suppress auto-send on each setter call; we send once at the end of SendReport().
    _xbox360.setManualReportMode(true);
}

CommunicationBackendId XInputBackend::BackendId() {
    return COMMS_BACKEND_XINPUT;
}

void XInputBackend::SendReport() {
    ScanInputs(InputScanSpeed::SLOW);
    ScanInputs(InputScanSpeed::MEDIUM);
    ScanInputs(InputScanSpeed::FAST);

    UpdateOutputs();

    // Digital buttons
    _xbox360.button(XBOX_A, _outputs.a);
    _xbox360.button(XBOX_B, _outputs.b);
    _xbox360.button(XBOX_X, _outputs.x);
    _xbox360.button(XBOX_Y, _outputs.y);
    _xbox360.button(XBOX_LSHOULDER, _outputs.buttonL);
    _xbox360.button(XBOX_RSHOULDER, _outputs.buttonR);
    _xbox360.button(XBOX_START, _outputs.start);
    _xbox360.button(XBOX_BACK, _outputs.select);
    _xbox360.button(XBOX_GUIDE, _outputs.home);
    _xbox360.button(XBOX_DUP, _outputs.dpadUp);
    _xbox360.button(XBOX_DDOWN, _outputs.dpadDown);
    _xbox360.button(XBOX_DLEFT, _outputs.dpadLeft);
    _xbox360.button(XBOX_DRIGHT, _outputs.dpadRight);
    _xbox360.button(XBOX_L3, _outputs.leftStickClick);
    _xbox360.button(XBOX_R3, _outputs.rightStickClick);

    // Triggers
    _xbox360.sliderLeft(_outputs.triggerLDigital ? 255 : _outputs.triggerLAnalog);
    _xbox360.sliderRight(_outputs.triggerRDigital ? 255 : _outputs.triggerRAnalog);

    // Analog sticks (convert 0-255 range to -32768..32767)
    _xbox360.X((_outputs.leftStickX - 128) * 65535 / 255 + 128);
    _xbox360.Y((_outputs.leftStickY - 128) * 65535 / 255 + 128);
    _xbox360.XRight((_outputs.rightStickX - 128) * 65535 / 255 + 128);
    _xbox360.YRight((_outputs.rightStickY - 128) * 65535 / 255 + 128);

    // Use x360_tx to keep input scanning active when the USB endpoint is busy.
    if (USBComposite) {
        x360_tx(0, _xbox360.getReport(), _xbox360.getReportSize());
    }
}
