#include "comms/GamecubeBackend.hpp"

#include "core/ControllerMode.hpp"
#include "core/InputSource.hpp"

#include <Nintendo.h>

GamecubeBackend::GamecubeBackend(
    InputSource **input_sources,
    size_t input_source_count,
    int polling_rate,
    int data_pin
)
    : CommunicationBackend(input_sources, input_source_count) {
    _gamecube = new CGamecubeConsole(data_pin);
    _data = defaultGamecubeData;

    if (polling_rate > 0) {
        // Delay used between input updates to postpone them until right before the
        // next poll, while also leaving time (850us) for processing to finish.
        _delay = (1000000 / polling_rate) - 850;
    } else {
        // If polling rate is set to 0, disable the delay.
        _delay = 0;
    }
}

GamecubeBackend::~GamecubeBackend() {
    delete _gamecube;
}

void GamecubeBackend::SendReport() {
    // Update inputs from all sources at once.
    ScanInputs();

    // Run gamemode logic.
    UpdateOutputs();

    // Digital outputs
    _data.report.a = _outputs.a;
    _data.report.b = _outputs.b;
    _data.report.x = _outputs.x;
    _data.report.y = _outputs.y;
    _data.report.z = _outputs.buttonR;
    _data.report.l = _outputs.triggerLDigital;
    _data.report.r = _outputs.triggerRDigital;
    _data.report.start = _outputs.start;
    _data.report.dleft = _outputs.dpadLeft | _outputs.select;
    _data.report.dright = _outputs.dpadRight | _outputs.home;
    _data.report.ddown = _outputs.dpadDown;
    _data.report.dup = _outputs.dpadUp;

    // Analog outputs
    _data.report.xAxis = _outputs.leftStickX;
    _data.report.yAxis = _outputs.leftStickY;
    _data.report.cxAxis = _outputs.rightStickX;
    _data.report.cyAxis = _outputs.rightStickY;
    _data.report.left = _outputs.triggerLAnalog + 31;
    _data.report.right = _outputs.triggerRAnalog + 31;

    // Send outputs to console.
    _gamecube->write(_data);

    delayMicroseconds(_delay);
}
