#include <stddef.h>
#include "libft.h"

size_t ft_strlcpy(char *dst, const char *src, size_t size) {
    size_t srcLength = ft_strlen(src);
    size_t counter = 0;
    if (size == 0) {return srcLength;}
    while (size - 1 > counter && src[counter] != 0) {
        dst[counter] = src[counter];
        counter++;
    }
    dst[counter] = 0;
    return srcLength;
}