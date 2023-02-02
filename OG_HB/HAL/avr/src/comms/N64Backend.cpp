#include "comms/N64Backend.hpp"

#include <Nintendo.h>

N64Backend::N64Backend(
    InputSource **input_sources,
    size_t input_source_count,
    int polling_rate,
    int data_pin
)
    : CommunicationBackend(input_sources, input_source_count) {
    _n64 = new CN64Console(data_pin);
    _data = defaultN64Data;

    if (polling_rate > 0) {
        // Delay used between input updates to postpone them until right before the
        // next poll, while also leaving time (850us) for processing to finish.
        _delay = (1000000 / polling_rate) - 850;
    } else {
        // If polling rate is set to 0, disable the delay.
        _delay = 0;
    }
}

N64Backend::~N64Backend() {
    delete _n64;
}

void N64Backend::SendReport() {
    // Update inputs from all sources at once.
    ScanInputs();

    // Run gamemode logic.
    UpdateOutputs();

    // Digital outputs
    _data.report.a = _outputs.a;
    _data.report.b = _outputs.b;
    _data.report.z = _outputs.buttonR;
    _data.report.l = _outputs.triggerLDigital;
    _data.report.r = _outputs.triggerRDigital;
    _data.report.start = _outputs.start;
    _data.report.dleft = _outputs.dpadLeft;
    _data.report.dright = _outputs.dpadRight;
    _data.report.ddown = _outputs.dpadDown;
    _data.report.dup = _outputs.dpadUp;
    // Somewhat ugly way of mapping right stick to C-Pad
    _data.report.cleft = _outputs.rightStickX < 128;
    _data.report.cright = _outputs.rightStickX > 128;
    _data.report.cdown = _outputs.rightStickY < 128;
    _data.report.cup = _outputs.rightStickY > 128;

    // Analog outputs - converted from unsigned to signed 8-bit integers
    _data.report.xAxis = _outputs.leftStickX - 128;
    _data.report.yAxis = _outputs.leftStickY - 128;

    // Send outputs to console.
    _n64->write(_data);

    delayMicroseconds(_delay);
}
