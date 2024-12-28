#include "mini_uart.h"

void main(void)
{
	uart_init();
	uart_send_string("UART Initialized...\r\n");
	uart_send_string("Hello, world!\r\n");

	while (1) {
		uart_send(uart_recv());
		uart_send_string("\r\nReceived: ");
	}
}