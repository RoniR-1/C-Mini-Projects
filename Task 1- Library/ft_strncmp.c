#include <stddef.h>
int ft_strncmp(const char* lhs, const char* rhs, size_t count) {
    size_t i = 0;
    while (i < count && lhs[i] && lhs[i] == rhs[i]) {
        i++;
    }
    if (i == count) {
        return 0;
    }
    return (unsigned char)lhs[i] - (unsigned char)rhs[i];
}