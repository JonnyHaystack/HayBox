#ifndef _CORE_COMMUNICATIONBACKEND_HPP
#define _CORE_COMMUNICATIONBACKEND_HPP

#include "core/ControllerMode.hpp"
#include "core/InputSource.hpp"
#include "state.hpp"

class CommunicationBackend {
  public:
    CommunicationBackend(
        InputState &inputs,
        InputSource **input_sources,
        size_t input_source_count
    );
    virtual ~CommunicationBackend(){};

    InputState &GetInputs();
    void ScanInputs();
    void ScanInputs(InputScanSpeed input_source_filter);

    virtual void UpdateOutputs();
    virtual CommunicationBackendId BackendId();
    virtual void SetGameMode(ControllerMode *gamemode);
    virtual ControllerMode *CurrentGameMode();

    virtual void SendReport() = 0;

  protected:
    InputState &_inputs;
    InputSource **_input_sources;
    size_t _input_source_count;

    OutputState _outputs;
    ControllerMode *_gamemode = nullptr;

  private:
    void ResetOutputs();
};

#endif
