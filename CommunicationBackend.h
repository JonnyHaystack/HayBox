#ifndef COMMUNICATIONBACKEND_H_QIWPYHS4
#define COMMUNICATIONBACKEND_H_QIWPYHS4

#include "state.h"

class CommunicationBackend {
public:
  enum Type {
    DINPUT,
  };
  CommunicationBackend();
  virtual ~CommunicationBackend(){};
  virtual void SendOutputs(state::OutputState outputState) = 0;
};

#endif /* end of include guard: COMMUNICATIONBACKEND_H_QIWPYHS4 */
