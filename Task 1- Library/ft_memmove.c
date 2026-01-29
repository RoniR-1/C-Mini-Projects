#include <stddef.h>
void* ft_memmove( void* dest, const void* src, size_t count ) {
    if (count == 0) {
        return dest;
    }
    unsigned char *d = dest;
    const unsigned char *s = src;
    if (d > s) {
        for (size_t i = count; i > 0; i--) {
            *(d+i-1) = *(s+i-1);
        }
    } else {
        for (size_t i = 0; i < count; i++) {
            *d++ = *s++;
        }
    }
    return dest;
}