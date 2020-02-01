#include "iobuffers.h"

ring_buffer midi_in_buf;
ring_buffer db_out_buf;
ring_buffer din_out_buf;

void ring_init(ring_buffer* buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    buffer->buffer[0] = 0;
}
