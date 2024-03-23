/*
 * This file is part of HayBox
 * Copyright (C) 2024 Jonathan Haylett
 *
 * HayBox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software. If not, see <http://www.gnu.org/licenses/>.
 */

#include "comms/ConfiguratorBackend.hpp"

#include "core/InputSource.hpp"
#include "core/Persistence.hpp"
#include "reboot.hpp"

#include <pb_arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>

ConfiguratorBackend::ConfiguratorBackend(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config,
    Stream &stream
)
    : CommunicationBackend(inputs, input_sources, input_source_count),
      _in(stream),
      _out(stream),
      _base_stream(stream),
      _config(config) {}

CommunicationBackendId ConfiguratorBackend::BackendId() {
    return COMMS_BACKEND_CONFIGURATOR;
}

void ConfiguratorBackend::SendReport() {
    int data = ReadByte();
    if (data < 0) {
        if (data == _in.EOP) {
            SkipToNextPacket();
        }
        return;
    }

    Command command = (Command)data;
    switch (command) {
        case CMD_GET_DEVICE_INFO:
            HandleGetDeviceInfo();
            break;
        case CMD_GET_CONFIG:
            HandleGetConfig();
            break;
        case CMD_SET_CONFIG:
            HandleSetConfig();
            break;
        case CMD_REBOOT_FIRMWARE:
            reboot_firmware();
            break;
        case CMD_REBOOT_BOOTLOADER:
            reboot_bootloader();
            break;
        case CMD_UNSPECIFIED:
        default:
            HandleUnknownCommand(command);
            break;
    }

    SkipToNextPacket();
}

size_t ConfiguratorBackend::ReadPacket(uint8_t *buffer, size_t max_len) {
    size_t bytes_read = 0;
    while (!_in.available()) {
        delay(1);
    }
    while (true) {
        int result = _in.read();
        if (result == _in.EOF) {
            continue;
        }
        if (result == _in.EOP) {
            break;
        }
        buffer[bytes_read++] = result;
        if (bytes_read >= max_len) {
            _in.next();
            return bytes_read;
        }
    }
    _in.next();

    return bytes_read;
}

int ConfiguratorBackend::ReadByte() {
    if (!_base_stream.available()) {
        return -1;
    }
    return _in.read();
}

void ConfiguratorBackend::SkipToNextPacket() {
    _in.next();
}

bool ConfiguratorBackend::WritePacket(Command command_id, uint8_t *buffer, size_t len) {
    _out.write((uint8_t)command_id);
    for (size_t i = 0; i < len; i++) {
        _out.write(buffer[i]);
    }
    return _out.end();
}

bool ConfiguratorBackend::HandleGetDeviceInfo() {
    static const DeviceInfo device_info = {
        FIRMWARE_NAME,
        FIRMWARE_VERSION,
        DEVICE_NAME,
    };

    // Ensure device info encodes correctly.
    size_t size;
    if (!pb_get_encoded_size(&size, DeviceInfo_fields, &device_info)) {
        char errmsg[] = "Failed to encode device info";
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, sizeof(errmsg));
        return false;
    }

    _out.write(CMD_SET_DEVICE_INFO);
    pb_ostream_t ostream = as_pb_ostream(_out);
    pb_encode(&ostream, DeviceInfo_fields, &device_info);
    return _out.end();
}

bool ConfiguratorBackend::HandleGetConfig() {
    if (!persistence.CheckSavedConfig()) {
        char errmsg[] = "Config file is invalid";
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, sizeof(errmsg));
        return false;
    }

    // Write raw config data to output stream.
    _out.write(CMD_SET_CONFIG);
    persistence.LoadConfigRaw(_out, false);
    return _out.end();
}

bool ConfiguratorBackend::HandleSetConfig() {
    // Reset config defaults first, so config is completely replaced rather than merged.
    _config = Config_init_default;

    pb_istream_t istream = as_pb_istream(_in);
    if (!pb_decode(&istream, Config_fields, &_config)) {
        char errmsg[100];
        size_t errmsg_len =
            snprintf(errmsg, sizeof(errmsg), "Failed to decode config: %s", istream.errmsg);
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, errmsg_len);

        // Restore old config.
        persistence.LoadConfig(_config);
        return false;
    }

    if (_config.default_backend_config > _config.communication_backend_configs_count) {
        char errmsg[75];
        size_t errmsg_len = snprintf(
            errmsg,
            sizeof(errmsg),
            "Default backend ID is %d but only %d backend configs are defined",
            (uint8_t)_config.default_backend_config,
            (uint8_t)_config.communication_backend_configs_count
        );
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, errmsg_len);
        return false;
    }

    for (size_t i = 0; i < _config.communication_backend_configs_count; i++) {
        uint8_t default_mode_id = _config.communication_backend_configs[i].default_mode_config;
        if (default_mode_id > _config.game_mode_configs_count) {
            char errmsg[75];
            size_t errmsg_len = snprintf(
                errmsg,
                sizeof(errmsg),
                "Default mode ID is %d for backend %d but only %d modes are defined",
                (uint8_t)default_mode_id,
                (uint8_t)i + 1,
                (uint8_t)_config.game_mode_configs_count
            );
            WritePacket(CMD_ERROR, (uint8_t *)errmsg, errmsg_len);
            return false;
        }
    }

    for (size_t i = 0; i < _config.game_mode_configs_count; i++) {
        uint8_t keyboard_mode_id = _config.game_mode_configs[i].keyboard_mode_config;
        if (keyboard_mode_id > _config.keyboard_modes_count) {
            char errmsg[85];
            size_t errmsg_len = snprintf(
                errmsg,
                sizeof(errmsg),
                "Keyboard mode ID %d is for game mode %d but only %d keyboard modes are defined",
                (uint8_t)keyboard_mode_id,
                (uint8_t)i + 1,
                (uint8_t)_config.keyboard_modes_count
            );
            WritePacket(CMD_ERROR, (uint8_t *)errmsg, errmsg_len);
            return false;
        }

        uint8_t custom_mode_id = _config.game_mode_configs[i].custom_mode_config;
        if (custom_mode_id > _config.custom_modes_count) {
            char errmsg[85];
            size_t errmsg_len = snprintf(
                errmsg,
                sizeof(errmsg),
                "Custom mode ID %d is for game mode config %d but only %d custom modes are defined",
                (uint8_t)custom_mode_id,
                (uint8_t)i + 1,
                (uint8_t)_config.custom_modes_count
            );
            WritePacket(CMD_ERROR, (uint8_t *)errmsg, errmsg_len);
            return false;
        }
    }

    if (!persistence.SaveConfig(_config)) {
        char errmsg[] = "Failed to save config to memory";
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, sizeof(errmsg));
        return false;
    }

    WritePacket(CMD_SUCCESS, nullptr, 0);
    return true;
}

bool ConfiguratorBackend::HandleUnknownCommand(Command command) {
    char errmsg[50];
    size_t errmsg_len =
        snprintf(errmsg, sizeof(errmsg), "Unknown command ID: %d", (uint8_t)command);
    return WritePacket(CMD_ERROR, (uint8_t *)errmsg, errmsg_len);
}