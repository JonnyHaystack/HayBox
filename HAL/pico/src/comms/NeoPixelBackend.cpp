#include "comms/NeoPixelBackend.hpp"

NeoPixelBackend::NeoPixelBackend(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    uint8_t control_pin,
    uint16_t pixel_count
)
    : CommunicationBackend(inputs, input_sources, input_source_count),
      _strip(pixel_count, control_pin),
      _pixel_count(pixel_count),
      _cycles_per_show(pixel_count / _max_pixels_per_cycle) {
    _strip.begin();
    _strip.setBrightness(100);
    if (_strip.canShow()) {
        _strip.show();
    }
}

NeoPixelBackend::~NeoPixelBackend() {
    _strip.clear();
}

void NeoPixelBackend::SendReport() {
    if (!_strip.canShow()) {
        return;
    }
    uint8_t start_pixel = _current_cycle * _max_pixels_per_cycle;
    uint8_t pixels_to_fill = min(_max_pixels_per_cycle, _pixel_count - start_pixel);
    _strip.fill(Adafruit_NeoPixel::Color(30, 30, 30), start_pixel, pixels_to_fill);
    _strip.show();
    if (++_current_cycle >= _cycles_per_show) {
        _current_cycle = 0;
    }
}