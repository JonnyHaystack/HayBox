#include "display/MenuButtonHints.hpp"

#include "comms/IntegratedDisplay.hpp"
#include "core/config_utils.hpp"
#include "util/state_util.hpp"

MenuButtonHints::MenuButtonHints(const CommunicationBackendId backend_id)
    : _backend_id(backend_id) {}

DisplayModeId MenuButtonHints::GetId() {
    return DISPLAY_MODE_BUTTON_HINTS;
}

void MenuButtonHints::HandleControls(
    IntegratedDisplay *instance,
    const DisplayControls &controls,
    Button button
) {
    if (button == controls.back) {
        instance->SetDisplayMode(DISPLAY_MODE_CONFIG);
    }
}

void MenuButtonHints::UpdateDisplay(IntegratedDisplay *instance, Adafruit_GFX &display) {
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

    /* Button hints */
    uint8_t old_rotation = display.getRotation();
    display.setRotation(3);

    display.setCursor(0, 0);

    display.println("Menu\n");
    display.println("Start\n");
    display.println("Back\n");
    display.println("Home\n");
    display.println("Capt.\n");
    display.println("L3\n");
    display.println("R3\n");

    display.setRotation(old_rotation);
}