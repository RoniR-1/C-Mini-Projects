#include <stddef.h>
#include <stdlib.h>
void* ft_memcpy(void* dest, const void* src, size_t count ) {
    unsigned char *destchar = dest;
    const unsigned char *srcchar = src;
    while (count--> 0) {
        *destchar++ = *srcchar++;
    }
    return dest;
}