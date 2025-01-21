#include "cpio.h"
#include "utils.h"

// Parse a cpio archive from the given memory location.
// Return -1 if the archive is invalid, 0 if the archive is valid, and 1 if the
// archive ends. Update next cpio header location by updating the pointer to the
// next header Update the path name and file size by pointer to the
// corresponding location
int parse_cpio(struct cpio_newc_header *cpio_curr_ptr, struct cpio_newc_header **cpio_next_ptr,
               char **cpio_path_name, char **cpio_data)
{
    // Check magic number
    if (strncmp(cpio_curr_ptr->c_magic, C_MAGIC, sizeof(cpio_curr_ptr->c_magic)) != 0) {
        return -1;
    }

    // Transfer file size
    unsigned int file_size = parse_hex_to_str(cpio_curr_ptr->c_filesize, 8);

    // Path name is the end of the header
    *cpio_path_name = (char *)cpio_curr_ptr + sizeof(struct cpio_newc_header);

    // Data is after the path name and padding
    unsigned int path_name_size = parse_hex_to_str(cpio_curr_ptr->c_namesize, 8);
    unsigned int data_offset = sizeof(struct cpio_newc_header) + path_name_size;
    data_offset += (data_offset % 4) ? 4 - (data_offset % 4) : 0;
    *cpio_data = (char *)cpio_curr_ptr + data_offset;

    // Update the pointer to the next header
    unsigned int file_padding = file_size % 4 ? 4 - (file_size % 4) : 0;
    *cpio_next_ptr =
        (struct cpio_newc_header *)((char *)cpio_curr_ptr + data_offset + file_size + file_padding);

    // If file_path is "TRAILER!!!", it is the end of the archive
    if (strncmp(*cpio_path_name, "TRAILER!!!", sizeof("TRAILER!!!")) == 0) {
        return 1;
    }

    return 0;
}