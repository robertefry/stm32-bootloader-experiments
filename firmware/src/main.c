
#include "systick.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdbool.h>

#define LED_PORT (GPIOA)
#define LED_PIN  (GPIO5)

static void setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

static void loop(void)
{
    gpio_toggle(LED_PORT, LED_PIN);
    delay_ms(1000);
}

int main(void)
{
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
    systick_setup();

    setup();

    while (true) {
        loop();
    }

    return 0;
}
