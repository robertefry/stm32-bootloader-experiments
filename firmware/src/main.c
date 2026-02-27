
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

static void delay_millis_approx(unsigned long millis)
{
    unsigned int cycles = millis * (s_RccClockConfig->ahb_frequency / 6000);

    for (unsigned long i = 0; i < cycles; ++i) {
        __asm__("nop");
    }
}

int main(void)
{
    struct rcc_clock_scale const*
        clock = &rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ];

    rcc_setup(clock);
    gpio_setup();

    while (true) {
        gpio_toggle(LED_PORT, LED_PIN);
        delay_millis_approx(1000);
    }

    return 0;
}
