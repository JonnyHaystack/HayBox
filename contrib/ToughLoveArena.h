#ifndef TOUGHLOVEARENA_H_6O0WX1JN
#define TOUGHLOVEARENA_H_6O0WX1JN

#include "KeyboardMode.h"
#include "socd.h"
#include "state.h"

class ToughLoveArena : public KeyboardMode {
public:
  ToughLoveArena(socd::SocdType, state::InputState &rInputState);
  void SendKeys();
};

#endif /* end of include guard: TOUGHLOVEARENA_H_6O0WX1JN */
