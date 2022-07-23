#include "comms/N64Backend.hpp"

#include "core/InputSource.hpp"

#include <N64Console.hpp>
#include <hardware/pio.h>

N64Backend::N64Backend(
    InputSource **input_sources,
    size_t input_source_count,
    uint data_pin,
    PIO pio,
    int sm,
    int offset
)
    : CommunicationBackend(input_sources, input_source_count) {
    _n64 = new N64Console(data_pin, pio, sm, offset);
    _report = default_n64_report;
}

N64Backend::~N64Backend() {
    delete _n64;
}

void N64Backend::SendReport() {
    // Update slower inputs before we start waiting for poll.
    ScanInputs(InputScanSpeed::SLOW);
    ScanInputs(InputScanSpeed::MEDIUM);

    // Read inputs
    _n64->WaitForPoll();

    // Update fast inputs in response to poll.
    ScanInputs(InputScanSpeed::FAST);

    // Run gamemode logic.
    UpdateOutputs();

    // Digital outputs
    _report.a = _outputs.a;
    _report.b = _outputs.b;
    _report.z = _outputs.buttonR;
    _report.l = _outputs.triggerLDigital;
    _report.r = _outputs.triggerRDigital;
    _report.start = _outputs.start;
    _report.dpad_left = _outputs.dpadLeft;
    _report.dpad_right = _outputs.dpadRight;
    _report.dpad_down = _outputs.dpadDown;
    _report.dpad_up = _outputs.dpadUp;
    // Somewhat ugly way of mapping right stick to C-Pad
    _report.c_left = _outputs.rightStickX < 128;
    _report.c_right = _outputs.rightStickX > 128;
    _report.c_down = _outputs.rightStickY < 128;
    _report.c_up = _outputs.rightStickY > 128;

    // Analog outputs
    _report.stick_x = _outputs.leftStickX - 128;
    _report.stick_y = _outputs.leftStickY - 128;

    // Send outputs to console.
    _n64->SendReport(&_report);
}

int N64Backend::GetOffset() {
    return _n64->GetOffset();
}
