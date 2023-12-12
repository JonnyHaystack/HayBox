#ifndef _CORE_INPUTMODE_HPP
#define _CORE_INPUTMODE_HPP

#include "socd.hpp"
#include "state.hpp"

#include <config.pb.h>

class InputMode {
  public:
    InputMode(const GameModeConfig &config);
    virtual ~InputMode();
    const GameModeConfig &GetConfig();

  protected:
    const GameModeConfig &_config;

    virtual void HandleSocd(InputState &inputs);
    virtual void HandleRemap(const InputState &original_inputs, InputState &remapped_inputs);

  private:
    socd::SocdState *_socd_states = nullptr;
};

#endif
