#ifndef _COMMS_N64BACKEND_HPP
#define _COMMS_N64BACKEND_HPP

#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"

#include <Nintendo.h>

class N64Backend : public CommunicationBackend {
  public:
    N64Backend(
        InputSource **input_sources,
        size_t input_source_count,
        int polling_rate,
        int data_pin
    );
    ~N64Backend();
    void SendReport();

  private:
    CN64Console *_n64;
    N64_Data_t _data;
    int _delay;
};

#endif
