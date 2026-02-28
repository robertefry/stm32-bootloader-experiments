
#include "systick.h"

#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

#include <stdint.h>

static volatile uint64_t s_SysTicksMS = 0;

void systick_setup(void)
{
    struct rcc_clock_scale const*
        clock = &rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ];

    rcc_clock_setup_pll(clock);

    systick_set_frequency(1000, clock->ahb_frequency);
    systick_counter_enable();
    systick_interrupt_enable();
}

void sys_tick_handler(void)
{
    // TODO: Use libc atomics library.
    // FIXME: Disable interrupts, while we're on a single-core MPU.
    s_SysTicksMS += 1;
}

uint64_t millis()
{
    return s_SysTicksMS;
}

void delay_ms(uint64_t ms)
{
    uint64_t start = millis();

    while ((millis() - start) < ms) {
        __asm__("nop");
    }
}
