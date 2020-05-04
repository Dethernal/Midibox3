#ifndef IOBUFFERS_H
#define IOBUFFERS_H

#include <stdint.h>

/* RAWBUF: This is the buffer for outgoing MIDI data. The larger the buffer,
    the less likely it is to overrun when SysEx delay is enabled and large SysEx
    transfers are occurring. */
#define RAWBUF  1025
typedef struct ring_buffer {
    uint8_t buffer[RAWBUF];
    uint16_t head;
    uint16_t tail;
} ring_buffer;

void ring_init(ring_buffer* buffer);

static inline uint8_t* ring_tail_offset(ring_buffer* buffer, const uint16_t offset) {
    if (buffer->tail + offset >= RAWBUF) {
        return &buffer->buffer[buffer->tail + offset - RAWBUF];
    } else {
        return &buffer->buffer[buffer->tail + offset];
    }
}

static inline uint8_t ring_popb(ring_buffer* buffer) {
    const uint8_t data = buffer->buffer[buffer->tail];
    buffer->tail += 1;
    if (buffer->tail >= RAWBUF) {
      buffer->tail = 0;
    }
    return data;
}

static inline void ring_pushb(ring_buffer* buffer, const uint8_t data) {
    buffer->buffer[buffer->head] = data;
    buffer->head += 1;
    if (buffer->head >= RAWBUF) {
      buffer->head = 0;
    }
}

static inline void ring_pushs(ring_buffer* buffer, const char* data) {
    while (*data) {
      ring_pushb(buffer, *data++);
    }
}

static inline void ring_push(ring_buffer* buffer, char* data, uint16_t size) {
    for (uint16_t i = 0; i < size; ++i) {
      ring_pushb(buffer, *(data + i));
    }
}

static inline void ring_ring_move(ring_buffer* in_buffer, ring_buffer* out_buffer, const uint16_t size) {
    uint8_t data;
    for (uint16_t i = 0; i < size; ++i) {
        data = ring_popb(in_buffer);
        ring_pushb(out_buffer, data);
    }
}

static inline void ring_ring_copy(ring_buffer* in_buffer, ring_buffer* out_buffer, const uint16_t size) {
    for (uint16_t i = 0; i < size; ++i) {
        uint8_t *data_ptr = ring_tail_offset(in_buffer, i + 1);
        ring_pushb(out_buffer, *data_ptr);
    }
}

#endif // IOBUFFERS_H
