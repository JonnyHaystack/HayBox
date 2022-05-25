#include "modes/DefaultKeyboardMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"

DefaultKeyboardMode::DefaultKeyboardMode(socd::SocdType socd_type) : KeyboardMode(socd_type) {}

void DefaultKeyboardMode::SendKeys(InputState &inputs) {
    Press('a', inputs.l);
    Press('b', inputs.left);
    Press('c', inputs.down);
    Press('d', inputs.right);
    Press('e', inputs.mod_x);
    Press('f', inputs.mod_y);
    Press('g', inputs.select);
    Press('h', inputs.start);
    Press('i', inputs.home);
    Press('j', inputs.r);
    Press('k', inputs.y);
    Press('l', inputs.lightshield);
    Press('m', inputs.midshield);
    Press('n', inputs.b);
    Press('o', inputs.x);
    Press('p', inputs.z);
    Press('q', inputs.up);
    Press('r', inputs.c_up);
    Press('s', inputs.c_left);
    Press('t', inputs.c_right);
    Press('u', inputs.a);
    Press('v', inputs.c_down);
}
