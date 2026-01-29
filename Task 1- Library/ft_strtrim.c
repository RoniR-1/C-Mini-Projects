#include <stddef.h>
#include <stddef.h>
#include "libft.h"


//if contains, return 1. else 0

static int strContains(char const *set, char c) {
    size_t counter = 0;
    while (set[counter] != 0) {
        if (set[counter] == c) {
            return 1;
        }
        counter++;
    }
    return 0;
}

char *ft_strtrim(char const *s1, char const *set) {
    if (s1 == 0 || set == 0) {return 0;}
    if (s1[0] == '\0') {
        return (ft_strdup(""));
    }
    size_t start = 0;
    while(s1[start] != 0 && strContains(set, s1[start]) == 1) {
        start++;
    }
    size_t len = ft_strlen(s1);
    size_t end = len - 1;
    while (strContains(set, s1[end]) == 1 && end > start) {
        end--;
    }
    return ft_substr(s1, start, end - start + 1);
}

