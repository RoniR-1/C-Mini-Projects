#include <stddef.h>

int ft_atoi(const char* str) {
    size_t i = 0;
    int result = 0;
    int sign = 1;
    char c;    
    while ((c = str[i]) && ((c >= 9 && c <= 13) || c == ' ')) { // remove whitespace
        i++;
    }
    c = str[i];
    if (c == '-') { //minus
        sign = -1;
        i++;
    } else if (c == '+') { //plus
        i++;
    }
    while ((c = str[i]) && c >= '0' && c <= '9') { // parsing
        result = result * 10 + (c - '0');
        i++;
    }
    return result * sign;
}