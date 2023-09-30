#ifndef _CORE_PERSISTENCE_HPP
#define _CORE_PERSISTENCE_HPP

#include <CRC32.h>
#include <config.pb.h>

namespace persistence {
    typedef struct _ConfigHeader {
        size_t config_size = 0;
        uint32_t config_crc = 0;
    } ConfigHeader;

    bool save_config(Config &config);
    bool load_config(Config &config);
    size_t load_config_raw(uint8_t *buffer, size_t buffer_len);
}

#define EEPROM_SIZE 4096
#define CONFIG_OFFSET sizeof(persistence::ConfigHeader)

#endif