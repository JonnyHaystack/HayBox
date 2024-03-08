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

#ifndef _CORE_PERSISTENCE_HPP
#define _CORE_PERSISTENCE_HPP

#include <config.pb.h>

class Persistence {
  public:
    typedef struct _ConfigHeader {
        size_t config_size = 0;
        uint32_t config_crc = 0;
    } ConfigHeader;

    Persistence();
    ~Persistence();

    bool SaveConfig(Config &config);
    bool LoadConfig(Config &config);
    size_t LoadConfigRaw(uint8_t *buffer, size_t buffer_len);

    static constexpr size_t eeprom_size = 4096;
    static constexpr size_t config_offset = sizeof(ConfigHeader);

  private:
    static Config _config;
    static uint8_t _buffer[eeprom_size - config_offset];
};

extern Persistence persistence;

#endif