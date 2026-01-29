#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

void* ft_calloc(size_t num, size_t size)
{
    size_t total = num * size;
    void *ptr = malloc(total);
    if (ptr != NULL)
        ft_bzero(ptr, total);
    return ptr;
}