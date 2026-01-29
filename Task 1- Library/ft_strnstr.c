#include <stddef.h>

static int is_match(const char *str, const char *substr, size_t start, size_t len) {
    size_t k = 0; 
    while ((start + k) < len &&
           str[start + k] != '\0' && 
           substr[k] != '\0' && 
           str[start + k] == substr[k]) {
        k++;
    }
    // Return 1 if match, 0 if no match
    return (substr[k] == '\0');
}

char *ft_strnstr(const char *str, const char *substr, size_t len) {
    size_t i;  
    if (substr[0] == '\0') {
        return ((char *)str);
    }
    i = 0;
    while (str[i] != '\0' && i < len) {
        if (is_match(str, substr, i, len))
            return ((char *)&str[i]);
        i++;
    }
    return (0);
}