#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char *ft_strmapi(char const *s, char (*f)(unsigned int, char)) {
    size_t i = 0;
    char *result = malloc(ft_strlen(s) + 1); // 1 for null, dont forget !
    if (result == 0) {
        return 0;
    }
    while (s[i] != 0) {
        result[i] = f(i, s[i]);
        i++;
    }
    result[i] = 0;
    return result;
}