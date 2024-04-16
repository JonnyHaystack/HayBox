#ifndef _INPUT_PCA9671INPUT_HPP
#define _INPUT_PCA9671INPUT_HPP

#include "core/InputSource.hpp"

#include <PCF8575.h>
#include <Wire.h>
#include <config.pb.h>

typedef struct {
    Button button;
    uint8_t bit;
} Pca9671ButtonMapping;

class Pca9671Input : public InputSource {
  public:
    Pca9671Input(
        const Pca9671ButtonMapping *button_mappings,
        size_t button_count,
        TwoWire &wire = Wire,
        int sda_pin = -1,
        int scl_pin = -1,
        uint8_t i2c_addr = 0x20
    );
    InputScanSpeed ScanSpeed();
    void UpdateInputs(InputState &inputs);

  protected:
    const Pca9671ButtonMapping *_button_mappings;
    size_t _button_count;
    PCF8575 _pcf;

  private:
    virtual void UpdateButtonState(InputState &inputs, size_t button_mapping_index, bool pressed);
};

#endif