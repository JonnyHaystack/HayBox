#ifndef GAMECUBEBACKEND_H_CVIKTWRY
#define GAMECUBEBACKEND_H_CVIKTWRY

#include "src/Nintendo/src/Nintendo.h"

#include "CommunicationBackend.h"

class GamecubeBackend : public CommunicationBackend {
public:
  GamecubeBackend(int pollingRate, int gccDataPin);
  ~GamecubeBackend();
  void SendOutputs(state::OutputState outputState);

private:
  CGamecubeConsole *mpGamecubeConsole;
  Gamecube_Data_t mGamecubeData;
  int mDelay;
};

#endif /* end of include guard: GAMECUBEBACKEND_H_CVIKTWRY */
