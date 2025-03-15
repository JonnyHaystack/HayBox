#ifndef _COMMS_DINPUTBACKEND_HPP
#define _COMMS_DINPUTBACKEND_HPP

#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"
#include "stdlib.hpp"

#include <TUGamepad.hpp>

class DInputBackend : public CommunicationBackend {
  public:
    DInputBackend(InputState &inputs, InputSource **input_sources, size_t input_source_count);
    ~DInputBackend();
    CommunicationBackendId BackendId();
    void SendReport();

  private:
    TUGamepad _gamepad;
};

#endif
