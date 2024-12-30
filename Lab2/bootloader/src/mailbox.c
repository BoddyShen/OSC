#include "mailbox.h"
#include "mini_uart.h"

#include "rpi3_peripherals/base.h"

// https://jsandler18.github.io/extra/mailbox.html
// https://github.com/raspberrypi/firmware/wiki/Mailboxes
// https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface


#define MAILBOX_BASE    PBASE + 0xb880

#define MAILBOX_READ    ((volatile unsigned int*)(MAILBOX_BASE + 0x00))
#define MAILBOX_STATUS  ((volatile unsigned int*)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE   ((volatile unsigned int*)(MAILBOX_BASE + 0x20))

#define MAILBOX_EMPTY   0x40000000
#define MAILBOX_FULL    0x80000000

#define MAILBOX_CHANNEL_ARM_TO_VC  8
#define MAILBOX_CHANNEL_VC_TO_ARM  9

#define GET_BOARD_REVISION  0x00010002
#define GET_ARM_MEMORY      0x00010005

#define REQUEST_CODE        0x00000000
#define REQUEST_SUCCEED     0x80000000
#define REQUEST_FAILED      0x80000001
#define TAG_REQUEST_CODE    0x00000000
#define END_TAG             0x00000000

// https://jsandler18.github.io/extra/mailbox.html
// https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
void mailbox_call(int channel, unsigned int msg_addr)
{
    // Transfer 64 bit message address to 4 lower bit channel + 28 upper bit  address
    unsigned int msg = (msg_addr & ~0xF) | channel;

    while(*MAILBOX_STATUS & MAILBOX_FULL); // wait until mailbox is not full
    *MAILBOX_WRITE = msg; // write the message to mailbox

    while(*MAILBOX_STATUS & MAILBOX_EMPTY); // wait until mailbox is not empty

    while(1)
    {
        if(*MAILBOX_READ == msg)
            return;
    } 
}


void get_board_revision(){
  volatile unsigned int __attribute__((aligned(16))) mailbox[64];
  mailbox[0] = 7 * 4; // buffer size in bytes
  mailbox[1] = REQUEST_CODE;
  // tags begin
  mailbox[2] = GET_BOARD_REVISION; // tag identifier
  mailbox[3] = 4; // maximum of request and response value buffer's length.
  mailbox[4] = TAG_REQUEST_CODE;
  mailbox[5] = 0; // return value buffer
  // tags end
  mailbox[6] = END_TAG; // 4 bytes * 6 = 24 = 0x18

  mailbox_call(MAILBOX_CHANNEL_ARM_TO_VC, (unsigned int)((unsigned long)&mailbox));
  if (mailbox[1] == REQUEST_SUCCEED) {
        uart_send_string("Board revision: ");
        uart_2hex(mailbox[5]); //it should be 0xa020d3 for rpi3 b+, actual 00a02082
        uart_send_string("\r\n");
    } else {
        uart_send_string("Failed to get board revision.\r\n");
    } 
}

void get_ARM_memory(){
  volatile unsigned int __attribute__((aligned(16))) mailbox[64];
  mailbox[0] = 8 * 4; // buffer size in bytes
  mailbox[1] = REQUEST_CODE;
  // tags begin
  mailbox[2] = GET_ARM_MEMORY; // tag identifier
  mailbox[3] = 8; // maximum of request and response value buffer's length.
  mailbox[4] = TAG_REQUEST_CODE;
  mailbox[5] = 0; // Value buffer (for base address)
  mailbox[6] = 0; // Value buffer (for size)

  // tags end
  mailbox[7] = END_TAG; // 4 bytes * 6 = 24 = 0x18

  mailbox_call(MAILBOX_CHANNEL_ARM_TO_VC, (unsigned int)((unsigned long)&mailbox));
  if (mailbox[1] == REQUEST_SUCCEED) {
        uart_send_string("ARM Memory Base Address: ");
        uart_2hex(mailbox[5]);
        uart_send_string("\r\n");
        uart_send_string("ARM Memory size: ");
        uart_2hex(mailbox[6]);
        uart_send_string("\r\n");
    } else {
        uart_send_string("Failed to get board revision.\r\n");
    } 
}