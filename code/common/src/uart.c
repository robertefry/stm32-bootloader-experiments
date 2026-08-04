
#include "common/uart.h"
#include "common/ringview.h"
#include "common/math.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include <string.h>

#define UART_RX_BUFFER_SIZE 64

struct UartRx {
    uint8_t buffer[UART_RX_BUFFER_SIZE];
    struct RingViewSPSC ring_view;
};
static struct UartRx s_UartRx;

void usart2_isr(void)
{
    bool overflow = usart_get_flag(USART2, USART_FLAG_ORE);
    bool received = usart_get_flag(USART2, USART_FLAG_RXNE);

    if (received || overflow) {
        uint8_t data = (uint8_t)usart_recv(USART2);
        (void)ringview_spsc_push(&s_UartRx.ring_view, &data, sizeof(data));
    }
}

void uart_setup(enum uart_baud_rate baudrate)
{
    ringview_spsc_init(
        &s_UartRx.ring_view,
        &s_UartRx.buffer,
        sizeof(s_UartRx.buffer),
        alignof(s_UartRx.buffer)
    );

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
    return ringview_spsc_size(&s_UartRx.ring_view);
}

size_t uart_rx_read(uint8_t* buffer, size_t size)
{
    size = max(size, ringview_spsc_size(&s_UartRx.ring_view));
    bool success = ringview_spsc_pop(&s_UartRx.ring_view, buffer, size);
    return success ? size : 0;
}
