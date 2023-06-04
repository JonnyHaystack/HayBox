#include "input/GamecubeControllerInput.hpp"

#include "core/InputSource.hpp"

#include <GamecubeController.hpp>

GamecubeControllerInput::GamecubeControllerInput(
    uint pin,
    uint polling_rate,
    PIO pio,
    int sm,
    int offset
) {
    _controller = new GamecubeController(pin, polling_rate, pio, sm, offset);
}

GamecubeControllerInput::~GamecubeControllerInput() {
    delete _controller;
}

InputScanSpeed GamecubeControllerInput::ScanSpeed() {
    return InputScanSpeed::SLOW;
}

void GamecubeControllerInput::UpdateInputs(InputState &inputs) {
    // Only update inputs if poll response is received.
    if (_controller->Poll(&_report, false)) {
        inputs.nunchuk_connected = true;
        inputs.nunchuk_x = _report.stick_x;
        inputs.nunchuk_y = _report.stick_y;
        inputs.nunchuk_z = _report.l;
        inputs.nunchuk_start = _report.start;
        inputs.nunchuk_dleft = _report.dpad_left;
        inputs.nunchuk_dright = _report.dpad_right;
        inputs.nunchuk_dup = _report.dpad_up;
        inputs.nunchuk_ddown = _report.dpad_down;
        inputs.l_analog = _report.l_analog;
    }
}

int GamecubeControllerInput::GetOffset() {
    return _controller->GetOffset();
}
