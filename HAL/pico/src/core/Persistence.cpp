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

#include "core/Persistence.hpp"

#include "stdlib.hpp"

#include <CRC32.h>
#include <LittleFS.h>
#include <pb_arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>

Persistence::Persistence() {
    LittleFS.begin();
}

Persistence::~Persistence() {
    LittleFS.end();
}

bool Persistence::SaveConfig(Config &config) {
    // Make sure config encodes correctly.
    size_t encoded_size;
    if (!pb_get_encoded_size(&encoded_size, Config_fields, &config)) {
        return false;
    }

    // Open file to store config data in.
    File config_file = LittleFS.open(config_filename, "w+");
    if (!config_file) {
        return false;
    }

    // Write empty header to start with.
    ConfigHeader header = { .config_size = 0, .config_crc = 0 };
    config_file.write((uint8_t *)&header, sizeof(ConfigHeader));

    // Encode Protobuf data directly into file body.
    pb_ostream_t ostream = as_pb_ostream(config_file);
    if (!pb_encode(&ostream, Config_fields, &config)) {
        config_file.close();
        return false;
    }

    // Calculate checksum.
    config_file.seek(config_offset);
    CRC32 crc;
    int value;
    while ((value = config_file.read()) != -1) {
        crc.update((uint8_t)value);
    }

    // Update header.
    header.config_size = ostream.bytes_written;
    header.config_crc = crc.finalize();
    config_file.seek(0);
    config_file.write((uint8_t *)&header, sizeof(ConfigHeader));

    // Persist changes.
    config_file.close();

    return true;
}

bool Persistence::LoadConfig(Config &config) {
    // Open file to load config data from.
    File config_file = LittleFS.open(config_filename, "r");
    if (!config_file) {
        return false;
    }

    if (!CheckSavedConfig(config_file)) {
        config_file.close();
        return false;
    }

    // Seek to start of Protobuf data.
    if (!config_file.seek(config_offset)) {
        config_file.close();
        return false;
    }

    // Reset config defaults first, so config is completely replaced rather than merged with
    // defaults.
    config = Config_init_default;

    // Decode streamed Protobuf data into config struct.
    pb_istream_t istream = as_pb_istream(config_file, (size_t)config_file.available());
    if (!pb_decode(&istream, Config_fields, &config)) {
        config_file.close();
        return false;
    }

    config_file.close();
    return true;
}

bool Persistence::CheckSavedConfig() {
    // Open file to load config data from.
    File config_file = LittleFS.open(config_filename, "r");
    if (!config_file) {
        return false;
    }

    bool is_valid = CheckSavedConfig(config_file);
    config_file.close();
    return is_valid;
}

size_t Persistence::LoadConfigRaw(Print &out, bool validate) {
    // Open file to load config data from.
    File config_file = LittleFS.open(config_filename, "r");
    if (!config_file) {
        return false;
    }

    // Optionally perform validation.
    if (validate && !CheckSavedConfig(config_file)) {
        config_file.close();
        return false;
    }

    // Seek to start of Protobuf data.
    if (!config_file.seek(config_offset)) {
        config_file.close();
        return false;
    }

    // Write raw Protobuf encoded data to output stream.
    int value;
    while ((value = config_file.read()) != -1) {
        out.write((uint8_t)value);
    }

    config_file.close();
    return true;
}

bool Persistence::CheckSavedConfig(File &config_file) {
    size_t file_size = config_file.size();

    // Read file header.
    ConfigHeader header;
    size_t bytes_read = config_file.read((uint8_t *)&header, sizeof(ConfigHeader));
    if (bytes_read < sizeof(ConfigHeader)) {
        return false;
    }

    // Validate config length.
    size_t config_size = file_size - config_offset;
    if (config_size != header.config_size) {
        return false;
    }

    // Calculate CRC for file contents and compare with CRC in header.
    CRC32 crc;
    int value;
    while ((value = config_file.read()) != -1) {
        crc.update((uint8_t)value);
    }
    if (crc.finalize() != header.config_crc) {
        return false;
    }

    return true;
}

Persistence persistence;