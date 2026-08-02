
#include "common/reset.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdint.h>

#define LED_PORT (GPIOA)
#define LED_PIN  (GPIO5)

int main(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_clear(LED_PORT, LED_PIN);

    for (uint8_t i = 0; i < 6; ++i) {
        gpio_toggle(LED_PORT, LED_PIN);
        for (uint16_t j = 0; j < (rcc_ahb_frequency / 500); ++j) {
            __asm__("nop");
        }
    }

    reset(FIRMWARE_BASE);
    return 0;
}
