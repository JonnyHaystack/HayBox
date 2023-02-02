#include "serial.hpp"

#include "stdlib.hpp"

#include <Adafruit_TinyUSB.h>

namespace serial {
    void init(unsigned long baudrate) {
        Serial.begin(baudrate);
    }

    void close() {
        Serial.end();
    }

    void print(const char *string) {
        Serial.print(string);
    }

    void write(uint8_t byte) {
        Serial.write(byte);
    }

    void write(uint8_t *bytes, size_t len) {
        Serial.write(bytes, len);
    }

    int available_for_write() {
        return Serial.availableForWrite();
    }
}