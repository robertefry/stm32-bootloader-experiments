
#include "common/systick.h"
#include "common/uart.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>

#include <stdbool.h>
#include <iso646.h>

#define LED_PORT        GPIOA
#define LED_PIN         GPIO5
#define LED_TIMER       TIM2
#define LED_OC          TIM_OC1

#define UART_PORT       GPIOA
#define UART_RX_PIN     GPIO3
#define UART_TX_PIN     GPIO2

#define PWM_FREQUENCY   1000 // 1kHz
#define PWM_PRECISION   1000

static uint32_t pwm_prescaler(uint32_t timer, uint32_t frequency, uint32_t precision)
{
    uint32_t timer_clock_freq = rcc_get_timer_clk_freq(timer);
    return (timer_clock_freq / (frequency * precision)) - 1;
}

static void setup(void)
{
    // Enable peripheral clocks.
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_TIM2);

    // Enable the LED output comparator.
    timer_set_oc_mode(LED_TIMER, LED_OC, TIM_OCM_PWM1);
    timer_set_oc_value(TIM2, LED_OC, 0);
    timer_enable_oc_output(LED_TIMER, LED_OC);

    // Enable the LED timer.
    timer_set_mode(LED_TIMER, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(LED_TIMER, pwm_prescaler(LED_TIMER, PWM_FREQUENCY, PWM_PRECISION));
    timer_set_period(LED_TIMER, PWM_PRECISION - 1);
    timer_enable_counter(LED_TIMER);

    // Enable the LED pin.
    gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);
    gpio_set_af(LED_PORT, GPIO_AF1, LED_PIN);

    // Enable the UART pins.
    gpio_mode_setup(UART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, UART_RX_PIN | UART_TX_PIN);
    gpio_set_af(UART_PORT, GPIO_AF7, UART_RX_PIN | UART_TX_PIN);
}

static uint32_t map_duty_state(uint32_t num_states, uint32_t state)
{
    state %= 2 * num_states;
    uint32_t index = (state < num_states) ? (state) : (2 * num_states - state);

    // Use approximate squared gamma correction.
    return (index * index * PWM_PRECISION) / (num_states * num_states);
}

static void loop_pwm(void)
{
    static uint32_t const num_duty_states = 50;
    static uint32_t duty_state = 0;

    uint32_t duty_cycle = map_duty_state(num_duty_states, ++duty_state);
    timer_set_oc_value(TIM2, LED_OC, duty_cycle);

    // TODO: Maybe use something less intrusive than a delay.
    // Perhaps a proper timed task scheduler running from the delay interrupt.
    delay_ms(2000 / num_duty_states);
}

static void loop_uart(void)
{
    if (not uart_rx_available()) {
        return;
    }

    uint8_t data;
    if (uart_rx_read(&data, sizeof(data)) != sizeof(data)) {
        // TODO: Handle the error case where we did not read enough bytes.
    }

    // For now, just return back the data we got plus one.
    data += 1;
    if (uart_tx_write(&data, sizeof(data)) != sizeof(data)) {
        // TODO: Handle the error case where we did not write enough bytes.
    }
}

static void loop(void)
{
    loop_pwm();
    loop_uart();
}

int main(void)
{
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
    systick_setup();
    uart_setup(11520);

    setup();

    while (true) {
        loop();
    }

    return 0;
}
