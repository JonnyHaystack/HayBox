#include "display/InputDisplay.hpp"

#include "comms/IntegratedDisplay.hpp"
#include "util/config_util.hpp"
#include "util/state_util.hpp"

InputDisplay::InputDisplay(
    InputViewerButton *input_viewer_buttons,
    size_t input_viewer_buttons_count,
    const CommunicationBackendId backend_id
)
    : _input_viewer_buttons(input_viewer_buttons),
      _input_viewer_buttons_count(input_viewer_buttons_count),
      _backend_id(backend_id) {}

DisplayModeId InputDisplay::GetId() {
    return DISPLAY_MODE_VIEWER;
}

void InputDisplay::HandleControls(
    IntegratedDisplay *instance,
    const DisplayControls &controls,
    Button button
) {
    if (button == controls.back) {
        instance->SetDisplayMode(DISPLAY_MODE_CONFIG);
    }
}

void InputDisplay::UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display) {
    InputState &inputs = instance->GetInputs();
    uint8_t font_width = instance->font_width;
    uint8_t color = instance->default_color;

    /* Gamemode text */
    display.setCursor(0, 0);
    if (instance->CurrentGameMode() != nullptr) {
        const GameModeConfig &mode_config = *instance->CurrentGameMode()->GetConfig();
        if (strnlen(mode_config.name, sizeof(mode_config.name)) > 0) {
            display.print(mode_config.name);
        } else {
            display.print(gamemode_name(mode_config.mode_id));
        }
    }

    /* Backend text */
    const char *backend_text = backend_name(_backend_id);
    display.setCursor(display.width() - (strlen(backend_text) * font_width), 0);
    display.print(backend_name(_backend_id));

    /* Input display */
    if (_input_viewer_buttons == nullptr) {
        return;
    }
    for (size_t i = 0; i < _input_viewer_buttons_count; i++) {
        InputViewerButton mapping = _input_viewer_buttons[i];
        if (get_button(inputs.buttons, mapping.button)) {
            display.fillCircle(mapping.center_x, mapping.center_y, mapping.radius, color);
        } else {
            display.drawCircle(mapping.center_x, mapping.center_y, mapping.radius, color);
        }
    }
}

void InputDisplay::UpdateButtonLayout(
    InputViewerButton *input_viewer_buttons,
    size_t input_viewer_buttons_count
) {
    _input_viewer_buttons = input_viewer_buttons;
    _input_viewer_buttons_count = input_viewer_buttons_count;
}