#ifndef INPUTMODE_H_ISO9UTAB
#define INPUTMODE_H_ISO9UTAB

#include <vector>

#include "socd.h"
#include "state.h"

class InputMode {
public:
  InputMode(socd::SocdType socdType, state::InputState &rInputState);
  virtual ~InputMode(){};
  virtual void UpdateOutputs() = 0;

protected:
  state::InputState &mrInputState;
  std::vector<socd::SocdPair> mSocdPairs;
  virtual void HandleSocd();

private:
  socd::SocdType mSocdType;
  std::vector<socd::SocdState> mSocdStates;
};

#endif /* end of include guard: INPUTMODE_H_ISO9UTAB */
