#ifndef _COMMS_NEOPIXELBACKEND_HPP
#define _COMMS_NEOPIXELBACKEND_HPP

#include "core/CommunicationBackend.hpp"

#include <FastLED.h>
#include <config.pb.h>

template <uint8_t data_pin, int led_count> class NeoPixelBackend : public CommunicationBackend {
  public:
    NeoPixelBackend(
        InputState &inputs,
        InputSource **input_sources,
        size_t input_source_count,
        const Button *button_mappings,
        const RgbConfig *rgb_configs,
        const size_t rgb_configs_count,
        const uint8_t &brightness
    )
        : CommunicationBackend(inputs, input_sources, input_source_count),
          _button_mappings(button_mappings),
          _rgb_configs(rgb_configs),
          _rgb_configs_count(rgb_configs_count),
          _brightness(brightness) {
        FastLED.addLeds<NEOPIXEL, data_pin>(_leds, led_count);
        FastLED.setMaxPowerInVoltsAndMilliamps(5, 200);
    }

    ~NeoPixelBackend() { FastLED.clear(true); }

    virtual void SetGameMode(ControllerMode *gamemode) {
        // Clear current button colors.
        for (size_t i = 0; i < button_colors_count; i++) {
            _button_colors[i] = 0;
        }

        if (gamemode == nullptr || gamemode->GetConfig() == nullptr) {
            _config = nullptr;
            return;
        }

        uint8_t rgb_config_id = gamemode->GetConfig()->rgb_config;
        if (rgb_config_id == 0 || rgb_config_id > _rgb_configs_count) {
            _config = nullptr;
            return;
        }
        _config = &_rgb_configs[rgb_config_id - 1];

        // Build new mapping array to give O(n) lookup later.
        for (size_t i = 0; i < _config->button_colors_count; i++) {
            ButtonToColorMapping mapping = _config->button_colors[i];
            _button_colors[max(0, mapping.button - 1)] = mapping.color;
        }
    }

    virtual void SendReport() {
        for (int i = 0; i < led_count; i++) {
            Button button = this->_button_mappings[i];
            _leds[i] = _config != nullptr ? _button_colors[max(0, button - 1)] : 0;
        }
        FastLED.setBrightness(_brightness);
        FastLED.show();
    }

  protected:
    static constexpr size_t button_colors_count =
        sizeof(RgbConfig::button_colors) / sizeof(ButtonToColorMapping);

    const Button *_button_mappings;
    const RgbConfig *_rgb_configs;
    const size_t _rgb_configs_count;
    const RgbConfig *_config = nullptr;
    const uint8_t &_brightness;

    CRGB _leds[led_count];
    uint32_t _button_colors[button_colors_count];
};

#endif