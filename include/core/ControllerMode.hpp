#ifndef _CORE_CONTROLLERMODE_HPP
#define _CORE_CONTROLLERMODE_HPP

#include "core/InputMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class ControllerMode : public InputMode {
  public:
    ControllerMode(socd::SocdType socd_type);
    void UpdateOutputs(InputState &inputs, OutputState &outputs);
    void ResetDirections();
    virtual void UpdateDirections(
        bool lsLeft,
        bool lsRight,
        bool lsDown,
        bool lsUp,
        bool rsLeft,
        bool rsRight,
        bool rsDown,
        bool rsUp,
        uint8_t analogStickMin,
        uint8_t analogStickNeutral,
        uint8_t analogStickMax,
        OutputState &outputs
    );

  protected:
    StickDirections directions;

  private:
    virtual void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) = 0;
    virtual void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) = 0;
};

#endif
