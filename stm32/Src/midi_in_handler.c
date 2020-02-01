#include "midi_in_handler.h"

uint8_t last_command = 0;
current_input_t input_state = INPUT_NONE;
uint16_t next_command_delay = 0;

volume_alteration_t current_alteration_mode;
volume_intercept_t current_intercept_mode;
uint8_t current_max_volume;
uint8_t new_max_volume;
sysex_nil_cmd_t current_command;

void handle_midi_byte_in(uint8_t data) {
    switch (input_state) {
    case INPUT_NONE:
        switch (data & 0xf0) {
        case 0x90:
            input_state = INPUT_NOTEON_NN;
            break;
        case 0xb0:
            input_state = INPUT_CONTROL;
            break;
        case 0xf0:
            if (data == 0xf0) {
                input_state = INPUT_SYSEX_DEV_ID;
            } else if (data == 0xf7) {
                input_state = INPUT_NONE;
            }
        }
        break;
    case INPUT_NOTEON_NN:
        input_state = INPUT_EXPRESSION;
        break;
    case INPUT_EXPRESSION:
        if (current_intercept_mode == VOLUME_VELOCITY) {
            data = change_volume(data);
        }
        input_state = INPUT_NONE;
        break;
    case INPUT_CONTROL:
        if (data == 0xb) {
            input_state = INPUT_EXPRESSION;
        }
        break;
    case INPUT_SYSEX_DEV_ID:
        if (data == DEV_NIL) {
            input_state = INPUT_SYSEX_NIL_CMD;
        } else {
            input_state = INPUT_SYSEX;
        }
        break;
    case INPUT_SYSEX_NIL_CMD:
        new_max_volume = 0;
        current_max_volume = 0;
        current_command = (sysex_nil_cmd_t) data;
        input_state = INPUT_SYSEX_NIL_ARG;
        break;
    case INPUT_SYSEX_NIL_ARG:
        switch (current_command) {
        case NIL_SYSEX_VOLUME:
            current_max_volume = ((data & 0x0f) << 4) + current_max_volume;
            new_max_volume = data & 0x0f;
            break;

        case NIL_SYSEX_INTERCEPT_MODE:
            current_intercept_mode = (volume_intercept_t) data & 0x0f;
            input_state = INPUT_SYSEX;
            break;

        case NIL_SYSEX_ALTERATION_MODE:
            current_alteration_mode = (volume_alteration_t) data & 0x0f;
            input_state = INPUT_SYSEX;
            break;
        }
        break;
    case INPUT_SYSEX:
        if (data == 0xf7) {
            input_state = INPUT_NONE;
        }
        break;
    }
}
