#ifndef _COMMS_DINPUTBACKEND_HPP
#define _COMMS_DINPUTBACKEND_HPP

#include "core/CommunicationBackend.hpp"
#include "core/ControllerMode.hpp"
#include "core/InputSource.hpp"
#include "stdlib.hpp"

#include <TUGamepad.hpp>

class DInputBackend : public CommunicationBackend {
  public:
    DInputBackend(InputSource **input_sources, size_t input_source_count);
    ~DInputBackend();
    void SendReport();

  private:
    hid_gamepad_hat_t GetDpadAngle(bool left, bool right, bool down, bool up);
    TUGamepad *_gamepad;
};

#endif
