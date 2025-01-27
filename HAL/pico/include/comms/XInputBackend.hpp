#ifndef _COMMS_XINPUTBACKEND_HPP
#define _COMMS_XINPUTBACKEND_HPP

#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"
#include "stdlib.hpp"

#include <Adafruit_USBD_XInput.hpp>

class XInputBackend : public CommunicationBackend {
  public:
    XInputBackend(InputState &inputs, InputSource **input_sources, size_t input_source_count);
    CommunicationBackendId BackendId();
    void SendReport();
    int16_t XInputBackend::ScaleValue(uint8_t input);

  private:
    Adafruit_USBD_XInput _xinput;
    xinput_report_t _report = {};
};

#endif