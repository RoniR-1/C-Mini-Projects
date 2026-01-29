#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

void ft_striteri(char *s, void (*f)(unsigned int, char*)) {
    size_t i = 0;
    while (s[i] != 0) {
        f(i, s + i);
        i++;
    }
}
