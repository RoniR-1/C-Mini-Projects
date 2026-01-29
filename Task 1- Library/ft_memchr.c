#include <stddef.h>
void* ft_memchr(const void* ptr, int ch, size_t count) {
    const unsigned char *s = ptr;
    while (count-- > 0) {
        if ((unsigned char)(*(s)) == (unsigned char) ch) {
            return ((void*)s);
        }
        s++;
    }
    return 0;
}