
#ifndef COMMON_RINGVIEW_H
#define COMMON_RINGVIEW_H

#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>

/**
 * @brief A non-owning view of a byte buffer enabling lock-free SPSC ring-like
 * FIFO buffer management. Applicable as a thread transfer queue or ISR buffer.
 */
struct RingViewSPSC {
    void* buffer;
    size_t size;
    size_t stride;
    atomic_size_t head;
    atomic_size_t tail;
};

/**
 * @brief Initialise a RingView over a given byte buffer.
 *
 * @param rv The RingView to initialise.
 * @param buffer A non-null pointer to an empty buffer.
 * @param size The size of the @p buffer.
 * @param stride The alignment of the @p buffer (must be a non-zero power of two).
 */
void ringview_spsc_init(struct RingViewSPSC* rv, void* buffer, size_t size, size_t stride);

/**
 * @brief Return whether the RingView is empty.
 */
[[nodiscard]]
bool ringview_spsc_empty(struct RingViewSPSC const* rv);

/**
 * @brief Return the number of bytes in the RingView.
 */
[[nodiscard]]
size_t ringview_spsc_size(struct RingViewSPSC const* rv);

/**
 * @brief Return the maximum number of bytes storable in the RingView.
 */
[[nodiscard]]
size_t ringview_spsc_capacity(struct RingViewSPSC const* rv);

/**
 * @brief Write data into the RingView.
 *
 * @param rv A non-null pointer to an initialised RingView.
 * @param data A non-null pointer to the source data.
 * @param size The size of @p data in bytes (must be a multiple of @p stride).
 *
 * @returns True if-and-only-if @p size bytes were successfully written.
 *
 * @note If @p size bytes cannot be written, nothing will happen.
 */
[[nodiscard]]
bool ringview_spsc_push(struct RingViewSPSC* rv, void const* data, size_t size);

/**
 * @brief Read and consume data from the ring buffer.
 *
 * @param rv A non-null pointer to an initialised RingView.
 * @param data A non-null pointer to a destination data.
 * @param size The size of @p data in bytes (must be a multiple of @p stride).
 *
 * @returns True if-and-only-if @p size bytes were successfully read.
 *
 * @note If @p size bytes cannot be read, nothing will happen.
 */
[[nodiscard]]
bool ringview_spsc_pop(struct RingViewSPSC* rv, void* data, size_t size);

#endif /* COMMON_RINGVIEW_H */
