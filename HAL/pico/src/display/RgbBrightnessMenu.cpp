#include "display/RgbBrightnessMenu.hpp"

#include "comms/IntegratedDisplay.hpp"

RgbBrightnessMenu::RgbBrightnessMenu(Config &config) : _config(config) {}

DisplayModeId RgbBrightnessMenu::GetId() {
    return DISPLAY_MODE_RGB_BRIGHTNESS;
}

void RgbBrightnessMenu::HandleControls(
    IntegratedDisplay *instance,
    const DisplayControls &controls,
    Button button
) {
    // const DisplayControls &controls = instance->_controls;
    if (button == controls.up) {
        _config.rgb_brightness++;
    } else if (button == controls.down) {
        _config.rgb_brightness--;
    } else if (button == controls.back) {
        instance->SetDisplayMode(DISPLAY_MODE_CONFIG);
    }
}

void RgbBrightnessMenu::UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display) {
    uint8_t font_width = instance->font_width;
    uint8_t font_height = instance->font_height;

    // Current brightness value.
    display.setCursor(display.width() / 2 - (font_width * 3) / 2, display.height() / 2);
    display.printf("%3d", _config.rgb_brightness);

    // Display control hints.
    display.setCursor(5, display.height() - font_height);
    display.print("Back");
    display.setCursor(45, display.getCursorY());
    display.print("-");
    display.setCursor(display.width() - 45 - font_width, display.getCursorY());
    display.print("+");
}