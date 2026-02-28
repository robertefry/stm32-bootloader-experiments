
#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void systick_setup(uint32_t ahb_frequency);

uint64_t millis(void);

void delay_ms(uint64_t ms);

#endif /* SYSTICK_H */
