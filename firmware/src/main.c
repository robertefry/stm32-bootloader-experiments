
#include "systick.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdbool.h>

#define LED_PORT (GPIOA)
#define LED_PIN  (GPIO5)

static void rcc_setup(struct rcc_clock_scale const* clock) {
    rcc_clock_setup_pll(clock);
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

int main(void)
{
    struct rcc_clock_scale const*
        clock = &rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ];

    rcc_setup(clock);
    gpio_setup();
    systick_setup(clock->ahb_frequency);

    while (true) {
        gpio_toggle(LED_PORT, LED_PIN);
        delay_ms(1000);
    }

    return 0;
}
