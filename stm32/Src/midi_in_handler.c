#include "midi_in_handler.h"
#include "midi.h"
#include "config.h"


uint8_t last_command = 0;
current_input_t input_state = INPUT_NONE;
uint16_t next_command_delay = 0;

volume_alteration_t current_alteration_mode;
volume_intercept_t current_intercept_mode;
uint8_t current_max_volume;
uint8_t new_max_volume;
sysex_nil_cmd_t current_command;
uint8_t skip = 0;

uint8_t dbg_in[256] = {0};
uint8_t dbg_in_ptr = 0;

ring_buffer direct_buffer = {{0}, 0, 0};

static void DirectWrite(uint8_t data) {
    ring_pushb(&direct_buffer, data);
    if (LL_USART_IsActiveFlag_TXE(USART1)) {
        LL_USART_TransmitData8(USART1, ring_popb(&direct_buffer));
        LL_USART_EnableIT_TXE(USART1);
    }
}
volatile char capture_dbg_sysex[256];
volatile uint8_t dbg_ptr = 0;

void handle_midi_byte_in(uint8_t data) {
    dbg_in[dbg_in_ptr] = data;
    if (dbg_ptr >= 255) {
        dbg_in_ptr = 0;
    } else {
        dbg_in_ptr += 1;
    }

    switch (input_state) {
    case INPUT_SKIP:
        skip -=1;
        if (skip == 0) {
            input_state = INPUT_NONE;
        }
        break;
    case INPUT_NONE:
        switch (data & 0xf0) {
        case 0x90:
            input_state = INPUT_NOTEON_NN;
            goto Done;
        case 0xb0:
            input_state = INPUT_CONTROL;
            goto Done;
        case 0xf0:
            if (data == 0xf0) {
                input_state = INPUT_SYSEX_DEV_ID;
            } else if (data == 0xf7) {
                input_state = INPUT_NONE;
            }
            return;
        default:
            if (MIDI_evt_len[data] == 0) {
                input_state = INPUT_NONE;
            } else {
                skip = MIDI_evt_len[data] - 1;
                input_state = INPUT_SKIP;
            }
            break;
        }
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
        } else {
            skip = 1;
            input_state = INPUT_SKIP;
        }
        break;
    case INPUT_SYSEX_DEV_ID:
        if (data == DEV_NIL) {
            input_state = INPUT_SYSEX_NIL_CMD;
        } else {
            input_state = INPUT_SYSEX;

            MPU401_WriteData(0xf0);
            MPU401_WriteData(data);
            DirectWrite(0xf0);
            DirectWrite(data);
        }
        return;
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
            input_state = INPUT_SYSEX_NIL;
            break;

        case NIL_SYSEX_ALTERATION_MODE:
            current_alteration_mode = (volume_alteration_t) data & 0x0f;
            input_state = INPUT_SYSEX_NIL;
            break;
        }
        return;
    case INPUT_SYSEX_NIL:
        if (data == 0xf7) {
            input_state = INPUT_NONE;
        }
        return;
    case INPUT_SYSEX:
        capture_dbg_sysex[dbg_ptr] = data;
        if (dbg_ptr == 255) {
            dbg_ptr = 0;
        } else {
            dbg_ptr += 1;
        }
        if (data == 0xf7) {
            input_state = INPUT_NONE;
        }
        break;
    }
    Done:
    MPU401_WriteData(data);
    DirectWrite(data);
}
