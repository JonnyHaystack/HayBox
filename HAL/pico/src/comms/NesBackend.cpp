#include "comms/NesBackend.hpp"

NesBackend::NesBackend(
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
      _nes(data_pin, clock_pin, latch_pin, pio, sm, offset) {}

CommunicationBackendId NesBackend::BackendId() {
    return COMMS_BACKEND_NES;
}

void NesBackend::SendReport() {
    ScanInputs();

    // Run gamemode logic
    UpdateOutputs();

    // Digital outputs
    _report.a = _outputs.a;
    _report.b = _outputs.b;
    _report.select = _outputs.select;
    _report.start = _outputs.start;
    _report.dpad_left = _outputs.dpadLeft || _outputs.leftStickX < 128;
    _report.dpad_right = _outputs.dpadRight || _outputs.leftStickX > 128;
    _report.dpad_down = _outputs.dpadDown || _outputs.leftStickY < 128;
    _report.dpad_up = _outputs.dpadUp || _outputs.leftStickY > 128;

    _nes.SendReport(_report);
}

int NesBackend::GetOffset() {
    return _nes.GetOffset();
}