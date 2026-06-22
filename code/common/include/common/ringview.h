
#ifndef COMMON_RINGVIEW_H
#define COMMON_RINGVIEW_H

#include "common/math.h"

#include <stddef.h>
#include <string.h>

// FIXME: Replace functions with #define or templates for better type consistency

// ┌────────────────────────────────────────────────┐
// │ ┌ 0                        ┌ head   ┌ capacity │
// │ __________________[==data==)________│          │
// │                   |- size -|        │          │
// └────────────────────────────────────────────────┘
// ┌────────────────────────────────────────────────┐
// │ ┌ 0      ┌ head                     ┌ capacity │
// │ >========)________[==data==========>│          │
// │ >--------|        |- size --------->│          │
// └────────────────────────────────────────────────┘

/**
 * A view of a buffer enabling write-optimised ring-like data access.
 */
struct RingView {
    // FIXME: Make ISR safe.
    char*  buffer;
    size_t capacity;
    size_t size;
    size_t head;
};

/**
 * Create a new ring view of a given buffer.
 *
 * @param buffer The buffer to view.
 * @param capacity The size of the buffer.
 */
static struct RingView ringview_create(char* buffer, size_t capacity)
{
    return (struct RingView){
        .buffer = buffer,
        .capacity = capacity,
        .size = 0,
        .head = 0,
    };
}

/**
 * Push data to the ring view.
 *
 * @param rv The ring view.
 * @param data The data to push to the buffer.
 * @param size The size of the data.
 * @returns The number of chars pushed to the ring view.
 */
static size_t ringview_push(struct RingView* rv, char const* data, size_t size)
{
    if (max(rv->buffer, data) < min(rv->buffer + rv->capacity, data + size)) {
        // prevent writing from intersecting memory
        return 0;
    }

    size_t remaining_size = rv->capacity - rv->size;
    if (remaining_size == 0) {
        return 0;
    }

    if (size > remaining_size) {
        size = remaining_size;
    }

    size_t block_size = min(size, rv->capacity - rv->head);
    memcpy(rv->buffer + rv->head, data, block_size);
    memcpy(rv->buffer, data + block_size, size - block_size);

    rv->head = (rv->head + size) % rv->capacity;
    rv->size += size;

    return size;
}

/**
 * Pop data from the ring view.
 *
 * @param rv The ring view.
 * @param buffer A buffer to pop the data to.
 * @param size The size of the data to pop.
 * @returns The number of chars popped from the ring view.
 */
static size_t ringview_pop(struct RingView* rv, char* buffer, size_t size)
{
    if (max(rv->buffer, buffer) < min(rv->buffer + rv->capacity, buffer + size)) {
        // prevent writing to intersecting memory
        return 0;
    }

    if (size > rv->size) {
        size = rv->size;
    }

    size_t tail = (rv->capacity + rv->head - rv->size) % rv->capacity;

    size_t block_size = min(size, rv->capacity - tail);
    memcpy(buffer, rv->buffer + tail, block_size);
    memcpy(buffer + block_size, rv->buffer, size - block_size);

    rv->size -= size;

    return size;
}

#endif /* COMMON_RINGVIEW_H */
