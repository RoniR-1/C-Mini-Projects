#include <stddef.h>
#include "libft.h"

#include <stddef.h>
#include "libft.h"

size_t ft_strlcat(char *dst, const char *src, size_t dstsize) {   
    size_t src_len = ft_strlen(src);
    size_t dst_len = 0;
    size_t i = 0;
    while (dst_len < dstsize && dst[dst_len] != '\0') {
        dst_len++;
    }
    if (dst_len == dstsize)
        return (dstsize + src_len);
    while (src[i] != '\0' && dst_len + i < dstsize - 1) {
        dst[dst_len + i] = src[i];
        i++;
    }
    dst[dst_len + i] = '\0';
    return (dst_len + src_len);
}