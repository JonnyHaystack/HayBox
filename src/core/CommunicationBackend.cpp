#include "core/CommunicationBackend.hpp"

#include "core/ControllerMode.hpp"
#include "core/InputSource.hpp"
#include "core/state.hpp"

CommunicationBackend::CommunicationBackend(InputSource **input_sources, size_t input_source_count) {
    _gamemode = nullptr;
    _input_sources = input_sources;
    _input_source_count = input_source_count;
}

InputState &CommunicationBackend::GetInputs() {
    return _inputs;
}

void CommunicationBackend::ScanInputs() {
    for (size_t i = 0; i < _input_source_count; i++) {
        _input_sources[i]->UpdateInputs(_inputs);
    }
}

void CommunicationBackend::ScanInputs(InputScanSpeed input_source_filter) {
    for (size_t i = 0; i < _input_source_count; i++) {
        InputSource *input_source = _input_sources[i];
        if (input_source->ScanSpeed() == input_source_filter) {
            input_source->UpdateInputs(_inputs);
        }
    }
}

void CommunicationBackend::ResetOutputs() {
    _outputs = OutputState();
}

void CommunicationBackend::UpdateOutputs() {
    ResetOutputs();
    if (_gamemode != nullptr) {
        _gamemode->UpdateOutputs(_inputs, _outputs);
    }
}

void CommunicationBackend::SetGameMode(ControllerMode *gamemode) {
    delete _gamemode;
    _gamemode = gamemode;
}
