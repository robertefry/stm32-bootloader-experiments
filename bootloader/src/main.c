
#include "common/memorymap.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>

#include <stdint.h>

#define LED_PORT (GPIOA)
#define LED_PIN  (GPIO5)

static void jump_to_firmware(void)
{
    vector_table_t* table = (vector_table_t*)(FIRMWARE_BASE);
    // TODO: Reset the state of the chip, and disable interrupts, before jumping.

    SCB_VTOR = (uint32_t)table;
    table->reset();
}

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

    jump_to_firmware();
    return 0;
}
