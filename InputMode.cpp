#include "InputMode.h"

InputMode::InputMode(socd::SocdType socdType, state::InputState &rInputState)
    : mrInputState(rInputState) {
  mSocdType = socdType;
  mrInputState = rInputState;
}

void InputMode::HandleSocd() {
  // Initialize SOCD states if they aren't initialized.
  if (mSocdStates.size() != mSocdPairs.size()) {
    for (int i = 0; i < mSocdPairs.size(); i++) {
      mSocdStates.push_back({
          .was_low = false,
          .was_high = false,
          .lock_low = false,
          .lock_high = false,
      });
    }
  }

  // Handle SOCD resolution for each SOCD button pair.
  for (int i = 0; i < mSocdPairs.size(); i++) {
    socd::SocdPair pair = mSocdPairs[i];
    socd::SocdState socd_state = mSocdStates[i];
    switch (mSocdType) {
    case socd::SOCD_NEUTRAL:
      socd::fNeutral(*pair.button_low, *pair.button_high);
      break;
    case socd::SOCD_2IP:
      mSocdStates[i] =
          socd::fTwoIP(*pair.button_low, *pair.button_high, socd_state);
      break;
    case socd::SOCD_2IP_NO_REAC:
      mSocdStates[i] = socd::fTwoIPNoReactivate(*pair.button_low,
                                                *pair.button_high, socd_state);
      break;
    case socd::SOCD_KEYBOARD:
      break;
    }
  }
}
