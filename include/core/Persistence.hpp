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
    uint8_t _buffer[eeprom_size - config_offset];
};

#endif