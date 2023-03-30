#ifndef _INPUT_GAMECUBECONTROLLERINPUT_HPP
#define _INPUT_GAMECUBECONTROLLERINPUT_HPP

#include "core/InputSource.hpp"
#include "core/state.hpp"

#include <GamecubeController.hpp>
#include <gamecube_definitions.h>

class GamecubeControllerInput : public InputSource {
  public:
    GamecubeControllerInput(
        uint data_pin,
        uint polling_rate,
        PIO pio = pio0,
        int sm = -1,
        int offset = -1
    );
    ~GamecubeControllerInput();
    InputScanSpeed ScanSpeed();
    void UpdateInputs(InputState &inputs);
    int GetOffset();

  protected:
    GamecubeController *_controller;
    gc_report_t _report;
};

#endif