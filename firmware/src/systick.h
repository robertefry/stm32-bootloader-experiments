
#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/**
 * Setup the system interrupt clock.
 */
void systick_setup(uint32_t ahb_frequency);

/**
 * Get the number of milliseconds since reset.
 *
 * Requires `systick_setup` be called during setup.
 */
uint64_t millis(void);

/**
 * Delay for a number of milliseconds.
 * Implemented as a busy wait.
 */
void delay_ms(uint64_t ms);

#endif /* SYSTICK_H */
