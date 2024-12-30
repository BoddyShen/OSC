#ifndef	_SHELL_H
#define	_SHELL_H

#define CMDS_LIST_LEN 3
#define CMDS_NAME_LEN 16
#define CMDS_HELP_LEN 128
#define BUFFER_SIZE 128 

typedef struct CMDS
{
    char command[CMDS_NAME_LEN];
    char help[CMDS_HELP_LEN];
    void (*func)(void);
} CMDS;

int strcmp(const char *str1, const char *str2);

void cmd_read ( void);
void cmd_clean ( void);

void do_help ( void );
void do_hello ( void );
void do_info ( void );

void cmd_exec ( void);

#endif  /*_SHELL_H */