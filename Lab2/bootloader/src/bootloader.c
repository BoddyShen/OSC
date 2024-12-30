#include "mini_uart.h"

#define KERNEL_LOAD_ADDR  0x80000
#define ACK  0x06  // Define ACK (0x06 indicates acknowledgment)
#define END_TAG 0x00  // Define END_TAG (0x00 indicates end of transmission)

void send_ack() {
    uart_send(ACK);
}


void load_kernel()
{
    unsigned int kernel_size = 0;
    char *kernel = (char *)KERNEL_LOAD_ADDR;

    // Wait for header (0x1B)
    while (uart_recv() != 0x1B);
    send_ack();

    // Receive kernel size (64-bit, sent in little-endian)
    for (int i = 0; i < 8; i++) {
        kernel_size |= (uart_recv() << (i * 8));
    }

    send_ack();

    // Start receiving kernel image
    for (unsigned int i = 0; i < kernel_size; i++) {
        kernel[i] = uart_recv();
        
        // Send ACK for every block of 256 bytes or at the end of the transfer
        if ((i + 1) % 256 == 0 || i == kernel_size - 1) {
            send_ack();
        }
    }

    // Wait for end-of-transmission marker (0x00)
    while (uart_recv() != END_TAG);
    send_ack();

    // Jump to kernel address and start execution
    ((void (*)())KERNEL_LOAD_ADDR)();
}