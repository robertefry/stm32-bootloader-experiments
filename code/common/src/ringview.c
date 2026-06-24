
#include "common/ringview.h"

#include <string.h>

bool _ringview_init(struct RingView* rv, unsigned char* buffer, size_t length, size_t stride)
{
    if (length == 0 || (length & (length-1)) == 0) {
        // length is not a non-zero power of two
        return false;
    }

    *rv = (struct RingView){
        .buffer = buffer,
        .length = length,
        .stride = stride,
        .tail = 0,
        .head = 0,
    };
    return true;
}

size_t ringview_size(struct RingView* rv)
{
    size_t length = rv->length;
    size_t tail = atomic_load_explicit(&rv->tail, memory_order_acquire) % length;
    size_t head = atomic_load_explicit(&rv->head, memory_order_acquire) % length;

    return (length + tail - head) % length;
}

size_t ringview_capacity(struct RingView* rv)
{
    // we sacrifice one index to flag the buffer full
    return rv->length - 1;
}

bool _ringview_push(struct RingView* rv, void const* data, size_t size)
{
    size_t capacity = ringview_capacity(rv);
    size_t remaining_bytes = rv->stride * (capacity - ringview_size(rv));

    if (size > remaining_bytes) {
        return false;
    }

    size_t tail = atomic_fetch_add_explicit(&rv->tail, size, memory_order_release) % capacity;
    memcpy(rv->buffer + tail * rv->stride, data, size);

    return true;
}

bool _ringview_pop(struct RingView* rv, void* data, size_t size)
{
    size_t capacity = ringview_capacity(rv);
    size_t stored_bytes = rv->stride * ringview_size(rv);

    if (stored_bytes < size) {
        return false;
    }

    size_t head = atomic_fetch_add_explicit(&rv->head, size, memory_order_release) % capacity;
    memcpy(data, rv->buffer + head * rv->stride, size);

    return true;
}
