#include "comms/IntegratedDisplay.hpp"

#include "core/Persistence.hpp"
#include "core/config_utils.hpp"
#include "util/state_util.hpp"

#define DEFAULT_COLOR 1

IntegratedDisplay::IntegratedDisplay(
    InputState &inputs,
    Adafruit_GFX &display,
    void (*clear_display)(),
    void (*update_display)(),
    const DisplayControls controls,
    const InputViewerButton *input_viewer_buttons,
    const size_t input_viewer_buttons_count,
    Config &config,
    CommunicationBackendId backend_id,
    CommunicationBackend **backends,
    size_t backends_count
)
    : CommunicationBackend(inputs, nullptr, 0),
      _display(display),
      _clear_display(clear_display),
      _update_display(update_display),
      _controls(controls),
      _controls_array{ controls.back, controls.down, controls.up, controls.enter },
      _input_viewer_buttons(input_viewer_buttons),
      _input_viewer_buttons_count(input_viewer_buttons_count),
      _config(config),
      _backend_id(backend_id),
      _backends(backends),
      _backends_count(backends_count) {
    _display.setTextSize(1);
    _display.setTextColor(DEFAULT_COLOR);

    /* Build default USB backends page */
    MenuPage::MenuItem *usb_backend_options =
        new MenuPage::MenuItem[config.communication_backend_configs_count];

    size_t usb_backend_options_count = 0;
    for (size_t i = 0; i < config.communication_backend_configs_count; i++) {
        CommunicationBackendConfig &backend_config = config.communication_backend_configs[i];
        MenuPage::MenuItem &current_option = usb_backend_options[usb_backend_options_count];

        if (backend_config.backend_id != COMMS_BACKEND_XINPUT &&
            backend_config.backend_id != COMMS_BACKEND_DINPUT &&
            backend_config.backend_id != COMMS_BACKEND_NINTENDO_SWITCH) {
            continue;
        }

        strlcpy(
            current_option.text,
            backend_name(backend_config.backend_id),
            sizeof(current_option.text)
        );
        current_option.key = i;
        current_option.action = &SetDefaultUsbBackend;
        usb_backend_options_count++;
    }

    _usb_backends_page = {
        .items = usb_backend_options,
        .items_count = usb_backend_options_count,
    };

    /* Build gamemodes page */
    MenuPage::MenuItem *gamemode_options = new MenuPage::MenuItem[config.game_mode_configs_count];

    size_t gamemode_options_count = 0;
    for (size_t i = 0; i < config.game_mode_configs_count; i++) {
        GameModeConfig &mode_config = config.game_mode_configs[i];
        MenuPage::MenuItem &current_option = gamemode_options[gamemode_options_count];

        // Don't show keyboard modes as a gamemode option unless using DInputBackend.
        if (_backend_id != COMMS_BACKEND_DINPUT && mode_config.mode_id == MODE_KEYBOARD) {
            continue;
        }

        if (strnlen(mode_config.name, sizeof(mode_config.name)) > 0) {
            strlcpy(current_option.text, mode_config.name, sizeof(current_option.text));
        } else {
            strlcpy(
                current_option.text,
                gamemode_name(mode_config.mode_id),
                sizeof(current_option.text)
            );
        }
        current_option.key = i;
        current_option.action = &SetDefaultMode;
        gamemode_options_count++;
    }

    _gamemode_options_page = {
        .items = gamemode_options,
        .items_count = gamemode_options_count,
    };

    /* Build SOCD types page */
    static MenuPage::MenuItem socd_options[_SocdType_MAX] = {};
    for (uint8_t socd_type = SOCD_NEUTRAL; socd_type < _SocdType_ARRAYSIZE; socd_type++) {
        MenuPage::MenuItem &current_option = socd_options[socd_type - 1];
        strlcpy(current_option.text, socd_name((SocdType)socd_type), sizeof(current_option.text));
        current_option.key = socd_type;
        current_option.action = &SetSocdType;
    }

    static MenuPage socd_page = {
        .items = socd_options,
        .items_count = sizeof(socd_options) / sizeof(MenuPage::MenuItem),
    };

    /* Build top-level page */
    // clang-format off
    static const MenuPage::MenuItem top_level_items[] = {
        {
            .text = "Gamemode",
            .page = &_gamemode_options_page,
        },
        {
            .text = "Default USB Mode",
            .page = &_usb_backends_page,
        },
        {
            .text = "SOCD Mode",
            .page = &socd_page,
        },
        {
            .text = "RGB Brightness",
            .action = [](IntegratedDisplay *instance, Config &config, uint8_t key) {
                instance->_display_mode = DISPLAY_MODE_RGB_BRIGHTNESS;
            },
        },
        {
            .text = "Return",
            .action = [](IntegratedDisplay *instance, Config &config, uint8_t key) {
                instance->_display_mode = DISPLAY_MODE_VIEWER;
            },
        },
        {
            .text = "Save changes",
            .action = [](IntegratedDisplay *instance, Config &config, uint8_t key) {
                persistence.SaveConfig(config);
                reboot_firmware();
            },
        },
        {
            .text = "Discard changes",
            .action = [](IntegratedDisplay *instance, Config &config, uint8_t key) {
                reboot_firmware();
            },
        }
    };
    // clang-format on

    _top_level_page = {
        .items = top_level_items,
        .items_count = sizeof(top_level_items) / sizeof(MenuPage::MenuItem),
    };

    _usb_backends_page.parent = &_top_level_page;
    _gamemode_options_page.parent = &_top_level_page;
    socd_page.parent = &_top_level_page;
}

IntegratedDisplay::~IntegratedDisplay() {
    delete[] _usb_backends_page.items;
    delete[] _gamemode_options_page.items;

    _clear_display();
    _update_display();
}

void IntegratedDisplay::SetGameMode(ControllerMode *gamemode) {
    GameModeId mode_id = gamemode->GetConfig().mode_id;
    strlcpy(_gamemode_text, gamemode_name(mode_id), sizeof(_gamemode_text));
    CommunicationBackend::SetGameMode(gamemode);
}

void IntegratedDisplay::UpdateOutputs() {
    if (!time_reached(_button_cooldown_end)) {
        return;
    }

    for (uint8_t i = 0; i < controls_count; i++) {
        Button button = _controls_array[i];
        if (get_button(_inputs.buttons, button)) {
            _button_cooldown_end = make_timeout_time_ms(button_cooldown_ms);
            switch (_display_mode) {
                case DISPLAY_MODE_VIEWER:
                    return HandleControlsViewerMode(button);
                case DISPLAY_MODE_CONFIG:
                    return HandleControlsConfigMode(button);
                case DISPLAY_MODE_RGB_BRIGHTNESS:
                    return HandleControlsRgbBrightnessMode(button);
            }
        }
    }
}

void IntegratedDisplay::HandleControlsViewerMode(Button button) {
    if (button == _controls.enter) {
        _display_mode = DISPLAY_MODE_CONFIG;
    }
}

void IntegratedDisplay::HandleControlsConfigMode(Button button) {
    if (_current_menu_page == nullptr) {
        _current_menu_page = &_top_level_page;
        _current_menu_offset = 0;
        return;
    }

    if (button == _controls.up) {
        _highlighted_menu_item = max(0, _highlighted_menu_item - 1);
    } else if (button == _controls.down) {
        _highlighted_menu_item =
            min(_current_menu_page->items_count - 1, _highlighted_menu_item + 1);
    } else if (button == _controls.enter) {
        // Bounds check.
        if (_highlighted_menu_item > _current_menu_page->items_count) {
            _highlighted_menu_item = 0;
            return;
        }

        const MenuPage::MenuItem &selected_item = _current_menu_page->items[_highlighted_menu_item];

        // If there's an action defined, perform it.
        if (selected_item.action != nullptr) {
            selected_item.action(this, _config, selected_item.key);

            // Go back up a level.
            if (_current_menu_page->parent != nullptr) {
                _current_menu_page = _current_menu_page->parent;
                _current_menu_offset = 0;
            }
            _highlighted_menu_item = 0;
            return;
        }

        // If there's a child page defined, drill into it.
        if (selected_item.page != nullptr) {
            _current_menu_page = selected_item.page;
            _current_menu_offset = 0;
            _highlighted_menu_item = 0;
            return;
        }
    } else if (button == _controls.back) {
        // If at top-level page, go back to input viewer.
        if (_current_menu_page->parent == nullptr) {
            _display_mode = DISPLAY_MODE_VIEWER;
            return;
        }

        _current_menu_page = _current_menu_page->parent;
        _highlighted_menu_item = 0;
    }
}

void IntegratedDisplay::HandleControlsRgbBrightnessMode(Button button) {
    if (button == _controls.up) {
        _config.rgb_brightness++;
    } else if (button == _controls.down) {
        _config.rgb_brightness--;
    } else if (button == _controls.back) {
        _display_mode = DISPLAY_MODE_CONFIG;
    }
}

void IntegratedDisplay::SendReport() {
    UpdateOutputs();

    _clear_display();

    if (_display_mode == DISPLAY_MODE_VIEWER) {
        /* Gamemode text */
        _display.setCursor(0, 0);
        _display.print(_gamemode_text);

        /* Backend text */
        const char *backend_text = backend_name(_backend_id);
        _display.setCursor(_display.width() - (strlen(backend_text) * font_width), 0);
        _display.print(backend_name(_backend_id));

        /* Input display */
        for (size_t i = 0; i < _input_viewer_buttons_count; i++) {
            InputViewerButton mapping = _input_viewer_buttons[i];
            if (get_button(_inputs.buttons, mapping.button)) {
                _display
                    .fillCircle(mapping.center_x, mapping.center_y, mapping.radius, DEFAULT_COLOR);
            } else {
                _display
                    .drawCircle(mapping.center_x, mapping.center_y, mapping.radius, DEFAULT_COLOR);
            }
        }
    } else if (_display_mode == DISPLAY_MODE_CONFIG) {
        /* Menu */
        if (_highlighted_menu_item - _current_menu_offset > max_visible_lines - 1) {
            _current_menu_offset++;
        } else if (_highlighted_menu_item < _current_menu_offset) {
            _current_menu_offset--;
        }
        uint8_t last_item_to_display =
            min(_current_menu_page->items_count - _current_menu_offset, max_visible_lines + 1);
        for (size_t i = 0; i < last_item_to_display; i++) {
            if (i + _current_menu_offset == _highlighted_menu_item) {
                _display.setCursor(0, i * (font_height + padding));
                _display.print(highlight_string);
            }
            _display.setCursor(font_width + padding, i * (font_height + padding));
            _display.print(_current_menu_page->items[i + _current_menu_offset].text);
        }
    } else if (_display_mode == DISPLAY_MODE_RGB_BRIGHTNESS) {
        // Current brightness value.
        _display.setCursor(_display.width() / 2 - (font_width * 3) / 2, _display.height() / 2);
        _display.printf("%3d", _config.rgb_brightness);

        // Display control hints.
        _display.setCursor(5, _display.height() - font_height);
        _display.print("Back");
        _display.setCursor(45, _display.getCursorY());
        _display.print("-");
        _display.setCursor(_display.width() - 45 - font_width, _display.getCursorY());
        _display.print("+");
    }

    _update_display();
}

void IntegratedDisplay::SetDefaultMode(
    IntegratedDisplay *instance,
    Config &config,
    uint8_t mode_config_index
) {
    if (mode_config_index < 0 || mode_config_index >= config.game_mode_configs_count) {
        return;
    }

    // Overwrite default game mode for all backend configs.
    for (size_t i = 0; i < config.communication_backend_configs_count; i++) {
        config.communication_backend_configs[i].default_mode_config = mode_config_index + 1;
    }

    // Update mode for all backends.
    for (size_t i = 0; i < instance->_backends_count; i++) {
        set_mode(
            instance->_backends[i],
            config.game_mode_configs[mode_config_index],
            config.keyboard_modes,
            config.keyboard_modes_count
        );
    }
}

void IntegratedDisplay::SetDefaultUsbBackend(
    IntegratedDisplay *instance,
    Config &config,
    uint8_t backend_config_index
) {
    if (backend_config_index < 0 ||
        backend_config_index >= config.communication_backend_configs_count) {
        return;
    }
    config.default_usb_backend_config = backend_config_index + 1;
}

void IntegratedDisplay::SetSocdType(
    IntegratedDisplay *instance,
    Config &config,
    uint8_t socd_type
) {
    if (socd_type <= SOCD_UNSPECIFIED || socd_type > _SocdType_MAX) {
        return;
    }

    // Overwrite SOCD type for all SOCD pairs of current gamemode's config.
    GameModeConfig &mode_config = instance->_gamemode->GetConfig();
    for (size_t i = 0; i < mode_config.socd_pairs_count; i++) {
        mode_config.socd_pairs[i].socd_type = (SocdType)socd_type;
    }
}
