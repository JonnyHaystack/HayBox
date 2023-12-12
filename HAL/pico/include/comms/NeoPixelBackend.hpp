#ifndef _COMMS_NEOPIXELBACKEND_HPP
#define _COMMS_NEOPIXELBACKEND_HPP

#include "core/CommunicationBackend.hpp"

#include <Adafruit_NeoPixel.h>
#include <config.pb.h>

class NeoPixelBackend : public CommunicationBackend {
  public:
    NeoPixelBackend(
        InputState &inputs,
        InputSource **input_sources,
        size_t input_source_count,
        const Button *button_mappings,
        const RgbConfig *rgb_configs,
        size_t rgb_configs_count,
        uint8_t control_pin,
        uint16_t pixel_count
    );
    ~NeoPixelBackend();
    virtual void SetGameMode(ControllerMode *gamemode);
    virtual void SendReport();

  protected:
    static constexpr uint8_t max_pixels_per_cycle = 10;
    static constexpr size_t button_colors_count =
        sizeof(RgbConfig::button_colors) / sizeof(ButtonToColorMapping);

    const Button *button_mappings;
    const RgbConfig *rgb_configs;
    const size_t rgb_configs_count;
    const uint16_t pixel_count;
    const uint8_t cycles_per_show;
    const RgbConfig *config = nullptr;

    Adafruit_NeoPixel _strip;
    size_t _current_pixel = 0;
    uint32_t _button_colors[button_colors_count];
};

#endif