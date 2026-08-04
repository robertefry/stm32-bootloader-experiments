
#ifndef COMMON_UART_H
#define COMMON_UART_H

#include <stddef.h>
#include <stdint.h>

enum uart_baud_rate : uint32_t {
    UART_BAUD_300    = 300,     // Very slow e.g. legacy/long-range or noisy lines
    UART_BAUD_1200   = 1200,    // Legacy low-speed devices
    UART_BAUD_2400   = 2400,    // Legacy low-speed devices
    UART_BAUD_4800   = 4800,    // Legacy for older GPS modules
    UART_BAUD_9600   = 9600,    // Classic default baud rate
    UART_BAUD_14400  = 14400,   // Uncommon, some modems
    UART_BAUD_19200  = 19200,   // Mid-speed serial comms
    UART_BAUD_38400  = 38400,   // Common default on many MCUs
    UART_BAUD_57600  = 57600,   // Common for GPS/telemetry
    UART_BAUD_115200 = 115200,  // Most common modern default
    UART_BAUD_230400 = 230400,  // High-speed, e.g. fast flashing/logging
    UART_BAUD_460800 = 460800,  // High-speed, e.g. sensor streaming
    UART_BAUD_921600 = 921600,  // Max common rate; needs short/clean wiring
};

/**
 * Setup the UART interface driver.
 */
void uart_setup(enum uart_baud_rate baudrate);

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
