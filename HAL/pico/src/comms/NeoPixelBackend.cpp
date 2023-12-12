#include "comms/NeoPixelBackend.hpp"

NeoPixelBackend::NeoPixelBackend(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    const Button *button_mappings,
    const RgbConfig *rgb_configs,
    size_t rgb_configs_count,
    uint8_t control_pin,
    uint16_t pixel_count
)
    : CommunicationBackend(inputs, input_sources, input_source_count),
      button_mappings(button_mappings),
      rgb_configs(rgb_configs),
      rgb_configs_count(rgb_configs_count),
      pixel_count(pixel_count),
      cycles_per_show(pixel_count / max_pixels_per_cycle),
      _strip(pixel_count, control_pin) {
    _strip.begin();
    _strip.setBrightness(100);
    if (_strip.canShow()) {
        _strip.show();
    }
}

NeoPixelBackend::~NeoPixelBackend() {
    _strip.clear();
}

void NeoPixelBackend::SetGameMode(ControllerMode *gamemode) {
    // Clear current button colors.
    for (size_t i = 0; i < button_colors_count; i++) {
        _button_colors[i] = 0;
    }

    // Reset current pixel bookmark.
    _current_pixel = 0;

    uint8_t rgb_config_id = gamemode->GetConfig().rgb_config;
    if (rgb_config_id == 0 || rgb_config_id > rgb_configs_count) {
        config = nullptr;
        return;
    }
    config = &rgb_configs[rgb_config_id - 1];

    // Build new mapping array to give O(n) lookup later.
    for (size_t i = 0; i < config->button_colors_count; i++) {
        ButtonToColorMapping mapping = config->button_colors[i];
        _button_colors[max(0, mapping.button - 1)] = mapping.color;
    }
}

void NeoPixelBackend::SendReport() {
    if (config == nullptr || !_strip.canShow()) {
        return;
    }

    uint8_t stop_pixel = min(pixel_count, _current_pixel + max_pixels_per_cycle);
    for (; _current_pixel < stop_pixel; _current_pixel++) {
        volatile Button button = this->button_mappings[_current_pixel];
        volatile uint32_t color = _button_colors[max(0, button - 1)];
        _strip.fill(color, _current_pixel, 1);
    }
    if (_current_pixel >= pixel_count) {
        _current_pixel = 0;
    }
    _strip.show();
}