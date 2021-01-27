#include "ToughLoveArena.h"
#include "KeyboardMode.h"

ToughLoveArena::ToughLoveArena(socd::SocdType socdType,
                               state::InputState &rInputState)
    : KeyboardMode(socdType, rInputState) {
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
}

void ToughLoveArena::SendKeys() {
  Press('s', mrInputState.left);
  Press('d', mrInputState.right);
  Press('j', mrInputState.b);
  Press('k', mrInputState.x);
  Press('l', mrInputState.z);
}
