#include "comms/GamecubeBackend.hpp"

#include "core/InputSource.hpp"

#include <GamecubeConsole.hpp>
#include <hardware/pio.h>
#include <hardware/timer.h>

GamecubeBackend::GamecubeBackend(
    InputSource **input_sources,
    size_t input_source_count,
    uint data_pin,
    PIO pio,
    int sm,
    int offset
)
    : CommunicationBackend(input_sources, input_source_count) {
    _gamecube = new GamecubeConsole(data_pin, pio, sm, offset);
    _report = default_gc_report;
}

GamecubeBackend::~GamecubeBackend() {
    delete _gamecube;
}

void GamecubeBackend::SendReport() {
    // Update slower inputs before we start waiting for poll.
    ScanInputs(InputScanSpeed::SLOW);
    ScanInputs(InputScanSpeed::MEDIUM);

    // Read inputs
    _gamecube->WaitForPollStart();

    // Update fast inputs in response to poll.
    // But wait 40us first so that we read inputs at the start of the 3rd byte of the poll command
    // not the second, so inputs are maximum 40us old by the time we start sending the report.
    busy_wait_us(40);
    ScanInputs(InputScanSpeed::FAST);

    // Run gamemode logic.
    UpdateOutputs();

    // Digital outputs
    _report.a = _outputs.a;
    _report.b = _outputs.b;
    _report.x = _outputs.x;
    _report.y = _outputs.y;
    _report.z = _outputs.buttonR;
    _report.l = _outputs.triggerLDigital;
    _report.r = _outputs.triggerRDigital;
    _report.start = _outputs.start;
    _report.dpad_left = _outputs.dpadLeft;
    _report.dpad_right = _outputs.dpadRight;
    _report.dpad_down = _outputs.dpadDown;
    _report.dpad_up = _outputs.dpadUp;

    // Analog outputs
    _report.stick_x = _outputs.leftStickX;
    _report.stick_y = _outputs.leftStickY;
    _report.cstick_x = _outputs.rightStickX;
    _report.cstick_y = _outputs.rightStickY;
    _report.l_analog = _outputs.triggerLAnalog;
    _report.r_analog = _outputs.triggerRAnalog;

    // Send outputs to console unless poll command is invalid.
    if (_gamecube->WaitForPollEnd() != PollStatus::ERROR) {
        _gamecube->SendReport(&_report);
    }
}

int GamecubeBackend::GetOffset() {
    return _gamecube->GetOffset();
}
