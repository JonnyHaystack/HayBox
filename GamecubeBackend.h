#ifndef GAMECUBEBACKEND_H_CVIKTWRY
#define GAMECUBEBACKEND_H_CVIKTWRY

#include "CommunicationBackend.h"

#include <Nintendo.h>

class GamecubeBackend : public CommunicationBackend {
public:
  GamecubeBackend(int pollingRate);
  ~GamecubeBackend();
  void SendOutputs(state::OutputState outputState);

private:
  CGamecubeConsole *mpGamecubeConsole;
  Gamecube_Data_t mGamecubeData;
  int mDelay;
};

#endif /* end of include guard: GAMECUBEBACKEND_H_CVIKTWRY */
