#include "mini_uart.h"
#include "bootloader.h"

void main(void)
{
	uart_init();
	uart_send_string("UART Initialized\r\n");
	load_kernel();
}