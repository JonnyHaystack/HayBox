#ifndef _COMMS_LBXINPUTVIEWER_HPP
#define _COMMS_LBXINPUTVIEWER_HPP

#include "core/CommunicationBackend.hpp"

enum reportState : byte {
    ReportOff = 0x30,
    ReportOn = 0x31,
    ReportEnd = 0x0A,
    ReportInvalid = 0x00
};

class LBXInputViewer : public CommunicationBackend {
  public:
    LBXInputViewer(InputSource **input_sources, size_t input_source_count);
    ~LBXInputViewer();
    void SendReport();

  private:
    uint8_t _report[26];
    uint8_t _clock;
};

#endif