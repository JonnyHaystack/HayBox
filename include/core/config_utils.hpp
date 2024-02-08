#ifndef _CORE_CONFIG_UTILS_HPP
#define _CORE_CONFIG_UTILS_HPP

#include "core/state.hpp"

#include <config.pb.h>

CommunicationBackendConfig backend_config_from_buttons(
    const InputState &inputs,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count
);

CommunicationBackendConfig backend_config_from_id(
    CommunicationBackendId backend_id,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count
);

uint8_t backend_config_id_from_backend_id(
    CommunicationBackendId backend_id,
    const CommunicationBackendConfig *backend_configs,
    size_t backend_configs_count
);

uint8_t mode_config_id_from_mode_id(
    GameModeId mode_id,
    const GameModeConfig *mode_configs,
    size_t mode_configs_count
);

constexpr const char *gamemode_name(GameModeId mode_id) {
    switch (mode_id) {
        case MODE_MELEE:
            return "Melee";
        case MODE_PROJECT_M:
            return "Project M";
        case MODE_ULTIMATE:
            return "Ultimate";
        case MODE_FGC:
            return "FGC";
        case MODE_RIVALS_OF_AETHER:
            return "Rivals";
        case MODE_KEYBOARD:
            return "Keyboard";
        default:
            return "Unknown";
    }
}

constexpr const char *backend_name(CommunicationBackendId backend_id) {
    switch (backend_id) {
        case COMMS_BACKEND_DINPUT:
            return "DInput";
        case COMMS_BACKEND_XINPUT:
            return "XInput";
        case COMMS_BACKEND_GAMECUBE:
            return "GameCube";
        case COMMS_BACKEND_N64:
            return "N64";
        case COMMS_BACKEND_NES:
            return "NES";
        case COMMS_BACKEND_SNES:
            return "SNES";
        case COMMS_BACKEND_NINTENDO_SWITCH:
            return "Switch";
        case COMMS_BACKEND_CONFIGURATOR:
            return "Configurator";
        default:
            return "Unknown";
    }
}

#endif