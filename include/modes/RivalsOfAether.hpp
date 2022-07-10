#ifndef _MODES_RIVALSOFAETHER_HPP
#define _MODES_RIVALSOFAETHER_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class RivalsOfAether : public ControllerMode {
  public:
    RivalsOfAether(socd::SocdType socd_type);

  private:
    void UpdateDigitalOutputs();
    void UpdateAnalogOutputs();
};

#endif
