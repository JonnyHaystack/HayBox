#ifndef _COMMS_NEOPIXELBACKEND_HPP
#define _COMMS_NEOPIXELBACKEND_HPP

#include "core/CommunicationBackend.hpp"

#include <Adafruit_NeoPixel.h>

class NeoPixelBackend : public CommunicationBackend {
  public:
    NeoPixelBackend(
        InputState &inputs,
        InputSource **input_sources,
        size_t input_source_count,
        uint8_t control_pin,
        uint16_t pixel_count
    );
    ~NeoPixelBackend();
    virtual void SendReport();

  protected:
    static constexpr uint8_t _max_pixels_per_cycle = 5;
    const uint8_t _cycles_per_show = _pixel_count / _max_pixels_per_cycle;

    Adafruit_NeoPixel _strip;
    uint8_t _current_cycle = 0;

  private:
    const uint16_t _pixel_count;
};

#endif