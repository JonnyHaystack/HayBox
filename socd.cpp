#include "socd.h"

socd::SocdState socd::fTwoIPNoReactivate(bool &isLow, bool &isHigh,
                                         SocdState socdState) {
  bool is_low = false;
  bool is_high = false;
  if (isLow && isHigh) {
    if (socdState.was_high) {
      is_low = true;
      is_high = false;
      socdState.lock_high = true;
    }
    if (socdState.was_low) {
      is_low = false;
      is_high = true;
      socdState.lock_low = true;
    }
  }
  if (!isLow && isHigh && (socdState.lock_high == false)) {
    is_low = false;
    is_high = true;
    socdState.was_high = true;
    socdState.was_low = false;
    socdState.lock_low = false;
  }
  if (isLow && !isHigh && (socdState.lock_low == false)) {
    is_low = true;
    is_high = false;
    socdState.was_low = true;
    socdState.was_high = false;
    socdState.lock_high = false;
  }
  if (!isLow && !isHigh) {
    socdState.was_high = false;
    socdState.was_low = false;
    socdState.lock_low = false;
    socdState.lock_high = false;
  }
  isLow = is_low;
  isHigh = is_high;
  return socdState;
}

socd::SocdState socd::fTwoIP(bool &isLow, bool &isHigh, SocdState socdState) {
  bool is_low = false;
  bool is_high = false;
  if (isLow && socdState.was_high) {
    is_low = true;
    is_high = false;
  }
  if (isHigh && socdState.was_low) {
    is_low = false;
    is_high = true;
  }
  if (!isLow && isHigh) {
    is_low = false;
    is_high = true;
    socdState.was_high = true;
    socdState.was_low = false;
  }
  if (isLow && !isHigh) {
    is_low = true;
    is_high = false;
    socdState.was_low = true;
    socdState.was_high = false;
  }
  isLow = is_low;
  isHigh = is_high;
  return socdState;
}

void socd::fNeutral(bool &isLow, bool &isHigh) {
  bool is_low = false;
  bool is_high = false;
  if (!isLow && isHigh) {
    is_low = false;
    is_high = true;
  }
  if (isLow && !isHigh) {
    is_low = true;
    is_high = false;
  }
  isLow = is_low;
  isHigh = is_high;
}
