#ifndef _CPIO_H_
#define _CPIO_H_

/*
    New ASCII Format Cpio Archive,
   https://man.freebsd.org/cgi/man.cgi?query=cpio&sektion=5 header + pathname +
   padding( NUL, \0) + file data + padding ...
*/

#define C_MAGIC "070701"
#define CPIO_START_ADDR (void *)0x8000000

struct cpio_newc_header {
    char c_magic[6]; // "070701"
    char c_ino[8];
    char c_mode[8];
    char c_uid[8];
    char c_gid[8];
    char c_nlink[8];
    char c_mtime[8];
    char c_filesize[8];
    char c_devmajor[8];
    char c_devminor[8];
    char c_rdevmajor[8];
    char c_rdevminor[8];
    char c_namesize[8];
    char c_check[8];
};

int parse_cpio(struct cpio_newc_header *cpio_curr_ptr, struct cpio_newc_header **cpio_next_ptr,
               char **cpio_path_name, char **cpio_data);

#endif /* _CPIO_H_ */