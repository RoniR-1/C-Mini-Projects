#include <stddef.h>
int ft_memcmp(const void* lhs, const void* rhs, size_t count) {
    const unsigned char *a = lhs;
    const unsigned char *b = rhs;
    while (count -- > 0) {
    if (*a != *b) {
        return *a - *b;
    }
    a++;
    b++;
    }
    return 0;    
}
