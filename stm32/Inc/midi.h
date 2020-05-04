#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>
#include "iobuffers.h"

extern const uint8_t MIDI_evt_len[256];
extern ring_buffer midi_out_buff;

#endif // MIDI_H
