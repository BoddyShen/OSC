#include "mini_uart.h"
#include "shell.h"

void main(void)
{
	uart_init();
	uart_send_string("UART Initialized\r\n");
	uart_send_string("Shell Initialized, type help or hello!\r\n");

	while (1) {
		uart_send_string("# ");
		cmd_clean();
		cmd_read();
		cmd_exec();
	}
}