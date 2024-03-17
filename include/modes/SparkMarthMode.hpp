#ifndef _MODES_SPARKMARTHMODE_HPP
#define _MODES_SPARKMARTHMODE_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

typedef struct {
    bool crouch_walk_os = false;
} SparkMarthModeOptions;

class SparkMarthMode : public ControllerMode {
  public:
    SparkMarthMode(socd::SocdType socd_type, SparkMarthModeOptions options = {});

  protected:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    SparkMarthModeOptions _options;
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
