#include "input/NunchukInput.hpp"

#include "gpio.hpp"

#include <ArduinoNunchuk.hpp>
#include <Wire.h>

NunchukInput::NunchukInput(int detect_pin) {
    delay(50);

    if (detect_pin > -1) {
        gpio::init_pin(detect_pin, gpio::GpioMode::GPIO_INPUT_PULLUP);
        if (gpio::read_digital(detect_pin)) {
            _nunchuk = nullptr;
            return;
        }
    }

    _nunchuk = new ArduinoNunchuk(Wire);

    if (!_nunchuk->init() || !_nunchuk->update()) {
        // If a Nunchuk isn't connected we don't want i2c to stay enabled because it might interfere
        // with inputs if i2c pins are also used for buttons. The destructor of ArduinoNunchuk
        // ends the i2c communication.
        delete _nunchuk;
        _nunchuk = nullptr;
    }
}

NunchukInput::~NunchukInput() {
    delete _nunchuk;
}

InputScanSpeed NunchukInput::ScanSpeed() {
    return InputScanSpeed::SLOW;
}

void NunchukInput::UpdateInputs(InputState &inputs) {
    if (_nunchuk != nullptr && _nunchuk->update()) {
        inputs.nunchuk_connected = true;
        inputs.nunchuk_x = _nunchuk->stickX();
        inputs.nunchuk_y = _nunchuk->stickY();
        inputs.nunchuk_c = _nunchuk->buttonC();
        inputs.nunchuk_z = _nunchuk->buttonZ();
    }
}
