
#include "common/ringview.h"
#include "common/math.h"
#include "common/debug.h"

#include <string.h>

void ringview_spsc_init(struct RingViewSPSC* rv, void* buffer, size_t size, size_t stride)
{
    DEBUG_ASSERT(
        stride != 0 && (stride & (stride-1)) == 0
        , "Req: the stride must be a non-zero power of two"
    );

    // round down the size to the nearest multiple of stride for alignment
    size -= size & (stride - 1);

    DEBUG_ASSERT(
        size != 0
        , "Req: reserve one stride to denote the full state"
    );

    *rv = (struct RingViewSPSC){
        .buffer = buffer,
        .size = size,
        .stride = stride,
        .tail = 0,
        .head = 0,
    };
}

bool ringview_spsc_empty(struct RingViewSPSC const* rv)
{
    size_t tail = atomic_load_explicit(&rv->tail, memory_order_acquire);
    size_t head = atomic_load_explicit(&rv->head, memory_order_acquire);

    return head == tail;
}

size_t ringview_spsc_size(struct RingViewSPSC const* rv)
{
    size_t tail = atomic_load_explicit(&rv->tail, memory_order_acquire);
    size_t head = atomic_load_explicit(&rv->head, memory_order_acquire);

    return (rv->size + tail - head) % rv->size;
}

size_t ringview_spsc_capacity(struct RingViewSPSC const* rv)
{
    return rv->size - rv->stride;
}

bool ringview_spsc_push(struct RingViewSPSC* rv, void const* data, size_t size)
{
    DEBUG_ASSERT(
        size == ((size + rv->stride - 1) & ~(rv->stride - 1))
        , "Req: size must be a multiple of the stride"
    );

    if (size > ringview_spsc_capacity(rv) - ringview_spsc_size(rv)) {
        return false;
    }

    size_t tail = atomic_load_explicit(&rv->tail, memory_order_relaxed);
    size_t chunk = min(size, rv->size - tail);

    memcpy((unsigned char*)rv->buffer + tail, data, chunk);
    memcpy(rv->buffer, (unsigned char*)data + chunk, size - chunk);

    atomic_store_explicit(&rv->tail, (tail + size) % rv->size, memory_order_release);
    return true;
}

bool ringview_spsc_pop(struct RingViewSPSC* rv, void* data, size_t size)
{
    DEBUG_ASSERT(
        size == ((size + rv->stride - 1) & ~(rv->stride - 1))
        , "Req: size must be a multiple of the stride"
    );

    if (size > ringview_spsc_size(rv)) {
        return false;
    }

    size_t head = atomic_load_explicit(&rv->head, memory_order_relaxed);
    size_t chunk = min(size, rv->size - head);

    memcpy(data, (unsigned char*)rv->buffer + head, chunk);
    memcpy((unsigned char*)data + chunk, rv->buffer, size - chunk);

    atomic_store_explicit(&rv->head, (head + size) % rv->size, memory_order_release);
    return true;
}
