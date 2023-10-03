#ifndef _COMMS_CONFIGURATORBACKEND_HPP
#define _COMMS_CONFIGURATORBACKEND_HPP

#include "cobs/Print.h"
#include "cobs/Stream.h"
#include "core/CommunicationBackend.hpp"
#include "core/Persistence.hpp"

#include <config.pb.h>

class ConfiguratorBackend : public CommunicationBackend {
  public:
    ConfiguratorBackend(InputSource **input_sources, size_t input_source_count, Config &config);
    ~ConfiguratorBackend();
    void SendReport();

  private:
    size_t ReadPacket(uint8_t *buffer, size_t max_len);
    bool WritePacket(Command command_id, uint8_t *buffer, size_t len);
    bool HandleUnknownCommand(Command command);
    bool HandleGetDeviceInfo();
    bool HandleGetConfig();
    bool HandleSetConfig(uint8_t *buffer, size_t len);

    packetio::COBSStream *_in;
    packetio::COBSPrint *_out;
    Persistence *_persistence;
    uint8_t _cmd_buffer[Persistence::eeprom_size - Persistence::config_offset];
    Config &_config;
};

#endif