#include "display/InputDisplay.hpp"

#include "comms/IntegratedDisplay.hpp"
#include "core/config_utils.hpp"
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
    if (button == controls.enter) {
        instance->SetDisplayMode(DISPLAY_MODE_CONFIG);
    }
}

void InputDisplay::UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display) {
    InputState &inputs = instance->GetInputs();
    uint8_t font_width = instance->font_width;
    uint8_t color = instance->default_color;

    /* Gamemode text */
    const char *gamemode_text = gamemode_name(instance->CurrentGameMode()->GetConfig().mode_id);
    display.setCursor(0, 0);
    display.print(gamemode_text);

    /* Backend text */
    const char *backend_text = backend_name(_backend_id);
    display.setCursor(display.width() - (strlen(backend_text) * font_width), 0);
    display.print(backend_name(_backend_id));

    /* Input display */
    for (size_t i = 0; i < _input_viewer_buttons_count; i++) {
        InputViewerButton mapping = _input_viewer_buttons[i];
        if (get_button(inputs.buttons, mapping.button)) {
            display.fillCircle(mapping.center_x, mapping.center_y, mapping.radius, color);
        } else {
            display.drawCircle(mapping.center_x, mapping.center_y, mapping.radius, color);
        }
    }
}