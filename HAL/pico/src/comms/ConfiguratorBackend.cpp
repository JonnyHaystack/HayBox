/*
 * This file is part of HayBox
 * Copyright (C) 2023 Jonathan Haylett
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
#include "reboot.hpp"
#include "serial.hpp"

#include <pb_decode.h>
#include <pb_encode.h>

ConfiguratorBackend::ConfiguratorBackend(
    InputState &inputs,
    InputSource **input_sources,
    size_t input_source_count,
    Config &config
)
    : CommunicationBackend(inputs, input_sources, input_source_count),
      _in(Serial),
      _out(Serial),
      _config(config) {
    serial::init(115200);
    _persistence = new Persistence();
}

ConfiguratorBackend::~ConfiguratorBackend() {
    serial::close();
    delete _persistence;
}

void ConfiguratorBackend::SendReport() {
    size_t packet_len = ReadPacket(_cmd_buffer, sizeof(_cmd_buffer));
    Command command = (Command)_cmd_buffer[0];
    switch (command) {
        case CMD_GET_DEVICE_INFO:
            HandleGetDeviceInfo();
            break;
        case CMD_GET_CONFIG:
            HandleGetConfig();
            break;
        case CMD_SET_CONFIG:
            HandleSetConfig(_cmd_buffer, packet_len);
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
}

size_t ConfiguratorBackend::ReadPacket(uint8_t *buffer, size_t max_len) {
    size_t bytes_read = 0;
    while (!Serial.available()) {
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

    uint8_t buffer[sizeof(DeviceInfo)];
    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&ostream, DeviceInfo_fields, &device_info)) {
        char errmsg[] = "Failed to encode device info";
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, sizeof(errmsg));
        return false;
    }

    return WritePacket(CMD_SET_DEVICE_INFO, buffer, ostream.bytes_written);
}

bool ConfiguratorBackend::HandleGetConfig() {
    size_t config_size = _persistence->LoadConfigRaw(_cmd_buffer, sizeof(_cmd_buffer));
    return WritePacket(CMD_SET_CONFIG, _cmd_buffer, config_size);
}

bool ConfiguratorBackend::HandleSetConfig(uint8_t *buffer, size_t len) {
    pb_istream_t istream = pb_istream_from_buffer(&buffer[1], len - 1);

    if (!pb_decode(&istream, Config_fields, &_config)) {
        char errmsg[] = "Failed to decode config";
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, sizeof(errmsg));
        return false;
    }

    if (!_persistence->SaveConfig(_config)) {
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