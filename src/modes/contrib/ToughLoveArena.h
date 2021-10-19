#ifndef TOUGHLOVEARENA_H_6O0WX1JN
#define TOUGHLOVEARENA_H_6O0WX1JN

#include "core/KeyboardMode.h"
#include "core/socd.h"
#include "core/state.h"

class ToughLoveArena : public KeyboardMode {
public:
  ToughLoveArena(socd::SocdType, state::InputState &rInputState);
  void SendKeys();
};

#endif /* end of include guard: TOUGHLOVEARENA_H_6O0WX1JN */
