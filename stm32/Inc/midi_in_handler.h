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
extern uint8_t skip;

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
    //INPUT_VOLUME,
    INPUT_EXPRESSION,
    INPUT_SYSEX,
    INPUT_SYSEX_NIL,
    INPUT_SYSEX_DEV_ID,
    INPUT_SYSEX_NIL_CMD,
    INPUT_SYSEX_NIL_ARG,
    INPUT_SKIP
} current_input_t;

static inline void change_max_volume(int8_t change) {
    int32_t new_volume = (int32_t) current_max_volume + change;
    if (new_volume < 0) {
        new_volume = 0;
    }
    if (new_volume > 255) {
        new_volume = 255;
    }
    current_max_volume = new_volume;

    uint8_t indicators = current_max_volume >> 5;

    if (indicators > 0) {
        LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
    } else {
        LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
    }
    if (indicators > 1) {
        LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
    } else {
        LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
    }
    if (indicators > 2) {
        LL_GPIO_SetOutputPin(LED3_GPIO_Port, LED3_Pin);
    } else {
        LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);
    }
    if (indicators > 3) {
        LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
    } else {
        LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
    }
    if (indicators > 4) {
        LL_GPIO_SetOutputPin(LED5_GPIO_Port, LED5_Pin);
    } else {
        LL_GPIO_ResetOutputPin(LED5_GPIO_Port, LED5_Pin);
    }
    if (indicators > 5) {
        LL_GPIO_SetOutputPin(LED6_GPIO_Port, LED6_Pin);
    } else {
        LL_GPIO_ResetOutputPin(LED6_GPIO_Port, LED6_Pin);
    }
    if (indicators > 6) {
        LL_GPIO_SetOutputPin(LED7_GPIO_Port, LED7_Pin);
    } else {
        LL_GPIO_ResetOutputPin(LED7_GPIO_Port, LED7_Pin);
    }
    if (current_max_volume == 255) {
        LL_GPIO_SetOutputPin(LED8_GPIO_Port, LED8_Pin);
    } else {
        LL_GPIO_ResetOutputPin(LED8_GPIO_Port, LED8_Pin);
    }

}

static inline uint8_t change_volume(uint8_t volume) {
    if (current_max_volume == 255) return  volume;
    uint16_t temp_volume;
    switch (current_alteration_mode) {
        case VOLUME_CLAMP:
            return volume > current_max_volume ? current_max_volume : volume;
        default:
        case VOLUME_MULTIPLY:
            temp_volume = (uint16_t) volume * (uint16_t) current_max_volume;
            temp_volume = (temp_volume >> 8);
            if (temp_volume > 255) {
                temp_volume = 255;
            }
            return (uint8_t) temp_volume;
    }
}

void handle_midi_byte_in(uint8_t data);

extern ring_buffer direct_buffer;
extern uint8_t dbg_in[256];
#endif // MIDI_IN_HANDLER_H
