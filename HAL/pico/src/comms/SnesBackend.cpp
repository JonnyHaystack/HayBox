#include "comms/SnesBackend.hpp"

SnesBackend::SnesBackend(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    uint data_pin,
    uint clock_pin,
    uint latch_pin,
    PIO pio,
    int sm,
    int offset
)
    : CommunicationBackend(inputs, input_sources, input_source_count),
      _snes(data_pin, clock_pin, latch_pin, pio, sm, offset) {}

void SnesBackend::SendReport() {
    ScanInputs();

    // Run gamemode logic
    UpdateOutputs();

    // Digital outputs
    _report.a = _outputs.a;
    _report.b = _outputs.b;
    _report.x = _outputs.x;
    _report.y = _outputs.y;
    _report.l = _outputs.buttonL || _outputs.triggerLDigital;
    _report.r = _outputs.buttonR || _outputs.triggerRDigital;
    _report.select = _outputs.select;
    _report.start = _outputs.start;
    _report.dpad_left = _outputs.dpadLeft || _outputs.leftStickX < 128;
    _report.dpad_right = _outputs.dpadRight || _outputs.leftStickX > 128;
    _report.dpad_down = _outputs.dpadDown || _outputs.leftStickY < 128;
    _report.dpad_up = _outputs.dpadUp || _outputs.leftStickY > 128;

    _snes.SendReport(_report);
}

int SnesBackend::GetOffset() {
    return _snes.GetOffset();
}