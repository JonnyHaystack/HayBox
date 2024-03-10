#include "modes/CustomControllerMode.hpp"

#include "util/state_util.hpp"

#define ANALOG_STICK_NEUTRAL 128

CustomControllerMode::CustomControllerMode(
    GameModeConfig &config,
    const CustomModeConfig &custom_mode_config
)
    : ControllerMode(config),
      _custom_mode_config(custom_mode_config) {}

void CustomControllerMode::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    for (size_t output = 0; output < _custom_mode_config.digital_button_mappings_count; output++) {
        Button input = _custom_mode_config.digital_button_mappings[output];
        set_output(outputs.buttons, (DigitalOutput)(output + 1), get_button(inputs.buttons, input));
    }

    if (inputs.nunchuk_connected) {
        outputs.triggerLDigital |= inputs.nunchuk_z;
    }
}

void CustomControllerMode::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    const Button *direction_buttons = _custom_mode_config.stick_direction_mappings;
    UpdateDirections(
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_LSTICK_LEFT)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_LSTICK_RIGHT)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_LSTICK_DOWN)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_LSTICK_UP)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_RSTICK_LEFT)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_RSTICK_RIGHT)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_RSTICK_DOWN)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_RSTICK_UP)),
        ANALOG_STICK_NEUTRAL - _custom_mode_config.stick_range,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_NEUTRAL + _custom_mode_config.stick_range,
        outputs
    );

    const AnalogModifier *modifiers = _custom_mode_config.modifiers;
    for (size_t i = 0; i < _custom_mode_config.modifiers_count; i++) {
        const AnalogModifier &modifier = modifiers[i];
        if (modifier.axis == AXIS_UNSPECIFIED || modifier.axis > _AnalogAxis_MAX) {
            continue;
        }
        if (!get_button(inputs.buttons, modifier.button)) {
            continue;
        }

        uint8_t OutputState::*axis = axis_pointer(modifier.axis);
        if (axis != nullptr) {
            outputs.*axis =
                ANALOG_STICK_NEUTRAL + (outputs.*axis - ANALOG_STICK_NEUTRAL) * modifier.multiplier;
        }
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}

Button CustomControllerMode::GetDirectionButton(
    const Button *direction_buttons,
    StickDirectionButton direction
) {
    if (direction == SD_UNSPECIFIED || direction > _StickDirectionButton_MAX) {
        return BTN_UNSPECIFIED;
    }
    return direction_buttons[direction - 1];
}