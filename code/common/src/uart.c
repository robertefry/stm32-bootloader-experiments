
#include "common/uart.h"
#include "common/ringview.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include <string.h>

#define UART_RX_BUFFER_SIZE 64

struct UartRx {
    uint8_t buffer[UART_RX_BUFFER_SIZE];
    struct RingView ring_view;
};
static struct UartRx s_UartRx;

void usart2_isr(void)
{
    // FIXME: Disable interrupts while running this ISR.

    bool overflow = usart_get_flag(USART2, USART_FLAG_ORE);
    bool received = usart_get_flag(USART2, USART_FLAG_RXNE);

    if (received || overflow) {
        uint8_t data = (uint8_t)usart_recv(USART2);
        ringview_push(&s_UartRx.ring_view, &data);
    }
}

void uart_setup(uint32_t baudrate)
{
    ringview_init(&s_UartRx.ring_view, &s_UartRx.buffer, UART_RX_BUFFER_SIZE);

    rcc_periph_clock_enable(RCC_USART2);

    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_set_baudrate(USART2, baudrate);
    usart_set_parity(USART2, 0);
    usart_set_stopbits(USART2, 1);

    usart_enable_rx_interrupt(USART2);
    nvic_enable_irq(NVIC_USART2_IRQ);

    usart_enable(USART2);
}

size_t uart_tx_write(uint8_t const* buffer, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        usart_send_blocking(USART2, (uint16_t)buffer[i]);
    }
    return size;
}

size_t uart_rx_available(void)
{
    return ringview_size(&s_UartRx.ring_view);
}

size_t uart_rx_read(uint8_t* buffer, size_t size)
{
    return ringview_pop(&s_UartRx.ring_view, buffer);
}
