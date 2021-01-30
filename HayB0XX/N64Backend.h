#ifndef N64BACKEND_H_HKGOPEJN
#define N64BACKEND_H_HKGOPEJN

#include "src/Nintendo/src/Nintendo.h"

#include "CommunicationBackend.h"

class N64Backend : public CommunicationBackend {
public:
  N64Backend(int pollingRate, int gccDataPin);
  ~N64Backend();
  void SendOutputs(state::OutputState outputState);

private:
  CN64Console *mpN64Console;
  N64_Data_t mN64Data;
  int mDelay;
};

#endif /* end of include guard: N64BACKEND_H_HKGOPEJN */
