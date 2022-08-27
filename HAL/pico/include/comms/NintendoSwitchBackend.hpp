#ifndef _COMMS_NINTENDOSWITCHBACKEND_HPP
#define _COMMS_NINTENDOSWITCHBACKEND_HPP

#include "core/CommunicationBackend.hpp"

typedef enum {
    SWITCH_HAT_UP,
    SWITCH_HAT_UP_RIGHT,
    SWITCH_HAT_RIGHT,
    SWITCH_HAT_DOWN_RIGHT,
    SWITCH_HAT_DOWN,
    SWITCH_HAT_DOWN_LEFT,
    SWITCH_HAT_LEFT,
    SWITCH_HAT_UP_LEFT,
    SWITCH_HAT_CENTERED,
} switch_gamepad_hat_t;

typedef struct __attribute__((packed, aligned(1))) {
    bool y : 1;
    bool b : 1;
    bool a : 1;
    bool x : 1;
    bool l : 1;
    bool r : 1;
    bool zl : 1;
    bool zr : 1;
    bool minus : 1;
    bool plus : 1;
    bool l3 : 1;
    bool r3 : 1;
    bool home : 1;
    bool capture : 1;
    uint8_t reserved0 : 2;
    switch_gamepad_hat_t hat;
    uint8_t lx;
    uint8_t ly;
    uint8_t rx;
    uint8_t ry;
    uint8_t reserved1;
} switch_gamepad_report_t;

class NintendoSwitchBackend : public CommunicationBackend {
  public:
    NintendoSwitchBackend(InputSource **input_sources, size_t input_source_count);
    ~NintendoSwitchBackend();

    static void RegisterDescriptor();

    void SendReport();

  protected:
    static const uint8_t _report_id = 0;
    static uint8_t _descriptor[];

    switch_gamepad_report_t _report;

    static switch_gamepad_hat_t GetHatPosition(bool left, bool right, bool down, bool up);
};

#endif