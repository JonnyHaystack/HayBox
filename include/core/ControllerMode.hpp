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
    OutputState* _outputs = nullptr;

    void SetLeftStickX(const uint16_t &value);
    void SetLeftStickY(const uint16_t &value);
    void SetStick(uint8_t* xAxis, uint8_t* yAxis, const uint8_t &xDirection, const uint8_t &yDirection, const uint16_t &xValue, const uint16_t &yValue);
    void SetLeftStick(const uint16_t &xValue, const uint16_t &yValue);
    void SetRightStick(const uint16_t &xValue, const uint16_t &yValue);
    void SetAngledFSmash(const uint16_t &xValue, const uint16_t yValue);

  private:
    uint8_t analog_stick_length;

    virtual void UpdateDigitalOutputs() = 0;
    virtual void UpdateAnalogOutputs() = 0;

    void SetAxis(uint8_t* axis, const int8_t &direction, const uint16_t &value);
};

#endif
