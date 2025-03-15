#ifndef _COMMS_NESBACKEND_HPP
#define _COMMS_NESBACKEND_HPP

#include "core/CommunicationBackend.hpp"

#include <NesConsole.hpp>

class NesBackend : public CommunicationBackend {
  public:
    NesBackend(
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
    CommunicationBackendId BackendId();
    void SendReport();
    int GetOffset();

  private:
    NesConsole _nes;
    nes_report_t _report;
};

#endif