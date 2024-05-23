#include "modes/CustomControllerMode.hpp"

#include "util/state_util.hpp"

#define SIGNUM(x) ((x > 0) - (x < 0))
#define ANALOG_STICK_NEUTRAL 128

CustomControllerMode::CustomControllerMode() : ControllerMode() {}

void CustomControllerMode::SetConfig(
    GameModeConfig &config,
    const CustomModeConfig &custom_mode_config
) {
    InputMode::SetConfig(config);
    _custom_mode_config = &custom_mode_config;
    for (size_t i = 0; i < custom_mode_config.modifiers_count; i++) {
        _modifier_button_masks[i] = make_button_mask(
            custom_mode_config.modifiers[i].buttons,
            custom_mode_config.modifiers[i].buttons_count
        );
    }
}

void CustomControllerMode::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    if (_custom_mode_config == nullptr) {
        return;
    }

    for (size_t output = 0; output < _custom_mode_config->digital_button_mappings_count; output++) {
        Button input = _custom_mode_config->digital_button_mappings[output];
        set_output(outputs.buttons, (DigitalOutput)(output + 1), get_button(inputs.buttons, input));
    }

    if (inputs.nunchuk_connected) {
        outputs.triggerLDigital |= inputs.nunchuk_z;
    }
}

void CustomControllerMode::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    if (_custom_mode_config == nullptr) {
        return;
    }

    const Button *direction_buttons = _custom_mode_config->stick_direction_mappings;
    uint8_t stick_range = _custom_mode_config->stick_range;
    UpdateDirections(
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_LSTICK_LEFT)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_LSTICK_RIGHT)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_LSTICK_DOWN)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_LSTICK_UP)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_RSTICK_LEFT)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_RSTICK_RIGHT)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_RSTICK_DOWN)),
        get_button(inputs.buttons, GetDirectionButton(direction_buttons, SD_RSTICK_UP)),
        ANALOG_STICK_NEUTRAL - stick_range,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_NEUTRAL + stick_range,
        outputs
    );

    const AnalogModifier *modifiers = _custom_mode_config->modifiers;
    for (size_t i = 0; i < _custom_mode_config->modifiers_count; i++) {
        const AnalogModifier &modifier = modifiers[i];
        if (modifier.axis == AXIS_UNSPECIFIED || modifier.axis > _AnalogAxis_MAX) {
            continue;
        }
        if (!all_buttons_held(inputs.buttons, _modifier_button_masks[i])) {
            continue;
        }

        uint8_t OutputState::*axis = axis_pointer(modifier.axis);
        if (axis != nullptr) {
            int8_t sign = 0;
            switch (modifier.combination_mode) {
                case COMBINATION_MODE_OVERRIDE:
                    sign = SIGNUM(outputs.*axis);
                    outputs.*axis = ANALOG_STICK_NEUTRAL +
                                    _custom_mode_config->stick_range * modifier.multiplier * sign;
                    break;
                case COMBINATION_MODE_COMPOUND:
                case COMBINATION_MODE_UNSPECIFIED:
                default:
                    outputs.*axis = ANALOG_STICK_NEUTRAL +
                                    (outputs.*axis - ANALOG_STICK_NEUTRAL) * modifier.multiplier;
                    break;
            }
        }
    }

    const AnalogTriggerMapping *analog_trigger_mappings =
        _custom_mode_config->analog_trigger_mappings;
    for (size_t i = 0; i < _custom_mode_config->analog_trigger_mappings_count; i++) {
        const AnalogTriggerMapping &mapping = analog_trigger_mappings[i];
        if (get_button(inputs.buttons, mapping.button)) {
            switch (mapping.trigger) {
                case TRIGGER_LT:
                    outputs.triggerLAnalog = mapping.value;
                    break;
                case TRIGGER_RT:
                    outputs.triggerRAnalog = mapping.value;
                    break;
                default:
                    break;
            }
        }
    }

    if (outputs.triggerLDigital) {
        outputs.triggerLAnalog = 255;
    }
    if (outputs.triggerRDigital) {
        outputs.triggerRAnalog = 255;
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