#ifndef _MODES_PLATFORMFIGHTER_HPP
#define _MODES_PLATFORMFIGHTER_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class PlatformFighter : public ControllerMode {
  public:
    PlatformFighter(socd::SocdType socd_type, uint8_t analog_stick_length = 100);

  protected:
    bool _horizontal_socd;

    void HandleSocd();
    void SetAxis(uint8_t* axis, const int8_t &direction, const uint16_t &value);
    void SetLeftStickX(const uint16_t &value);
    void SetLeftStickY(const uint16_t &value);
    void SetStick(uint8_t* xAxis, uint8_t* yAxis, const uint8_t &xDirection, const uint8_t &yDirection, const uint16_t &xValue, const uint16_t &yValue);
    void SetLeftStick(const uint16_t &xValue, const uint16_t &yValue);
    void SetRightStick(const uint16_t &xValue, const uint16_t &yValue);
    void SetAngledFSmash(const uint16_t &xValue, const uint16_t yValue);
};

#endif
