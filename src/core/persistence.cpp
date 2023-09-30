#include "core/persistence.hpp"

#include "stdlib.hpp"

#include <EEPROM.h>
#include <pb_decode.h>
#include <pb_encode.h>

bool persistence::save_config(Config &config) {
    uint8_t buffer[EEPROM_SIZE - CONFIG_OFFSET];

    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&ostream, &Config_msg, &config)) {
        return false;
    }

    ConfigHeader header = {
        .config_size = ostream.bytes_written,
        .config_crc = CRC32::calculate(buffer, ostream.bytes_written),
    };

    // Store number of bytes of configuration data at offset 0.
    EEPROM.put(0, header);

    // Store the rest of the data directly after that.
    for (size_t i = 0; i < header.config_size; i++) {
        EEPROM.write(CONFIG_OFFSET + i, buffer[i]);
    }
#ifdef ARDUINO_PICO_REVISION
    EEPROM.commit();
#endif

    return true;
}

bool persistence::load_config(Config &config) {
    uint8_t buffer[EEPROM_SIZE - CONFIG_OFFSET];
    size_t config_size = load_config_raw(buffer, sizeof(buffer));

    // No valid config found.
    if (config_size == 0) {
        return false;
    }

    pb_istream_t istream = pb_istream_from_buffer(buffer, config_size);

    Config original = config;

    // Return true if successfully decoded.
    if (pb_decode(&istream, &Config_msg, &config)) {
        return true;
    }

    // Otherwise reset back to original config.
    config = original;
    return false;
}

size_t persistence::load_config_raw(uint8_t *buffer, size_t buffer_len) {
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
        buffer[bytes_read] = EEPROM.read(CONFIG_OFFSET + bytes_read);
    }

    // If CRC32 checksum does not match, consider it as having failed to read.
    if (CRC32::calculate(buffer, bytes_read) != header.config_crc) {
        return 0;
    }

    // Return the number of bytes of config data read.
    return bytes_read;
}