#ifndef MIDI_IN_HANDLER_H
#define MIDI_IN_HANDLER_H

#include "export.h"
#include "iobuffers.h"
#include <stdint.h>
#include <midi.h>

typedef enum volume_alteration_t {
    VOLUME_CLAMP,
    VOLUME_MULTIPLY
} volume_alteration_t;

typedef enum volume_intercept_t {
    VOLUME_NOT_INTERCEPT,
    VOLUME_MASTER,
    VOLUME_VELOCITY
} volume_intercept_t;


extern volume_alteration_t current_alteration_mode;
extern volume_intercept_t current_intercept_mode;
extern uint8_t current_max_volume;

static const uint8_t DEV_NIL = 'N';

typedef enum sysex_nil_cmd_t {
    NIL_SYSEX_VOLUME,
    NIL_SYSEX_INTERCEPT_MODE,
    NIL_SYSEX_ALTERATION_MODE
} sysex_nil_cmd_t;

typedef enum current_input_t {
    INPUT_NONE,
    INPUT_CONTROL,
    INPUT_NOTEON_NN,
    INPUT_VOLUME,
    INPUT_EXPRESSION,
    INPUT_SYSEX,
    INPUT_SYSEX_DEV_ID,
    INPUT_SYSEX_NIL_CMD,
    INPUT_SYSEX_NIL_ARG
} current_input_t;

inline uint8_t change_volume(uint8_t volume) {
    if (current_max_volume == 255) return  volume;
    uint16_t temp_volume;
    switch (current_alteration_mode) {
        case VOLUME_CLAMP:
            return volume > current_max_volume ? current_max_volume : volume;
        case VOLUME_MULTIPLY:
            temp_volume = (uint16_t) volume * (uint16_t) current_max_volume;
            return (uint8_t) (temp_volume >> 8);
    }
}

void handle_midi_byte_in(uint8_t data);

#endif // MIDI_IN_HANDLER_H
