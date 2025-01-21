#include "utils.h"

int strncmp(const char *str1, const char *str2, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    return 0;
}

// Parse an ASCII string representing a hexadecimal number to an unsigned
// integer. (big-endian)
unsigned int parse_hex_to_str(char *s, unsigned int len)
{
    unsigned int r = 0;
    for (unsigned int i = 0; i < len; i++) {
        r *= 16;
        if (s[i] >= '0' && s[i] <= '9') {
            r += s[i] - '0';
        } else if (s[i] >= 'a' && s[i] <= 'f') {
            r += s[i] - 'a' + 10;
        } else if (s[i] >= 'A' && s[i] <= 'F') {
            r += s[i] - 'A' + 10;
        } else {
            return r;
        }
    }
    return r;
}
