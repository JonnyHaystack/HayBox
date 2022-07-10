#ifndef _CORE_CONTROLLERMODE_HPP
#define _CORE_CONTROLLERMODE_HPP

#include "core/InputMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class ControllerMode : public InputMode {
  public:
    ControllerMode(socd::SocdType socd_type, uint8_t analog_stick_length = 127);
    void UpdateOutputs(InputState* inputs, OutputState* outputs);
    void ResetDirections();
    virtual void UpdateDirections(
        bool lsLeft,
        bool lsRight,
        bool lsDown,
        bool lsUp,
        bool rsLeft,
        bool rsRight,
        bool rsDown,
        bool rsUp
    );

  protected:
    StickDirections directions;
    uint8_t _analog_stick_length;
    OutputState* _outputs = nullptr;

  private:

    virtual void UpdateDigitalOutputs() = 0;
    virtual void UpdateAnalogOutputs() = 0;
};

#endif
