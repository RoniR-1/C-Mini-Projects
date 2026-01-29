#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char *ft_strjoin(char const *s1, char const *s2) {
    size_t lens1 = ft_strlen(s1);
    size_t lens2 = ft_strlen(s2);
    char* result =  malloc(lens1+ lens2 + 1);
    if (result == 0) {return 0;} //allocate len s1 + len s2 + null
    size_t counter = 0;
    while (lens1 > counter) {
        result[counter] = s1[counter];
        counter++;
    }
    while (lens1 + lens2 > counter) { //could have >= instead of result[counter] = 0
        result[counter] = s2[counter - lens1];
        counter++;
    }
    result[counter] = 0;
    return result;
}