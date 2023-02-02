#ifndef _INPUT_NUNCHUKINPUT_HPP
#define _INPUT_NUNCHUKINPUT_HPP

#include "core/InputSource.hpp"

#include <ArduinoNunchuk.hpp>
#include <Wire.h>

class NunchukInput : public InputSource {
  public:
    NunchukInput(TwoWire &wire = Wire, int detect_pin = -1, int sda_pin = 4, int scl_pin = 5);
    ~NunchukInput();
    InputScanSpeed ScanSpeed();
    void UpdateInputs(InputState &inputs);

  protected:
    ArduinoNunchuk *_nunchuk;
};

#endif