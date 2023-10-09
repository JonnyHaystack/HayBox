#include "comms/B0XXInputViewer.hpp"

#include "core/InputSource.hpp"
#include "serial.hpp"

#define ASCII_BIT(x) (x ? '1' : '0');

B0XXInputViewer::B0XXInputViewer(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count
)
    : CommunicationBackend(inputs, input_sources, input_source_count) {
    serial::init(115200);
}

B0XXInputViewer::~B0XXInputViewer() {
    serial::close();
}

void B0XXInputViewer::SendReport() {
    if (serial::available_for_write() < 32) {
        return;
    }

    // Report clock is used to limit input viewer reports to being sent only once every 5
    // increments.
    if (_clock < 5) {
        _clock++;
        return;
    }
    _clock = 0;

    // Only scan fast input sources because we don't want to waste any more time than necessary
    // on the input viewer and we can't afford to read from something like a Nunchuk twice.
    _report[0] = ASCII_BIT(_inputs.mb1);
    _report[1] = ASCII_BIT(_inputs.rf6);
    _report[2] = ASCII_BIT(_inputs.rf2);
    _report[3] = ASCII_BIT(_inputs.rf1);
    _report[4] = ASCII_BIT(_inputs.rt1);
    _report[5] = ASCII_BIT(_inputs.lf4);
    _report[6] = ASCII_BIT(_inputs.rf5);
    _report[7] = ASCII_BIT(_inputs.rf2);
    _report[8] = ASCII_BIT(_inputs.rf4);
    _report[9] = ASCII_BIT(_inputs.lf2);
    _report[10] = ASCII_BIT(_inputs.lf1);
    _report[11] = ASCII_BIT(_inputs.lf3);
    _report[12] = ASCII_BIT(_inputs.lt1);
    _report[13] = ASCII_BIT(_inputs.lt2);
    _report[14] = ASCII_BIT(_inputs.rt3);
    _report[15] = ASCII_BIT(_inputs.rt5);
    _report[16] = ASCII_BIT(_inputs.rt4);
    _report[17] = ASCII_BIT(_inputs.rt2);
    _report[18] = ASCII_BIT(_inputs.rf7);
    _report[19] = ASCII_BIT(_inputs.rf8);
    _report[20] = ASCII_BIT(false);
    _report[21] = ASCII_BIT(false);
    _report[22] = ASCII_BIT(false);
    _report[23] = ASCII_BIT(true);
    _report[24] = '\n';

    serial::write(_report, 25);
}