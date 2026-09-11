#ifndef _COMMS_XINPUTBACKEND_HPP
#define _COMMS_XINPUTBACKEND_HPP

#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"
#include "stdlib.hpp"

#include <USBComposite.h>

class XInputBackend : public CommunicationBackend {
  public:
    XInputBackend(InputState &inputs, InputSource **input_sources, size_t input_source_count);
    CommunicationBackendId BackendId();
    void SendReport();

  private:
    USBXBox360 _xbox360;
};

#endif
