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

#include "core/Persistence.hpp"

#include "stdlib.hpp"

#include <CRC32.h>
#include <EEPROM.h>
#include <pb_decode.h>
#include <pb_encode.h>

Config Persistence::_config;
uint8_t Persistence::_buffer[eeprom_size - config_offset];

Persistence::Persistence() {
    EEPROM.begin(eeprom_size);
}

Persistence::~Persistence() {
    EEPROM.end();
}

bool Persistence::SaveConfig(Config &config) {
    pb_ostream_t ostream = pb_ostream_from_buffer(_buffer, sizeof(_buffer));

    if (!pb_encode(&ostream, Config_fields, &config)) {
        return false;
    }

    ConfigHeader header = {
        .config_size = ostream.bytes_written,
        .config_crc = CRC32::calculate(_buffer, ostream.bytes_written),
    };

    // Store number of bytes of configuration data at offset 0.
    EEPROM.put(0, header);

    // Store the rest of the data directly after that.
    for (size_t i = 0; i < header.config_size; i++) {
        EEPROM.write(config_offset + i, _buffer[i]);
    }
    EEPROM.commit();

    return true;
}

bool Persistence::LoadConfig(Config &config) {
    size_t config_size = LoadConfigRaw(_buffer, sizeof(_buffer));

    // No valid config found.
    if (config_size == 0) {
        return false;
    }

    pb_istream_t istream = pb_istream_from_buffer(_buffer, config_size);

    // Return true if successfully decoded.
    if (pb_decode(&istream, Config_fields, &_config)) {
        config = _config;
        return true;
    }

    // Otherwise reset back to original config.
    return false;
}

size_t Persistence::LoadConfigRaw(uint8_t *buffer, size_t buffer_len) {
    // Get config header containing CRC32 checksum and number of stored bytes of config data from
    // offset 0 of EEPROM.
    ConfigHeader header;
    EEPROM.get(0, header);

    // Buffer can't be less than the size we expect to read.
    if (buffer_len < header.config_size) {
        return 0;
    }

    // Read config data from EEPROM into the buffer.
    size_t bytes_read;
    for (bytes_read = 0; bytes_read < header.config_size; bytes_read++) {
        buffer[bytes_read] = EEPROM.read(config_offset + bytes_read);
    }

    // If CRC32 checksum does not match, consider it as having failed to read.
    if (CRC32::calculate(buffer, bytes_read) != header.config_crc) {
        return 0;
    }

    // Return the number of bytes of config data read.
    return bytes_read;
}

Persistence persistence;