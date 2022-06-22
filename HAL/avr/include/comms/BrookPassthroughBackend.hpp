#ifndef _COMMS_BROOKPASSTHROUGHBACKEND_HPP
#define _COMMS_BROOKPASSTHROUGHBACKEND_HPP

#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"
#include "core/state.hpp"

class BrookPassthroughBackend : public CommunicationBackend {
  public:
    BrookPassthroughBackend(
        InputSource **input_sources,
        size_t input_source_count,
        int dpadUp_pin = -1,
        int l3_pin = -1
    );
    // ~BrookPassthroughBackend();
    void UpdateOutputs();
    void SendReport();

  private:
    int _dpadUp_pin;
    int _l3_pin;
};

#endif
