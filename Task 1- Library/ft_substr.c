#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char *ft_substr(char const *s, unsigned int start, size_t len) {
    if (s == 0) {return 0;}
    if (start >= ft_strlen(s)) {return ft_strdup("");}
    size_t s_len = ft_strlen(s);
    if (len > s_len - start) {len = s_len - start;}
    char *result = (char*)malloc(len+1);
    if (result == 0) {return 0;}
    //what if len == 0?
    size_t counter = 0;
    while (counter < len) {
        result[counter] = s[start + counter];
        counter++;
    }
    result[counter] = 0;
    return result;
}