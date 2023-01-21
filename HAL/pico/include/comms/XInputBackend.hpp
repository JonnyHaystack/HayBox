#ifndef _COMMS_XINPUTBACKEND_HPP
#define _COMMS_XINPUTBACKEND_HPP

#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"
#include "stdlib.hpp"

#include <Adafruit_USBD_XInput.hpp>

class XInputBackend : public CommunicationBackend {
  public:
    XInputBackend(InputSource **input_sources, size_t input_source_count);
    ~XInputBackend();
    void SendReport();

  private:
    Adafruit_USBD_XInput *_xinput;
    xinput_report_t _report = {};
};

#endif