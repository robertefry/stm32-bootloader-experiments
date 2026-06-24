
#ifndef COMMON_RINGVIEW_H
#define COMMON_RINGVIEW_H

#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>

/**
 * @brief A non-owning view of a byte buffer enabling lock-free SPSC ring-like
 * FIFO buffer management. Applicable as a thread transfer queue or ISR buffer.
 */
struct RingView {
    unsigned char* buffer;
    size_t stride;
    size_t length;
    atomic_size_t head;
    atomic_size_t tail;
};

/**
 * @brief Initialise a RingView over a given byte buffer.
 *
 * @param rv The RingView to initialise.
 *
 * @param buffer A non-null pointer to a buffer of at least (@p length * @p stride)
 * bytes. The buffer must remain valid for the lifetime of the RingView.
 *
 * @param length The number of element slots in @p buffer. Must be a non-zero power of two.
 *
 * @param stride The size of each element in bytes.
 *
 * @note The capacity of the RingView is one less than the length of the buffer.
 * We use one element slot to mark the buffer as full.
 *
 * @returns True if-and-only-if initialisation was successful.
 */
bool _ringview_init(struct RingView* rv, unsigned char* buffer, size_t length, size_t stride);

/**
 * @brief Initialise a RingView over a given element buffer.
 *
 * @param rv The RingView to initialise.
 * @param buffer The backing element buffer.
 * @param length The number of element slots in @p buffer. Must be a non-zero power of two.
 *
 * @note The capacity of the RingView is one less than the length of the buffer.
 * We use one element slot to mark the buffer as full.
 *
 * @returns True if-and-only-if initialisation was successful.
 */
#define ringview_init(rv, buffer, length) \
    _ringview_init((rv), (unsigned char*)(buffer), (length), sizeof(*(buffer)))

/**
 * @brief Return the number of elements in the ring buffer.
 */
size_t ringview_size(struct RingView* rv);

/**
 * @brief Return the maximum number of elements storable in the ring buffer.
 */
size_t ringview_capacity(struct RingView* rv);

/**
 * @brief Write bytes into the ring buffer.
 *
 * @note If @p size bytes cannot be written, nothing will happen.
 *
 * @param rv A non-null pointer to an initialised RingView.
 * @param data A non-null pointer to the source data.
 * @param size The number of bytes to write.
 *
 * @returns True if-and-only-if @p size bytes where successfully written.
 */
bool _ringview_push(struct RingView* rv, void const* data, size_t size);

/**
 * @brief Write an element into the ring buffer.
 *
 * @param rv A non-null pointer to the RingView.
 * @param element A non-null pointer to the element to write.
 *
 * @returns True if-and-only-if the element was successfully written.
 */
#define ringview_push(rv, element) \
    _ringview_push((rv), (element), sizeof(*(element)))

/**
 * @brief Read and consume bytes from the ring buffer.
 *
 * @note If @p size bytes cannot be read, nothing will happen.
 *
 * @param rv A non-null pointer to an initialised RingView.
 * @param data A non-null pointer to a destination buffer.
 * @param size The number of bytes to read.
 *
 * @returns True if-and-only-if @p size bytes where successfully read.
 */
bool _ringview_pop(struct RingView* rv, void* data, size_t size);

/**
 * @brief Read and consume an element from the ring buffer.
 *
 * @param rv A non-null pointer to the RingView.
 * @param element A non-null pointer to the destination element.
 *
 * @returns True if-and-only-if the element was successfully consumed.
 */
#define ringview_pop(rv, element) \
    _ringview_pop((rv), (element), sizeof(*(element)))

#endif /* COMMON_RINGVIEW_H */
