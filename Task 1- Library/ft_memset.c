#include <stddef.h>
void* ft_memset(void* dest, int ch, size_t count) {
    unsigned char * bytelocation = dest;
    unsigned char replacement = (unsigned char) ch;
    while(count > 0) {
        count--;
        *(bytelocation + count) = replacement;
    }
    return dest;
}