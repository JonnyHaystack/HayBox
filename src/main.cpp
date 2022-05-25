/* HayB0XX Version 2.0.0 */

#define NUNCHUK_ENABLE false

// #include <Arduino.h>

#if NUNCHUK_ENABLE
#include <NintendoExtensionCtrl.h>
#endif

#include "config.hpp"

#include "config/mode_selection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

extern CommunicationBackend **backends;
extern uint8_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

#if NUNCHUK_ENABLE
Nunchuk gNunchuk;
#endif

void setup() {
    // Controller-specific setup. Must be called first.
    initialise();

#if NUNCHUK_ENABLE
    gNunchuk.begin();
    if (gNunchuk.connect()) {
        inputs.nunchuk_connected = true;
    } else {
        gNunchuk.i2c().end();
    }
#endif
}

void loop() {
    /* Mode selection */
    // select_mode();

    for (int i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->CurrentInputs());
    }
}