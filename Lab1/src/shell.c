#include "mini_uart.h"
#include "shell.h"
#include "utils.h"
#include "mailbox.h"

CMDS cmd_list[CMDS_LIST_LEN] = {
    {"help", "print all available commands", do_help},
    {"hello", "print Hello World!", do_hello},
    {"info", "get board and ARM memory info", do_info}
};

char buffer[BUFFER_SIZE];

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

void cmd_read( void ) 
{
    char c;
    int i = 0;
    while (1)
    {
        c = uart_recv();
        if (c == '\n' || c == '\r')
        {
            buffer[i] = '\0';
            uart_send_string("\r\n");
            return;
        }
        buffer[i++] = c;
        uart_send(c);

        if (i >= BUFFER_SIZE - 1) { // buffer full
            buffer[i] = '\0';
            uart_send_string("\r\nBuffer full, try shorter command!\r\n");
            cmd_clean();
            return;
        }
    }
}

void cmd_clean( void )
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = '\0';
    }
}

void cmd_exec ( void )
{
    for (int i = 0; i < CMDS_LIST_LEN; i++)
    {
        if (strcmp(buffer, cmd_list[i].command) == 0)
        {
            if (cmd_list[i].func != NULL) {
                cmd_list[i].func();
            } else {
                uart_send_string("Error: Command exists but has no function.\r\n");
            }
            return;
        }
    }
    uart_send_string("Command not found, type help for available commands\r\n");
}

void do_help ( void )
{
    for (int i = 0; i < CMDS_LIST_LEN; i++)
    {
        uart_send_string(cmd_list[i].command);
        uart_send_string(": ");
        uart_send_string(cmd_list[i].help);
        uart_send_string("\r\n");
    }
}

void do_hello ( void )
{
    uart_send_string("Hello World!\r\n");
}

void do_info ( void )
{
    get_board_revision();
    get_ARM_memory();
}
