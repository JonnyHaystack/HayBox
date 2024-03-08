#include "comms/IntegratedDisplay.hpp"

#include "core/Persistence.hpp"
#include "core/config_utils.hpp"
#include "util/state_util.hpp"

IntegratedDisplay::IntegratedDisplay(
    InputState &inputs,
    Adafruit_GFX &display,
    void (*clear_display)(),
    void (*update_display)(),
    const DisplayControls controls,
    DisplayMode **display_modes,
    size_t display_modes_count
)
    : CommunicationBackend(inputs, nullptr, 0),
      _display(display),
      _clear_display(clear_display),
      _update_display(update_display),
      _controls(controls),
      _controls_array{ controls.back, controls.down, controls.up, controls.enter },
      _display_modes(display_modes),
      _display_modes_count(display_modes_count) {
    _display.setTextSize(1);
    _display.setTextColor(default_color);
}

IntegratedDisplay::~IntegratedDisplay() {
    _clear_display();
    _update_display();
}

void IntegratedDisplay::SendReport() {
    DisplayMode *active_mode = GetActiveDisplayMode();
    if (active_mode == nullptr) {
        return;
    }

    _clear_display();
    active_mode->UpdateDisplay(this, _display);
    _update_display();

    // This is done *after* display update so any side effects of HandleControls are not shown
    // without active_mode also being up-to-date.
    HandleControls(active_mode);
}

void IntegratedDisplay::HandleControls(DisplayMode *active_mode) {
    if (!time_reached(_button_cooldown_end)) {
        return;
    }

    for (uint8_t i = 0; i < controls_count; i++) {
        Button button = _controls_array[i];
        if (get_button(_inputs.buttons, button)) {
            _button_cooldown_end = make_timeout_time_ms(button_cooldown_ms);
            active_mode->HandleControls(this, _controls, button);
            return;
        }
    }
}

void IntegratedDisplay::SetDisplayMode(DisplayModeId display_mode) {
    _display_mode = display_mode;
}

DisplayMode *IntegratedDisplay::GetActiveDisplayMode() {
    for (size_t i = 0; i < _display_modes_count; i++) {
        if (_display_modes[i]->GetId() == _display_mode) {
            return _display_modes[i];
        }
    }
    return nullptr;
}