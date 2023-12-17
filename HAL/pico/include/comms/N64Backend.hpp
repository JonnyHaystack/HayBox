#ifndef _COMMS_N64BACKEND_HPP
#define _COMMS_N64BACKEND_HPP

#include "core/CommunicationBackend.hpp"

#include <N64Console.hpp>
#include <hardware/pio.h>

class N64Backend : public CommunicationBackend {
  public:
    N64Backend(
        InputState &inputs,
        InputSource **input_sources,
        size_t input_source_count,
        uint data_pin,
        PIO pio = pio0,
        int sm = -1,
        int offset = -1
    );
    void SendReport();
    int GetOffset();

  private:
    N64Console _n64;
    n64_report_t _report;
};

#endif
