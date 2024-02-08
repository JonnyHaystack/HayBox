#include "comms/IntegratedDisplay.hpp"

#include "core/Persistence.hpp"
#include "core/config_utils.hpp"

IntegratedDisplay::IntegratedDisplay(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Adafruit_GFX &display,
    Config &config,
    CommunicationBackendId backend_id,
    CommunicationBackend **backends,
    size_t backends_count
)
    : CommunicationBackend(inputs, input_sources, input_source_count),
      _display(display),
      _config(config),
      _backend_id(backend_id),
      _backends(backends),
      _backends_count(backends_count) {
    _display.setTextSize(1);
    _display.setTextColor(WHITE);

    /* Build default USB backends page */
    MenuPage::MenuItem *usb_backend_options =
        new MenuPage::MenuItem[config.communication_backend_configs_count];

    size_t usb_backend_options_count = 0;
    for (size_t i = 0; i < config.communication_backend_configs_count; i++) {
        CommunicationBackendConfig &backend_config = config.communication_backend_configs[i];
        MenuPage::MenuItem &current_backend_option = usb_backend_options[usb_backend_options_count];

        if (backend_config.backend_id != COMMS_BACKEND_XINPUT &&
            backend_config.backend_id != COMMS_BACKEND_DINPUT &&
            backend_config.backend_id != COMMS_BACKEND_NINTENDO_SWITCH) {
            continue;
        }

        strlcpy(
            current_backend_option.text,
            backend_name(backend_config.backend_id),
            sizeof(current_backend_option.text)
        );
        current_backend_option.key = i;
        current_backend_option.action = &SetDefaultUsbBackend;
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
        MenuPage::MenuItem &current_gamemode_option = gamemode_options[gamemode_options_count];

        // Don't show keyboard modes as a gamemode option unless using DInputBackend.
        if (_backend_id != COMMS_BACKEND_DINPUT && mode_config.mode_id == MODE_KEYBOARD) {
            continue;
        }

        if (strnlen(mode_config.name, sizeof(mode_config.name)) > 0) {
            strlcpy(
                current_gamemode_option.text,
                mode_config.name,
                sizeof(current_gamemode_option.text)
            );
        } else {
            strlcpy(
                current_gamemode_option.text,
                gamemode_name(mode_config.mode_id),
                sizeof(current_gamemode_option.text)
            );
        }
        current_gamemode_option.key = i;
        current_gamemode_option.action = &SetDefaultMode;
        gamemode_options_count++;
    }

    _gamemode_options_page = {
        .items = gamemode_options,
        .items_count = gamemode_options_count,
    };

    /* Build top-level page */
    char top_level_item1_text[] = "Default Gamemode";
    char top_level_item2_text[] = "Default USB Mode";
    char top_level_item3_text[] = "Exit";

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
}

IntegratedDisplay::~IntegratedDisplay() {
    delete[] _usb_backends_page.items;
    delete[] _gamemode_options_page.items;

    _display.clearDisplay();
    _display.display();
}

void IntegratedDisplay::SetGameMode(ControllerMode *gamemode) {
    _gamemode = gamemode->GetConfig().mode_id;
    strlcpy(_gamemode_text, gamemode_name(_gamemode), sizeof(_gamemode_text));
}

void IntegratedDisplay::UpdateOutputs() {
    // Process inputs.
    if (!time_reached(_button_cooldown_end)) {
        return;
    }
    if (_current_menu_page == nullptr) {
        _current_menu_page = &_top_level_page;
        _current_menu_offset = 0;
        return;
    }

    if (_display_mode != DISPLAY_MODE_CONFIG) {
        if (_inputs.mb7) {
            _button_cooldown_end = make_timeout_time_ms(button_cooldown_ms);
            _display_mode = DISPLAY_MODE_CONFIG;
        }
        return;
    }

    // Handle Up button press.
    if (_inputs.mb6) {
        _button_cooldown_end = make_timeout_time_ms(button_cooldown_ms);

        _highlighted_menu_item = max(0, _highlighted_menu_item - 1);
        return;
    }

    // Handle Down button press.
    if (_inputs.mb5) {
        _button_cooldown_end = make_timeout_time_ms(button_cooldown_ms);

        _highlighted_menu_item =
            min(_current_menu_page->items_count - 1, _highlighted_menu_item + 1);
        return;
    }

    // Handle Select button press.
    if (_inputs.mb7) {
        _button_cooldown_end = make_timeout_time_ms(button_cooldown_ms);

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
    }

    // Handle Back button press.
    if (_inputs.mb4) {
        _button_cooldown_end = make_timeout_time_ms(button_cooldown_ms);

        if (_current_menu_page->parent == nullptr) {
            _display_mode = DISPLAY_MODE_VIEWER;
            return;
        }

        _current_menu_page = _current_menu_page->parent;
        _highlighted_menu_item = 0;
        return;
    }
}

void IntegratedDisplay::SendReport() {
    UpdateOutputs();

    _display.clearDisplay();

    if (_display_mode == DISPLAY_MODE_VIEWER) {
        /* Gamemode text */
        _display.setCursor(0, 0);
        _display.print(_gamemode_text);

        /* Backend text */
        const char *backend_text = backend_name(_backend_id);
        _display.setCursor(_display.width() - (strlen(backend_text) * 6), 0);
        _display.print(backend_name(_backend_id));
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
    }

    _display.display();
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
