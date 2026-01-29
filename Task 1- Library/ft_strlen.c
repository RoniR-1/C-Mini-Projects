#include <stddef.h>
size_t ft_strlen(const char* str) {
    if (str == NULL) {
        return 0;
    }
    size_t length = 0;
    while(*(str + length) != 0) {
        length++;
    }
    return length;
};