#ifndef _COMMS_SNESBACKEND_HPP
#define _COMMS_SNESBACKEND_HPP

#include "core/CommunicationBackend.hpp"

#include <SnesConsole.hpp>

class SnesBackend : public CommunicationBackend {
  public:
    SnesBackend(
        InputState &inputs,
        InputSource **input_sources,
        size_t input_source_count,
        uint data_pin,
        uint clock_pin,
        uint latch_pin,
        PIO pio = pio0,
        int sm = -1,
        int offset = -1
    );
    void SendReport();
    int GetOffset();

  private:
    SnesConsole _snes;
    snes_report_t _report;
};

#endif