/* HayBox Version 2.0.0 */

#include "config.hpp"

#include "config/mode_selection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

extern CommunicationBackend **backends;
extern size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

void setup() {
    // Controller-specific setup. Must be called first.
    initialise();
}

void loop() {
    /* Mode selection */
    // select_mode();

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}