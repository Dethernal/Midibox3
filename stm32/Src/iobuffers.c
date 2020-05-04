#include "iobuffers.h"

void ring_init(ring_buffer* buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    buffer->buffer[0] = 0;
}
