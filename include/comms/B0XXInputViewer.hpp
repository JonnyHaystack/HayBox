#ifndef _COMMS_B0XXINPUTVIEWER_HPP
#define _COMMS_B0XXINPUTVIEWER_HPP

#include "core/CommunicationBackend.hpp"

enum reportState : byte {
    ReportOff = 0x30,
    ReportOn = 0x31,
    ReportEnd = 0x0A,
    ReportInvalid = 0x00
};

class B0XXInputViewer : public CommunicationBackend {
  public:
    B0XXInputViewer(InputSource **input_sources, size_t input_source_count);
    ~B0XXInputViewer();
    void SendReport();

  private:
    uint8_t _report[25];
    uint8_t _clock;
};

#endif