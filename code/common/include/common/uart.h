
#ifndef COMMON_UART_H
#define COMMON_UART_H

#include <stddef.h>
#include <stdint.h>

/**
 * Setup the UART interface driver.
 */
void uart_setup(uint32_t baudrate);

/**
 * Write some bytes to the UART TX pin.
 *
 * @param buffer A pointer to an array of bytes.
 * @param size The number of bytes to write.
 * @returns The number of bytes written.
 */
size_t uart_tx_write(uint8_t const* buffer, size_t size);

/**
 * Determine the number of bytes available to read from the UART RX pin.
 */
size_t uart_rx_available(void);

/**
 * Read some bytes from the UART RX pin.
 *
 * @param buffer A pointer to a buffer to write bytes into.
 * @param size The available size of the buffer.
 * @returns The number of bytes written to the buffer.
 */
size_t uart_rx_read(uint8_t* buffer, size_t size);

#endif /* COMMON_UART_H */
