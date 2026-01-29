#include <stddef.h>
void ft_bzero(void *s, size_t n) {
    unsigned char *actualPointer = s;
    while(n > 0) {
        n--;
        *(actualPointer + n) = 0;
    }
}