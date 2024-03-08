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

#ifndef _COMMS_CONFIGURATORBACKEND_HPP
#define _COMMS_CONFIGURATORBACKEND_HPP

#include "cobs/Print.h"
#include "cobs/Stream.h"
#include "core/CommunicationBackend.hpp"
#include "core/Persistence.hpp"

#include <config.pb.h>

class ConfiguratorBackend : public CommunicationBackend {
  public:
    ConfiguratorBackend(
        InputState &inputs,
        InputSource **input_sources,
        size_t input_source_count,
        Config &config
    );
    ~ConfiguratorBackend();
    CommunicationBackendId BackendId();
    void SendReport();

  private:
    size_t ReadPacket(uint8_t *buffer, size_t max_len);
    bool WritePacket(Command command_id, uint8_t *buffer, size_t len);
    bool HandleUnknownCommand(Command command);
    bool HandleGetDeviceInfo();
    bool HandleGetConfig();
    bool HandleSetConfig(uint8_t *buffer, size_t len);

    packetio::COBSStream _in;
    packetio::COBSPrint _out;
    Persistence *_persistence;
    uint8_t _cmd_buffer[Persistence::eeprom_size - Persistence::config_offset];
    Config &_config;
};

#endif