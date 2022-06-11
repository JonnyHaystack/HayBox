#include "comms/B0XXInputViewer.hpp"

#include "core/InputSource.hpp"
#include "serial.hpp"

#define ASCII_BIT(x) (x ? '1' : '0');

B0XXInputViewer::B0XXInputViewer(InputSource **input_sources, size_t input_source_count)
    : CommunicationBackend(input_sources, input_source_count) {
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
    ScanInputs(InputScanSpeed::FAST);

    _report[0] = ASCII_BIT(_inputs.start);
    _report[1] = ASCII_BIT(_inputs.y);
    _report[2] = ASCII_BIT(_inputs.x);
    _report[3] = ASCII_BIT(_inputs.b);
    _report[4] = ASCII_BIT(_inputs.a);
    _report[5] = ASCII_BIT(_inputs.l);
    _report[6] = ASCII_BIT(_inputs.r);
    _report[7] = ASCII_BIT(_inputs.z);
    _report[8] = ASCII_BIT(_inputs.up);
    _report[9] = ASCII_BIT(_inputs.down);
    _report[10] = ASCII_BIT(_inputs.right);
    _report[11] = ASCII_BIT(_inputs.left);
    _report[12] = ASCII_BIT(_inputs.mod_x);
    _report[13] = ASCII_BIT(_inputs.mod_y);
    _report[14] = ASCII_BIT(_inputs.c_left);
    _report[15] = ASCII_BIT(_inputs.c_right);
    _report[16] = ASCII_BIT(_inputs.c_up);
    _report[17] = ASCII_BIT(_inputs.c_down);
    _report[18] = ASCII_BIT(_inputs.lightshield);
    _report[19] = ASCII_BIT(_inputs.midshield);
    _report[20] = ASCII_BIT(false);
    _report[21] = ASCII_BIT(false);
    _report[22] = ASCII_BIT(false);
    _report[23] = ASCII_BIT(true);
    _report[24] = '\n';

    serial::write(_report, 25);
}