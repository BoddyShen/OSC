#include "mini_uart.h"

#define KERNEL_LOAD_ADDR  0x60000
#define ACK  0x06  // Define ACK (0x06 indicates acknowledgment)
#define START_TAG 0x1B
#define END_TAG 0xFF  // Define END_TAG (0xFF indicates end of transmission)

void send_ack() {
    uart_send(ACK);
}


void load_kernel()
{
    unsigned int kernel_size = 0;
    char *kernel = (char *)KERNEL_LOAD_ADDR;

    // Wait for header (0x1B)
    while (uart_recv() != START_TAG);
    send_ack();

    // Receive kernel size (64-bit, sent in little-endian)
    for (int i = 0; i < 8; i++) {
        kernel_size |= (uart_recv() << (i * 8));
    }
    send_ack();

    // Start receiving kernel image
    for (unsigned int i = 0; i < kernel_size; i++) {
        kernel[i] = uart_recv();
    }

    // Wait for end-of-transmission marker (0x00)
     while (uart_recv() != END_TAG);
     send_ack();

    // Jump to kernel address and start execution
    ((void (*)())KERNEL_LOAD_ADDR)();
}
