
#include "systick.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdbool.h>

#define LED_PORT (GPIOA)
#define LED_PIN  (GPIO5)

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

int main(void)
{
    systick_setup();
    gpio_setup();

    while (true) {
        gpio_toggle(LED_PORT, LED_PIN);
        delay_ms(1000);
    }

    return 0;
}
