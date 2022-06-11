#ifndef _SERIAL_HPP
#define _SERIAL_HPP

#include "stdlib.hpp"

namespace serial {
    void init(unsigned long baudrate);
    void close();
    void print(const char *string);
    void write(uint8_t byte);
    void write(uint8_t *bytes, size_t len);
    int available_for_write();
}

#endif