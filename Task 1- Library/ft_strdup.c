#include <stdlib.h>
#include "libft.h"

char* ft_strdup(const char *s) {
    size_t len;
    char *dup;
    len = ft_strlen(s);
    dup = (char *)malloc(sizeof(char) * (len + 1));    
    if (dup == NULL)
        return NULL;
    ft_strlcpy(dup, s, len + 1);
    return dup;
}