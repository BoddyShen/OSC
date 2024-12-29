#include "rpi3_peripherals/mini_uart.h"
#include "rpi3_peripherals/gpio.h"

void uart_init ( void )
{
    register unsigned int r;

    // map UART1 to GPIO pins, refer to page 101, 102 of BCM2837-ARM-Peripherals.pdf
    r = *GPFSEL1;
    r &= ~(7<<12);           // clean gpio14
    r |= 2<<12;              // set alt5 for gpio14
    r &= ~(7<<15);           // clean gpio15
    r |= 2<<15;              // set alt5 for gpio15
    *GPFSEL1 = r;

    // Set pull-up/down for GPIO pins 14 and 15 to neither
    *GPPUD = 0;            
    r=150; while(r--) { asm volatile("nop"); } // wait 150 cycles
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = 0;        

    // initialize UART
    *AUX_ENABLES = 1;       // enable UART1, AUX mini uart
    *AUX_MU_CNTL_REG = 0;   // Disable transmitter and receiver during configuration.
    *AUX_MU_LCR_REG = 3;    // Enable 8 bit mode
    *AUX_MU_MCR_REG = 0;    // Set RTS line to be always high
    *AUX_MU_IER_REG = 0;    // Disable interrupts, wait until interrupts lesson
    *AUX_MU_BAUD_REG = 270; // 115200 baud, system clock is 250MHz
    *AUX_MU_IIR_REG = 6;    // No FIFO

    *AUX_MU_CNTL_REG = 3;   // Enable receiver and transmitter
}

char uart_recv ( void )
{
    // wait until the UART has received something, check bit 0 of AUX_MU_LSR_REG
    while(1) {
        if(*AUX_MU_LSR_REG & 0x01) break;
    }
    return (char)(*AUX_MU_IO_REG);
}

void uart_send ( char c )
{
    // wait until the UART is ready to transmit, check bit 5 of AUX_MU_LSR_REG
    while(1) {
        if(*AUX_MU_LSR_REG & 0x20) break;
    }
    *AUX_MU_IO_REG = c;
}

void uart_send_string(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        uart_send((char)str[i]);
    }
}

void uart_2hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c = 28; c >= 0; c -= 4) {
        n = (d >> c) & 0xF;  // Extract 4 bits (nibbles), from the highest to the lowest
        n += n > 9 ? 0x37 : 0x30;  // Convert the number to an ASCII character
        uart_send(n);
    }
}