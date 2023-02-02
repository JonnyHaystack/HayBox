#ifndef _COMMS_DINPUTBACKEND_HPP
#define _COMMS_DINPUTBACKEND_HPP

#include "stdlib.hpp"

#include "core/CommunicationBackend.hpp"
#include "core/ControllerMode.hpp"
#include "core/InputSource.hpp"

#include <Joystick.h>

class DInputBackend : public CommunicationBackend {
  public:
    DInputBackend(InputSource **input_sources, size_t input_source_count);
    ~DInputBackend();
    void SendReport();

  private:
    int16_t GetDpadAngle(bool left, bool right, bool down, bool up);
    Joystick_ *_joystick;
};

#endif
