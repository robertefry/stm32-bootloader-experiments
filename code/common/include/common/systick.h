
#ifndef COMMON_SYSTICK_H
#define COMMON_SYSTICK_H

#include <stdint.h>

/**
 * Setup the system interrupt clock.
 */
void systick_setup(void);

/**
 * Get the number of milliseconds since reset.
 *
 * Requires `systick_setup` be called during setup.
 */
uint32_t millis(void);

/**
 * Delay for a number of milliseconds.
 * Implemented as a busy wait.
 */
void delay_ms(uint32_t ms);

#endif /* COMMON_SYSTICK_H */
