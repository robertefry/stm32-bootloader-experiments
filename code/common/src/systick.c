
#include "common/systick.h"

#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

#include <stdint.h>
#include <stdatomic.h>

static atomic_uint_fast32_t s_SysTicksMS = 0;

void sys_tick_handler(void)
{
    static_assert(atomic_is_lock_free(&s_SysTicksMS));
    atomic_fetch_add_explicit(&s_SysTicksMS, 1, memory_order_relaxed);
}

void systick_setup(void)
{
    systick_set_frequency(1000, rcc_ahb_frequency);
    systick_counter_enable();
    systick_interrupt_enable();
}

uint32_t millis()
{
    return atomic_load_explicit(&s_SysTicksMS, memory_order_relaxed);
}

void delay_ms(uint32_t ms)
{
    uint32_t start = millis();

    while (millis() - start < ms) {
        __asm__("nop");
    }
}
